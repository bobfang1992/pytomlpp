#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include <toml++/toml.h>

using namespace std::string_view_literals;

TEST (TEST_LIB_CONFIG, TEST_EXCEPTION_OK){
    static constexpr auto source = R"(
        [library]
        name = "toml++"
        authors = ["Mark Gillard <mark.gillard@outlook.com.au>"]
    
        [dependencies]
        cpp = 17
    )"sv;
    auto tbl = toml::parse(source);

    EXPECT_EQ(tbl["dependencies"]["cpp"], 17);
}

TEST (TEST_LIB_CONFIG, TEST_EXCEPTION_NOT_VALID){
    EXPECT_THROW({
        static constexpr auto source = R"(
            [library]
            name = "toml++"
            authors = ["Mark Gillard <mark.gillard@outlook.com.au>"
        )"sv;
        auto tbl = toml::parse(source);
    }, toml::parse_error);
}

// TEST (TEST_TABLE_TO_DICT, TEST_INTEGER) {
//     static constexpr auto source = R"(
//         a = 3
//         b = 4
//     )"sv;
//     auto tbl = toml::parse(source);

//     EXPECT_EQ(tbl["a"], 3);
//     EXPECT_EQ(tbl["b"], 4);
// }