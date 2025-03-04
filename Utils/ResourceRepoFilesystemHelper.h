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
        
        const FileInfo* getFileInfo(const std::string& fileName) const;
    };

    FilesHelper& _filesHelper;
    std::string _repoDirectory;
    std::vector<std::unique_ptr<DirectoryInfo>> _directoriesInfos;

    DirectoryInfo& exploredDirectory(DirectoryInfo& directoryInfo);
    void exploreDirectory(DirectoryInfo& directoryInfo);
    const DirectoryInfo* getDirectoryExistingConstInfo(const Path& directoryPath) const;
    DirectoryInfo* getDirectoryExistingInfo(const Path& directoryPath) const;
    DirectoryInfo* getDirectoryInfo(Path directoryName);
    
public:
    // object stores assosiations of case insensitive paths with actual (case sensitive) file paths
    // Constructor params:
    // repoDirectory - directory, to which all in-repo path are relative, must be correct directory path on current filesystem (case sensitive if on case sensitive filesystem)
    // filesHelper - FilesHelper object to access filesystem
    FilesystemHelper(const std::string& repoDirectory, FilesHelper& filesHelper);

    // returns real (case sensitive) file path of given case-insensitive filepath if exists
    // returns empty string if wanted file doesn't exist
    std::string getActualFilesystemFilepath(Path filePath);
    std::string getActualFilesystemDirpath(const Path& directoryPath);
};

#endif // RESOURCEREPOFILESYSTEMHELPER_H_INCLUDED