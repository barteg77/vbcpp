#include "MockFilesHelper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../Utils/FilesystemHelper.h"

using ::testing::Return;

TEST(FilesystemHelperTest, TestTest) {
    MockFilesHelper mfh{};
    FilesystemHelper rrfh{"/testRepoDir", mfh};
    
    EXPECT_CALL(mfh, getDirectoriesList("/testRepoDir"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{"Buses", "Data", "Displays", "fonts"}));
    EXPECT_CALL(mfh, getFilesList("/testRepoDir"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{}));
    
    EXPECT_CALL(mfh, getDirectoriesList("/testRepoDir/Buses"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{"Jelcz", "Neoplan", "MAN"}));
    EXPECT_CALL(mfh, getFilesList("/testRepoDir/Buses"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{}));

    EXPECT_CALL(mfh, getDirectoriesList("/testRepoDir/Buses/Neoplan"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{"Repaints", "sounds", "Texsts", "TEXTURE"}));
    EXPECT_CALL(mfh, getFilesList("/testRepoDir/Buses/Neoplan"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{"config.xml", "desktop_mat.xml", "desktop.fbx"}));
    
    EXPECT_CALL(mfh, getDirectoriesList("/testRepoDir/Buses/Neoplan/TEXTURE"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{"rep_Auwarter", "rep_Polen"}));
    EXPECT_CALL(mfh, getFilesList("/testRepoDir/Buses/Neoplan/TEXTURE"))
        .Times(1)
        .WillOnce(Return(std::vector<std::string>{"AFR200.tga", "AlteTuer.dds", "d92_panel.bmp", "N4009_2D_wagenkasten.dds"}));
    
    const std::string modelActualPath ("/testRepoDir/Buses/Neoplan/TEXTURE/N4009_2D_wagenkasten.dds");
    std::string gotActualPath;
    for (int i=0;i<2;i++) {
        gotActualPath = rrfh.getActualFilesystemFilepath(Path("buses\\Neoplan\\texture\\n4009_2d_wagenkasten.dds"));
        EXPECT_EQ(gotActualPath, modelActualPath);
    }    
}