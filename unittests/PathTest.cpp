#include <gtest/gtest.h>
#include "../Utils/Path.h"

// Path:
// *removes ./ and ../
// *sets letters to lowercase
// *replaces \ with / and removes unnecessary ones
TEST(PathTest, PathNormalizationTest) {

    struct TestData {
        std::string input;
        std::string output;
    };
    std::vector<TestData> td {
        {"", ""},// czy to jest dobre?
        {"q/x/../w/e", "q/w/e"},
        { "a", "a" },
        { "a/.", "a" },
        { "b/../a", "a" },
        { "a/b/..", "a" },
        { "a/b/./..", "a" },
        { "a/b/c/../../", "a" },
        { "/a/", "a" },
        { "./b", "b" },
        {"\\Q/w/E\\a\\/..\\r/T\\y\\//\\\\", "q/w/e/r/t/y"},
    };

    for (const auto & d : td ) {
        Path p(d.input);
        EXPECT_EQ(p.getString(), d.output);
    }
}