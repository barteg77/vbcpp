#ifndef RESOURCEREPOFILESYSTEMHELPER_H_INCLUDED
#define RESOURCEREPOFILESYSTEMHELPER_H_INCLUDED

//#include <algorithm>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <stack>
#include <string>
#include <vector>

#include "FilesHelper.h"
#include "Path.h"

class FilesystemHelper final
{
    struct FilesystemEntryInfo
    {
        std::string _nameLowercase;
        std::string _nameActual;

        FilesystemEntryInfo(const std::string& nameActual)
        : _nameActual(nameActual)
         {
            _nameLowercase.reserve(_nameActual.size());
            for (auto& nameChar : nameActual) {
                _nameLowercase.push_back(static_cast<char>(std::tolower(nameChar)));
            }
        }
    };

    struct FileInfo final : public FilesystemEntryInfo 
    {
        FileInfo(const std::string& nameActual)
        : FilesystemEntryInfo(nameActual) {}
    };

    struct DirectoryInfo final : public FilesystemEntryInfo
    {
        bool _explored;// separate explored for files and directories?
        std::vector<FileInfo> _files;

        DirectoryInfo(const std::string nameActual)
        : FilesystemEntryInfo(nameActual),
          _explored(false) {}
    };

    FilesHelper& _filesHelper;
    std::string _repoDirectory;
    std::vector<std::unique_ptr<DirectoryInfo>> _directoriesInfos;

    DirectoryInfo& exploredDirectory(DirectoryInfo& directoryInfo) {
        if (!directoryInfo._explored) {
            exploreDirectory(directoryInfo);
        }
        return directoryInfo;
    }

    void exploreDirectory(DirectoryInfo& directoryInfo) {
        assert(!directoryInfo._explored);
        std::vector<std::string> directoriesNames = _filesHelper.getDirectoriesList(directoryInfo._nameActual);
        std::vector<std::string> filesNames = _filesHelper.getFilesList(directoryInfo._nameActual);

        for (const std::string& directoryName : directoriesNames) {
            _directoriesInfos.push_back(std::make_unique<DirectoryInfo>(_filesHelper.joinPathsImproved(directoryInfo._nameActual, directoryName)));
        }
        for (const std::string& fileName : filesNames) {
            directoryInfo._files.push_back(FileInfo(fileName));
        }
        directoryInfo._explored = true;
    }

    DirectoryInfo* getDirectoryExistingInfo(const Path& directoryPath) {
        const auto iter = std::find_if(_directoriesInfos.begin(), _directoriesInfos.end(),
                                       [directoryPath] (const std::unique_ptr<DirectoryInfo>& directoryInfo)
                                        { return directoryInfo->_nameLowercase == directoryPath.getString(); }
                                        );
        return (iter == _directoriesInfos.end() ? nullptr : (*iter).get());
    }

    DirectoryInfo* getDirectoryInfo(Path directoryName) {
        std::stack<std::string> missingParts;
        DirectoryInfo* directoryInfo;

        do {
            directoryInfo = getDirectoryExistingInfo(directoryName);
            if (directoryInfo == nullptr && directoryName.getElementsCount() > 0) {
                missingParts.push(directoryName.getBackPart());
                directoryName = directoryName.withoutBackPart();
            }
        } while (directoryInfo == nullptr);

        while (!missingParts.empty()) {
            if (!directoryInfo->_explored) {
                exploreDirectory(*directoryInfo);
            }
            directoryName = directoryName.withBackPart(missingParts.top());
            missingParts.pop();
            directoryInfo = getDirectoryExistingInfo(directoryName);
            if (directoryInfo == nullptr) {
                return nullptr;
            }
        }
        return directoryInfo;//may not be explored
    }
    
public:
    FilesystemHelper(const std::string& repoDirectory, FilesHelper& filesHelper)
    : _repoDirectory(repoDirectory),
      _filesHelper(filesHelper) {
        //upewnic, ze repo directory istnieje
        _directoriesInfos.push_back(std::make_unique<DirectoryInfo>(""));
    }

    std::string getActualFilesystemFilepath(Path filePath){
        const std::string fileName = filePath.getBackPart();
        const Path directoryPath = filePath.withoutBackPart();

        DirectoryInfo* directoryInfo = getDirectoryInfo(directoryPath);
        if (directoryInfo == nullptr){
            throw "nie ma takiego katalogu";
        }
        if (!directoryInfo->_explored) {
            exploreDirectory(*directoryInfo);
        }
        const auto fileInDirectory = std::find_if(directoryInfo->_files.begin(), directoryInfo->_files.end(),
                                     [fileName] (const FileInfo& fileInfo)
                                     { return fileInfo._nameLowercase == fileName; }
                                     );
        if (fileInDirectory == directoryInfo->_files.end()) {
            throw "nie ma takiego pliku";
        }
        return _filesHelper.joinPathsImproved(_filesHelper.joinPathsImproved(_repoDirectory, directoryInfo->_nameActual), fileInDirectory->_nameActual);
    }
};

#endif // RESOURCEREPOFILESYSTEMHELPER_H_INCLUDED