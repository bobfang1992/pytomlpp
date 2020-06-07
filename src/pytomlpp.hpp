#pragma once
#include <chrono>
#include <iostream>
#include <sstream>


#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include <toml.hpp>
namespace py = pybind11;

using namespace pybind11::literals;

namespace pytomlpp {
// declarations
py::dict table_to_dict(const toml::table& t);
py::list array_to_list(const toml::array& a);

py::object toml_date_to_python_date(const toml::date& date) {
    auto PY_DATETIME_MODULE = py::module::import("datetime");
    py::object py_date = PY_DATETIME_MODULE.attr("date")(date.year, date.month, date.day);
    return py_date;
}

py::object toml_time_to_python_time(const toml::time& time) {
    auto PY_DATETIME_MODULE = py::module::import("datetime");
    py::object py_time = PY_DATETIME_MODULE.attr("time")(time.hour, time.minute, time.second, time.nanosecond/1000);
    return py_time;
}

py::object toml_date_time_to_python_date_time(const toml::date_time& dt) {
    auto PY_DATETIME_MODULE = py::module::import("datetime");
    py::object timezone = py::none();
    if(dt.time_offset) {
        py::object time_delta = PY_DATETIME_MODULE.attr("timedelta")("minutes"_a = dt.time_offset.value().minutes);
        timezone = PY_DATETIME_MODULE.attr("timezone")(time_delta);
    }
    py::object py_date_time = py::module::import("datetime")
        .attr("datetime")(dt.date.year, dt.date.month, dt.date.day,
                          dt.time.hour, dt.time.minute, dt.time.second, dt.time.nanosecond/1000, "tzinfo"_a = timezone);
    return py_date_time;
}

// implementations
py::list array_to_list(const toml::array& a) {
    py::list result;
    for(size_t i = 0; i < a.size(); i++) {
        const toml::node* value = a.get(i);
        if(value->type() == toml::node_type::integer) {
            const toml::value<int64_t>* integer_value = value->as_integer();
            const int64_t integer_v = integer_value->get();
            result.append(integer_v);
        } else if (value->type() == toml::node_type::floating_point) {
            const toml::value<double>* float_value = value->as_floating_point();
            const double float_v = float_value->get();
            result.append(float_v);
        } else if (value->type() == toml::node_type::string) {
            const toml::value<std::string>* string_value = value->as_string();
            const std::string string_v = string_value->get();
            result.append(string_v);
        } else if (value->type() == toml::node_type::boolean) {
            const toml::value<bool>* boolean_value = value->as_boolean();
            const bool bool_v = boolean_value->get();
            result.append(bool_v);
        } else if (value->type() == toml::node_type::none) {
            result.append(py::none());
        } else if (value->type() == toml::node_type::table) {
            const toml::table* table_value = value->as_table();
            py::dict d = table_to_dict(*table_value);
            result.append(d); 
        } else if (value->type() == toml::node_type::array) {
            const toml::array* array_value = value->as_array();
            py::list array_v = array_to_list(*array_value);
            result.append(array_v);
        } else if (value->type() == toml::node_type::date) {
            const toml::value<toml::date>* date_value = value->as_date();
            const toml::date date_v = date_value->get();
            auto date = toml_date_to_python_date(date_v);
            result.append(date);         
        } else if (value->type() == toml::node_type::time) {
            const toml::value<toml::time>* time_value = value->as_time();
            const toml::time time_v = time_value->get();
            auto time = toml_time_to_python_time(time_v);
            result.append(time);
        } else if (value->type() == toml::node_type::date_time) {
            const toml::value<toml::date_time>* date_time_value = value->as_date_time();
            const toml::date_time dt_v = date_time_value->get();
            auto datetime = toml_date_time_to_python_date_time(dt_v);
            result.append(datetime);
        }
        else {
            std::stringstream err_message;
            err_message << "cannot convert the type of this node to proper python types: " << value->type() << std::endl;
            std::string err_message_string = err_message.str();
            py::print(err_message_string);
            throw std::runtime_error(err_message_string);
        }
    }
    return result;
}

py::dict table_to_dict(const toml::table& t) {
    py::dict result;
    for(auto it: t) {
        auto key = it.first.c_str();
        const toml::node* value = &(it.second);
        if(value->type() == toml::node_type::integer) {
            const toml::value<int64_t>* integer_value = value->as_integer();
            const int64_t integer_v = integer_value->get();
            result[key] = integer_v;
        } else if (value->type() == toml::node_type::floating_point) {
            const toml::value<double>* float_value = value->as_floating_point();
            const double float_v = float_value->get();
            result[key] = float_v;
        } else if (value->type() == toml::node_type::string) {
            const toml::value<std::string>* string_value = value->as_string();
            const std::string string_v = string_value->get();
            result[key] = string_v;
        } else if (value->type() == toml::node_type::boolean) {
            const toml::value<bool>* boolean_value = value->as_boolean();
            const bool bool_v = boolean_value->get();
            result[key] = bool_v;
        } else if (value->type() == toml::node_type::none) {
            result[key] = py::none();
        } else if (value->type() == toml::node_type::table) {
            const toml::table* table_value = value->as_table();
            py::dict d = table_to_dict(*table_value);
            result[key] = d; 
        } else if (value->type() == toml::node_type::array) {
            const toml::array* array_value = value->as_array();
            py::list array_v = array_to_list(*array_value);
            result[key] = array_v;
        } else if (value->type() == toml::node_type::date) {
            const toml::value<toml::date>* date_value = value->as_date();
            const toml::date date_v = date_value->get();
            auto date = toml_date_to_python_date(date_v);
            result[key] = date;
        } else if (value->type() == toml::node_type::time) {
            const toml::value<toml::time>* time_value = value->as_time();
            const toml::time time_v = time_value->get();
            auto time = toml_time_to_python_time(time_v);
            result[key] = time;
        } else if (value->type() == toml::node_type::date_time) {
            const toml::value<toml::date_time>* date_time_value = value->as_date_time();
            const toml::date_time dt_v = date_time_value->get();
            auto datetime = toml_date_time_to_python_date_time(dt_v);
            result[key] = datetime;
        }
        else {
            std::stringstream err_message;
            err_message << "cannot convert the type of this node to proper python types: " << value->type() << std::endl;
            std::string err_message_string = err_message.str();
            py::print(err_message_string);
            throw std::runtime_error(err_message_string);
        }
    }
    return result;
}

}