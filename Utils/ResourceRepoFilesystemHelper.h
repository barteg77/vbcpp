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
        bool _explored;
        std::vector<FileInfo> _files;

        DirectoryInfo(const std::string nameActual)
        : FilesystemEntryInfo(nameActual),
          _explored(false) {}
    };

    std::string _repoDirectory;
    std::vector<DirectoryInfo> _directoriesInfos;

    void exploreDirectory(DirectoryInfo& directoryInfo) {
        std::vector<std::string> directoriesNames = FilesHelper::getDirectoriesList(directoryInfo._nameActual);
        std::vector<std::string> filesNames = FilesHelper::getFilesList(directoryInfo._nameActual);

        for (const std::string& directoryName : directoriesNames) {
            _directoriesInfos.push_back(DirectoryInfo(FilesHelper::joinPaths(directoryInfo._nameActual, directoryName)));
        }
        for (const std::string& fileName : filesNames) {
            directoryInfo._files.push_back(FileInfo(fileName));
        }
        directoryInfo._explored = true;
    }

    DirectoryInfo* getDirectoryExistingInfo(const Path& directoryPath) {
        return &(*std::find_if(_directoriesInfos.begin(), _directoriesInfos.end(),
                               [directoryPath] (const DirectoryInfo& directoryInfo)
                               { return directoryInfo._nameLowercase == directoryPath.getString(); }
                               ));
    }

    DirectoryInfo* getDirectoryInfo(Path directoryName) {
        std::stack<std::string> missingParts;
        DirectoryInfo* directoryInfo;

        do {
            directoryInfo = getDirectoryExistingInfo(directoryName);
            if (directoryName.getElementsCount() > 0) {
                missingParts.push(directoryName.getBackPart());
                directoryName = directoryName.withoutBackPart();
            }
        } while (directoryInfo == &(*_directoriesInfos.end()));

        while (!missingParts.empty()) {
            if (!directoryInfo->_explored) {
                exploreDirectory(*directoryInfo);
            }
            directoryName = directoryName.withBackPart(missingParts.top());
            missingParts.pop();
            directoryInfo = getDirectoryExistingInfo(directoryName);
            if (directoryInfo == &(*_directoriesInfos.end())) {
                return nullptr;
            }
        }
        return directoryInfo;
    }
    
public:
    FilesystemHelper(const std::string& repoDirectory)
    : _repoDirectory(repoDirectory) {
        //upewnic, ze repo directory istnieje
        _directoriesInfos.push_back(DirectoryInfo(""));
    }

    std::string getActualFilesystemFilepath(Path filePath){
        const std::string fileName = filePath.getBackPart();
        const Path directoryPath = filePath.withoutBackPart();

        DirectoryInfo* directoryInfo = getDirectoryInfo(directoryPath);
        if (directoryInfo == nullptr){
            return "";//nie ma takiego
        }
        if (!directoryInfo->_explored) {
            exploreDirectory(*directoryInfo);
        }
        const auto fileInDirectory = std::find_if(directoryInfo->_files.begin(), directoryInfo->_files.end(),
                                     [fileName] (const FileInfo& fileInfo)
                                     { return fileInfo._nameLowercase == fileName; }
                                     );
        if (fileInDirectory == directoryInfo->_files.end()) {
            return "";//nie ma takiego
        }
        return FilesHelper::joinPaths(FilesHelper::joinPaths(_repoDirectory, directoryInfo->_nameActual), fileInDirectory->_nameActual);
    }
};

#endif // RESOURCEREPOFILESYSTEMHELPER_H_INCLUDED