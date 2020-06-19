#include <fstream> //required for toml::parse_file()

#include <pytomlpp.hpp>

std::string TPP_VERSION = std::to_string(TOML_LIB_MAJOR) + "." +
                          std::to_string(TOML_LIB_MINOR) + "." +
                          std::to_string(TOML_LIB_PATCH);

py::dict loads(std::string_view toml_stirng) {
    auto tbl = toml::parse(toml_stirng);
    return pytomlpp::table_to_dict(tbl);
}

py::dict load(std::string file_name) {
    auto tbl = toml::parse_file(file_name);
    return pytomlpp::table_to_dict(tbl);
}

std::string dumps(py::dict object) {
    const auto& t = pytomlpp::py_dict_to_toml_table(object);
    std::stringstream ss;
    ss << t;
    return ss.str();
}

void dump(py::dict object, std::string filename) {
    const auto& t = pytomlpp::py_dict_to_toml_table(object);
    std::ofstream os(filename);
    os << t;
    os.close();
}

PYBIND11_MODULE(pytomlpp, m) {
    m.doc() = "tomlplusplus python wrapper";
    m.attr("lib_version") = TPP_VERSION;
    m.def("loads", &loads);
    m.def("load", &load);
    m.def("dumps", &dumps);
    m.def("dump", &dump);
}

