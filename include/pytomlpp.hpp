#pragma once
#include <chrono>
#include <iostream>
#include <sstream>


#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include <toml++/toml.h>
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
            throw std::runtime_error(err_message_string);
        }
    }
    return result;
}

toml::array py_list_to_toml_array(const py::list& list);
toml::table py_dict_to_toml_table(const py::dict& object);

toml::date py_date_to_toml_date(const py::handle& date) {
    int year = date.attr("year").cast<py::int_>();
    int month = date.attr("month").cast<py::int_>();
    int day = date.attr("day").cast<py::int_>();

    toml::date d;
    d.year = year;
    d.month = month;
    d.day = day;

    return d;
}

toml::time py_time_to_toml_time(const py::handle& time) {
    int hour = time.attr("hour").cast<py::int_>();
    int minute = time.attr("minute").cast<py::int_>();
    int second = time.attr("second").cast<py::int_>();
    int microsecond = time.attr("microsecond").cast<py::int_>();

    toml::time t;
    t.hour = hour;
    t.minute = minute;
    t.second = second;
    t.nanosecond = microsecond * 1000;

    return t;
}

toml::date_time py_datetime_to_toml_date_time(py::handle& datetime) {
    int year = datetime.attr("year").cast<py::int_>();
    int month = datetime.attr("month").cast<py::int_>();
    int day = datetime.attr("day").cast<py::int_>();
    int hour = datetime.attr("hour").cast<py::int_>();
    int minute = datetime.attr("minute").cast<py::int_>();
    int second = datetime.attr("second").cast<py::int_>();
    int microsecond = datetime.attr("microsecond").cast<py::int_>();
    
    py::object tz_info = datetime.attr("tzinfo");

    toml::date d;
    toml::time t;

    toml::date_time dt;

    d.year = year;
    d.month = month;
    d.day = day;

    t.hour = hour;
    t.minute = minute;
    t.second = second;
    t.nanosecond = microsecond * 1000;

    dt.date = d;
    dt.time = t;

    if(!tz_info.is_none()) {
        toml::time_offset to;
        py::object time_delta = tz_info.attr("utcoffset")(datetime);
        py::object total_seconds_object = time_delta.attr("total_seconds")();
        int total_seconds = total_seconds_object.cast<py::int_>();
        int total_minutes = total_seconds / 60;
        to.minutes = total_minutes;
        dt.time_offset = to;
    }
    return dt;
}

toml::array py_list_to_toml_array(const py::list& list) {
    toml::array arr;

    auto PY_DATETIME_MODULE = py::module::import("datetime");
    auto date_class = PY_DATETIME_MODULE.attr("date");
    auto time_class = PY_DATETIME_MODULE.attr("time");
    auto datetime_class = PY_DATETIME_MODULE.attr("datetime");

    for(auto it: list) {
        if(py::isinstance<py::str>(it)) {
            std::string string_value = it.cast<py::str>();
            arr.push_back(string_value);
        } else if(py::isinstance<py::int_>(it)) {
            int64_t int_value = it.cast<py::int_>();
            arr.push_back(int_value);
        } else if (py::isinstance<py::float_>(it)) {
            double float_value = it.cast<py::float_>();
            arr.push_back(float_value);
        } else if (py::isinstance<py::bool_>(it)) {
            bool bool_value = it.cast<py::bool_>();
            arr.push_back(bool_value);
        } else if (py::isinstance<py::dict>(it)) {
            toml::table t = py_dict_to_toml_table(it.cast<py::dict>());
            arr.push_back(std::move(t));
        } else if (py::isinstance<py::list>(it)) {
            toml::array a = py_list_to_toml_array(it.cast<py::list>());
            arr.push_back(std::move(a));
        } else if (py::isinstance(it, datetime_class)) {
            // Order matters here.
            // isinstance(datetime_obj, datetime) --> true
            // isinstance(datetime_obj, date) --> true as well 
            // so we need to test datetime first then date.
            toml::date_time date_time_value = py_datetime_to_toml_date_time(it);
            arr.push_back(date_time_value);
        } else if (py::isinstance(it, date_class)) {
            toml::date date_value = py_date_to_toml_date(it);
            arr.push_back(date_value);                
        } else if (py::isinstance(it, time_class)) {
            toml::time time_value = py_time_to_toml_time(it);
            arr.push_back(time_value);
        } else {
            std::stringstream ss;
            ss << "not a valid type for converstion " << it << std::endl;
            throw std::runtime_error(ss.str());
        }
    }
    return arr;
}

toml::table py_dict_to_toml_table(const py::dict& object) {
    toml::table t;

    auto PY_DATETIME_MODULE = py::module::import("datetime");
    auto date_class = PY_DATETIME_MODULE.attr("date");
    auto time_class = PY_DATETIME_MODULE.attr("time");
    auto datetime_class = PY_DATETIME_MODULE.attr("datetime");
    
    for(auto it: object) {
        auto key = it.first;
        auto value = it.second;
        if(!py::isinstance<py::str>(key)) {
            throw std::runtime_error("key must be a string...");
        } else {
            std::string key_string = std::string(py::str(key));
            bool insert_ok = true;
            if(py::isinstance<py::int_>(value)) {
                int64_t int_value = value.cast<py::int_>();
                auto insert = t.insert_or_assign(key_string, int_value);
                insert_ok = insert.second;
            } else if (py::isinstance<py::float_>(value)) {
                double float_value = value.cast<py::float_>();
                auto insert = t.insert_or_assign(key_string, float_value);
                insert_ok = insert.second;
            } else if (py::isinstance<py::str>(value)) {
                std::string string_value = value.cast<py::str>();
                auto insert = t.insert_or_assign(key_string, string_value);
                insert_ok = insert.second;
            } else if (py::isinstance<py::bool_>(value)) {
                bool bool_value = value.cast<py::bool_>();
                auto insert = t.insert_or_assign(key_string, bool_value);
                insert_ok = insert.second;
            } else if (py::isinstance<py::dict>(value)) {
                toml::table table_value = py_dict_to_toml_table(value.cast<py::dict>());
                auto insert = t.insert_or_assign(key_string, std::move(table_value));
                insert_ok = insert.second;
            } else if (py::isinstance<py::list>(value)) {
                toml::array array_value = py_list_to_toml_array(value.cast<py::list>());
                auto insert = t.insert_or_assign(key_string, std::move(array_value));
                insert_ok = insert.second;
            } else if (py::isinstance(value, datetime_class)) {
                // Order matters here.
                // isinstance(datetime_obj, datetime) --> true
                // isinstance(datetime_obj, date) --> true as well 
                // so we need to test datetime first then date.
                toml::date_time date_time_value = py_datetime_to_toml_date_time(value);
                auto insert = t.insert_or_assign(key_string, date_time_value);
                insert_ok = insert.second;
            } else if (py::isinstance(value, date_class)) {
                toml::date date_value = py_date_to_toml_date(value);
                auto insert = t.insert_or_assign(key_string, date_value);            
                insert_ok = insert.second;    
            } else if (py::isinstance(value, time_class)) {
                toml::time time_value = py_time_to_toml_time(value);
                auto insert = t.insert_or_assign(key_string, time_value);
                insert_ok = insert.second;
            } else {
                std::stringstream ss;
                ss << "cannot convert value " << value << " to proper toml type" << std::endl;
                throw std::runtime_error(ss.str());
            }
            if(!insert_ok) {
                std::stringstream ss;
                ss << "cannot insert key value pair:" << key << "," << value << std::endl;
                throw std::runtime_error(ss.str());
            }
        }
    }
    return t;
}

}