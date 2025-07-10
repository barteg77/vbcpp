#ifndef FILESYSTEMHELPER_H_INCLUDED
#define FILESYSTEMHELPER_H_INCLUDED

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

/**
 * @brief Associates relative, case-insensitive paths with valid filesystem paths.
 * 
 * FilesystemHelper handles assiciations inside its _repoDirectory. 
 * 
 * FilesystemHelper stores list of paths to directories. Each path is
 * described with its lowercase path relative to _repoDirectory, actual path
 * relative to _repoDirectory and list of files in it (if it was necessary to
 * load it).
 * 
 * Each file is described with its lowercase and actual name.
 */
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
    DirectoryInfo* getDirectoryInfo(Path directoryName, const bool createDirs=false);

    inline bool createDirectory(const Path& inRepoPath) {
        return _filesHelper.createDirectory(_filesHelper.joinPathsImproved(_repoDirectory, inRepoPath.getString()));
    }
    
public:
    /**
     * @param repoDirectory root directory of FilesystemHelper instance (paths you
     * will query it will have to be realtive to it)
     * @param filesHelper filesystem interface
     */
    FilesystemHelper(const std::string& repoDirectory, FilesHelper& filesHelper);

    /**
     * @brief Get valid filesystem path of file
     * @param filePath file path relative to FilesystemHelper's
     * repoDirectory
     * @return case sensitive file path or empty string if it does not exist
     */
    std::string getActualFilesystemFilepath(Path filePath);

    /**
     * @brief Get valid filesystem path of file
     * @param filePath file path relative to FilesystemHelper's
     * repoDirectory
     * @return case sensitive directory path or empty string if it does not exist
     */
    std::string getActualFilesystemDirpath(const Path& directoryPath, const bool createDirs=false);
};

#endif // FILESYSTEMHELPER_H_INCLUDED