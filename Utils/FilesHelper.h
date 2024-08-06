#ifndef FILESHELPER_H_INCLUDED
#define FILESHELPER_H_INCLUDED


#include "imgui.h"
#include <vector>
#include <string>


class FilesHelper
{
    public:
        static bool isFileExists(const std::string& fileName, const std::string& basePath = std::string())
        {
            FILE* file;
            file = fopen((basePath.empty() ? fileName : basePath + '/' + fileName).c_str(), "r");
            if (file)
            {
                fclose(file);
                return true;
            }

            return file;
        }
        static bool doFilesExist(const std::vector<std::string>& fileNames, const std::string& basePath) {
            for(auto& fileName : fileNames) {
                if (!isFileExists(fileName, basePath)) {
                    return false;
                }
            }
            return true;
        }

		static bool isDirectoryExists(const std::string& dirPath);

        virtual std::vector<std::string> getFilesList(const std::string& path);
        virtual std::vector<std::string> getDirectoriesList(const std::string& path);

		static std::string getFileNameFromPath(const std::string& path);
		static std::string getFileExtension(const std::string& fileName);
        static std::string getPathToDirectoryFromFileName(const std::string& fileName);

		static std::string getRelativePathToDir(const std::string& filePath, const std::string& dirPath);
		static std::string getRelativePathToDirInGameDir(std::string filePath, std::string dirPath);
		static bool isInPathSubdir(std::string filePath, std::string dirPath);

		static void copyFile(const std::string& from, const std::string& to);

        static std::string joinPaths(const std::string& part1, const std::string& part2)
        { return part1 + '/' + part2; }
};


#endif // FILESHELPER_H_INCLUDED
