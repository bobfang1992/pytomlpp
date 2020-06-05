#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <pytomlpp.hpp>

namespace py = pybind11;

PYBIND11_MODULE(pytomlpp, m) {
    m.doc() = "tomlplusplus python wrapper";

    m.def("loads", &loads, "parse string into a toml table");

    py::class_<toml::node_view<toml::node>> toml_node(m, "NodeView");
    py::class_<toml::table> toml_table(m, "Table");
    
    toml_table.def("keys", &keys);

    toml_table.def("values", &values);

    toml_table.def("__repr__", [](const toml::table& t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    });
}

