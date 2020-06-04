#include <pybind11/pybind11.h>

#include <add.hpp>
#include <pytomlpp.hpp>

namespace py = pybind11;

struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

PYBIND11_MODULE(pytomlpp, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");

    py::class_<Pet> pet(m, "Pet");
    pet.def(py::init<const std::string &>()).def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
    pet.doc() = "A Pet class";
}

