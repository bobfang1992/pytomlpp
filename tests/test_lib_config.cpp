#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include <pytomlpp.hpp>

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