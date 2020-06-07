#include <fstream> //required for toml::parse_file()

#include <pytomlpp.hpp>

py::dict loads(std::string_view toml_stirng) {
    auto tbl = toml::parse(toml_stirng);
    return pytomlpp::table_to_dict(tbl);
}

py::dict load(std::string file_name) {
    auto tbl = toml::parse_file(file_name);
    return pytomlpp::table_to_dict(tbl);
}

PYBIND11_MODULE(pytomlpp, m) {
    m.doc() = "tomlplusplus python wrapper";
    m.def("loads", &loads);
    m.def("load", &load);
}

