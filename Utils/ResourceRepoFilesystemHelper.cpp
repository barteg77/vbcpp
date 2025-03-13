#include "ResourceRepoFilesystemHelper.h"
#include "Logger.h"

const FilesystemHelper::FileInfo* FilesystemHelper::DirectoryInfo::getFileInfo(const std::string& fileName) const {
    const std::vector<FileInfo>::const_iterator fileInfo = std::find_if(_files.begin(), _files.end(),
                                                                        [fileName] (const FileInfo& fileInfo)
                                                                        { return fileInfo._nameLowercase == fileName; });
    return fileInfo == _files.end() ? nullptr : &(*fileInfo);
}

FilesystemHelper::DirectoryInfo& FilesystemHelper::exploredDirectory(DirectoryInfo& directoryInfo) {
    if (!directoryInfo._explored) {
        exploreDirectory(directoryInfo);
    }
    return directoryInfo;
}

void FilesystemHelper::exploreDirectory(DirectoryInfo& directoryInfo) {
    assert(!directoryInfo._explored);
    const std::string directoryAbsolutePath (_filesHelper.joinPathsImproved(_repoDirectory, directoryInfo._nameActual));
    const std::vector<std::string> directoriesNames (_filesHelper.getDirectoriesList(directoryAbsolutePath));
    const std::vector<std::string> filesNames (_filesHelper.getFilesList(directoryAbsolutePath));

    for (const std::string& directoryName : directoriesNames) {
        _directoriesInfos.push_back(std::make_unique<DirectoryInfo>(_filesHelper.joinPathsImproved(directoryInfo._nameActual, directoryName)));
    }
    for (const std::string& fileName : filesNames) {
        directoryInfo._files.push_back(FileInfo(fileName));
    }
    directoryInfo._explored = true;
}

const FilesystemHelper::DirectoryInfo* FilesystemHelper::getDirectoryExistingConstInfo(const Path& directoryPath) const {
    const auto iter = std::find_if(_directoriesInfos.begin(), _directoriesInfos.end(),
                                    [directoryPath] (const std::unique_ptr<DirectoryInfo>& directoryInfo)
                                    { return directoryInfo->_nameLowercase == directoryPath.getString(); }
                                    );
    return (iter == _directoriesInfos.end() ? nullptr : (*iter).get());
}

FilesystemHelper::DirectoryInfo* FilesystemHelper::getDirectoryExistingInfo(const Path& directoryPath) const {
    const auto iter = std::find_if(_directoriesInfos.begin(), _directoriesInfos.end(),
                                    [directoryPath] (const std::unique_ptr<DirectoryInfo>& directoryInfo)
                                    { return directoryInfo->_nameLowercase == directoryPath.getString(); }
                                    );
    return (iter == _directoriesInfos.end() ? nullptr : (*iter).get());
}

FilesystemHelper::DirectoryInfo* FilesystemHelper::getDirectoryInfo(Path directoryName) {
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

FilesystemHelper::FilesystemHelper(const std::string& repoDirectory, FilesHelper& filesHelper)
: _repoDirectory(repoDirectory),
    _filesHelper(filesHelper) {
    //upewnic, ze repo directory istnieje
    _directoriesInfos.push_back(std::make_unique<DirectoryInfo>(""));
}

std::string FilesystemHelper::getActualFilesystemFilepath(Path filePath){
    const std::string fileName = filePath.getBackPart();
    const Path directoryPath = filePath.withoutBackPart();

    DirectoryInfo* directoryInfo = getDirectoryInfo(directoryPath);
    if (directoryInfo == nullptr){
        return "";
    }
    if (!directoryInfo->_explored) {
        exploreDirectory(*directoryInfo);
    }
    const auto fileInDirectory = std::find_if(directoryInfo->_files.begin(), directoryInfo->_files.end(),
                                    [fileName] (const FileInfo& fileInfo)
                                    { return fileInfo._nameLowercase == fileName; }
                                    );
    if (fileInDirectory == directoryInfo->_files.end()) {
        LOG_DEBUG("Actual path of file \"" + filePath.getString() + "\" not found (this file does not exist)");
        return "";
    }
    std::string actualPath = _filesHelper.joinPathsImproved(_filesHelper.joinPathsImproved(_repoDirectory, directoryInfo->_nameActual), fileInDirectory->_nameActual);
    LOG_DEBUG("Found actual path of file \"" + filePath.getString() + "\", is \"" + actualPath);
    return actualPath;
}

std::string FilesystemHelper::getKnownActualFilesystemFilepath(const Path& filePath) const {
    const Path directoryPath = filePath.withoutBackPart();
    const DirectoryInfo* const directoryInfo = getDirectoryExistingConstInfo(directoryPath);
    assert(directoryInfo != nullptr);

    const std::string fileName = filePath.getBackPart();
    const FileInfo* const fileInfo = directoryInfo->getFileInfo(fileName);

    std::string actualPath = _filesHelper.joinPathsImproved(_filesHelper.joinPathsImproved(_repoDirectory, directoryInfo->_nameActual), fileInfo->_nameActual);
    LOG_DEBUG("Found known actual path of file \"" + filePath.getString() + "\", is \"" + actualPath);
    return actualPath;
}