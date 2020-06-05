#pragma once

#define TOML_EXCEPTIONS 1

#include <string_view>

#include <toml.hpp>

toml::table loads(std::string_view doc) {
    auto tbl = toml::parse(doc);
    return tbl;
}

std::vector<std::string> keys(const toml::table& t) {
    std::vector<std::string> result;
    result.reserve(t.size());
    for(auto it: t) {
        result.push_back(it.first);
    }
    return result;
}

std::vector<toml::node_view<toml::node>> values(toml::table& t) {
    auto ks = keys(t);
    std::vector<toml::node_view<toml::node>> result;
    result.reserve(t.size());
    for(auto k: ks) {
        result.push_back(t[k]);
    }
    return result;
}