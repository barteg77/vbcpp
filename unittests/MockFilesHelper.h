#include "../Utils/FilesHelper.h"
#include <gmock/gmock.h>

class MockFilesHelper : public FilesHelper {
public:
    MOCK_METHOD(std::vector<std::string>, getFilesList, (const std::string& path), (override));
    MOCK_METHOD(std::vector<std::string>, getDirectoriesList, (const std::string& path), (override));
};