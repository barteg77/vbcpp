cmake_minimum_required(VERSION 3.14)
#project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#FetchContent_MakeAvailable(googlemock)

add_executable(
  path_test
  PathTest.cpp
  ../Utils/Path.cpp
  ../Utils/FilesHelper.cpp
  ../Utils/Logger.cpp
)
target_link_libraries(
  path_test
  BulletDynamics BulletCollision BulletSoftBody LinearMath
  GTest::gtest_main
)

add_executable(
  resource_repo_filesystem_helper_test
  FilesystemHelperTest.cpp
  ../Utils/FilesHelper.cpp
  ../Utils/Logger.cpp
  ../Utils/Path.cpp
  ../Utils/FilesystemHelper.cpp
)
target_link_libraries(
  resource_repo_filesystem_helper_test
  BulletDynamics BulletCollision BulletSoftBody LinearMath
  GTest::gtest_main
  gmock_main
)

include(GoogleTest)
#include(GoogleMock)
gtest_discover_tests(path_test)
gtest_discover_tests(resource_repo_filesystem_helper_test)