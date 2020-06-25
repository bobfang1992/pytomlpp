#pragma once

#include <chrono>
#include <iostream>
#include <sstream>

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional.hpp>
#define TOML_OPTIONAL_TYPE tl::optional
#include <toml.hpp>

#include <pytomlpp/toml_types.hpp>

namespace py = pybind11;

using namespace pybind11::literals;

namespace pytomlpp {

struct DecodeError : public std::exception {
  std::string err_message;

  DecodeError(const std::string &message) : err_message(message) {}

  const char *what() const noexcept override { return err_message.c_str(); }
};

// declarations
py::dict toml_table_to_py_dict(const toml::table &t);
py::list toml_array_to_py_list(const toml::array &a);

// implementations
py::list toml_array_to_py_list(const toml::array &a) {
  py::list result;
  for (size_t i = 0; i < a.size(); i++) {
    const toml::node *value = a.get(i);
    if (value->type() == toml::node_type::integer) {
      const toml::value<int64_t> *integer_value = value->as_integer();
      const int64_t integer_v = integer_value->get();
      result.append(integer_v);
    } else if (value->type() == toml::node_type::floating_point) {
      const toml::value<double> *float_value = value->as_floating_point();
      const double float_v = float_value->get();
      result.append(float_v);
    } else if (value->type() == toml::node_type::string) {
      const toml::value<std::string> *string_value = value->as_string();
      const std::string string_v = string_value->get();
      result.append(string_v);
    } else if (value->type() == toml::node_type::boolean) {
      const toml::value<bool> *boolean_value = value->as_boolean();
      const bool bool_v = boolean_value->get();
      result.append(bool_v);
    } else if (value->type() == toml::node_type::none) {
      result.append(py::none());
    } else if (value->type() == toml::node_type::table) {
      const toml::table *table_value = value->as_table();
      py::dict d = toml_table_to_py_dict(*table_value);
      result.append(d);
    } else if (value->type() == toml::node_type::array) {
      const toml::array *array_value = value->as_array();
      py::list array_v = toml_array_to_py_list(*array_value);
      result.append(array_v);
    } else if (value->type() == toml::node_type::date) {
      const toml::value<toml::date> *date_value = value->as_date();
      const toml::date date = date_value->get();
      result.append(date);
    } else if (value->type() == toml::node_type::time) {
      const toml::value<toml::time> *time_value = value->as_time();
      const toml::time time = time_value->get();
      result.append(time);
    } else if (value->type() == toml::node_type::date_time) {
      const toml::value<toml::date_time> *date_time_value =
          value->as_date_time();
      const toml::date_time datetime = date_time_value->get();
      result.append(datetime);
    } else {
      std::stringstream err_message;
      err_message
          << "cannot convert the type of this node to proper python types: "
          << value->type() << std::endl;
      std::string err_message_string = err_message.str();
      throw DecodeError(err_message_string);
    }
  }
  return result;
}

py::dict toml_table_to_py_dict(const toml::table &t) {
  py::dict result;
  for (auto it : t) {
    auto key = it.first.c_str();
    const toml::node *value = &(it.second);
    if (value->type() == toml::node_type::integer) {
      const toml::value<int64_t> *integer_value = value->as_integer();
      const int64_t integer_v = integer_value->get();
      result[key] = integer_v;
    } else if (value->type() == toml::node_type::floating_point) {
      const toml::value<double> *float_value = value->as_floating_point();
      const double float_v = float_value->get();
      result[key] = float_v;
    } else if (value->type() == toml::node_type::string) {
      const toml::value<std::string> *string_value = value->as_string();
      const std::string string_v = string_value->get();
      result[key] = string_v;
    } else if (value->type() == toml::node_type::boolean) {
      const toml::value<bool> *boolean_value = value->as_boolean();
      const bool bool_v = boolean_value->get();
      result[key] = bool_v;
    } else if (value->type() == toml::node_type::none) {
      result[key] = py::none();
    } else if (value->type() == toml::node_type::table) {
      const toml::table *table_value = value->as_table();
      py::dict d = toml_table_to_py_dict(*table_value);
      result[key] = d;
    } else if (value->type() == toml::node_type::array) {
      const toml::array *array_value = value->as_array();
      py::list array_v = toml_array_to_py_list(*array_value);
      result[key] = array_v;
    } else if (value->type() == toml::node_type::date) {
      const toml::value<toml::date> *date_value = value->as_date();
      const toml::date date = date_value->get();
      result[key] = date;
    } else if (value->type() == toml::node_type::time) {
      const toml::value<toml::time> *time_value = value->as_time();
      const toml::time time = time_value->get();
      result[key] = time;
    } else if (value->type() == toml::node_type::date_time) {
      const toml::value<toml::date_time> *date_time_value =
          value->as_date_time();
      const toml::date_time datetime = date_time_value->get();
      result[key] = datetime;
    } else {
      std::stringstream err_message;
      err_message
          << "cannot convert the type of this node to proper python types: "
          << value->type() << std::endl;
      std::string err_message_string = err_message.str();
      throw DecodeError(err_message_string);
    }
  }
  return result;
}

toml::array py_list_to_toml_array(const py::list &list);
toml::table py_dict_to_toml_table(const py::dict &object);

toml::array py_list_to_toml_array(const py::list &list) {
  toml::array arr;

  auto PY_DATETIME_MODULE = py::module::import("datetime");
  auto date_class = PY_DATETIME_MODULE.attr("date");
  auto time_class = PY_DATETIME_MODULE.attr("time");
  auto datetime_class = PY_DATETIME_MODULE.attr("datetime");

  for (auto it : list) {
    if (py::isinstance<py::str>(it)) {
      std::string string_value = it.cast<py::str>();
      arr.push_back(string_value);
    } else if (py::isinstance<py::int_>(it)) {
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
      toml::date_time date_time_value = it.cast<toml::date_time>();
      arr.push_back(date_time_value);
    } else if (py::isinstance(it, date_class)) {
      toml::date date_value = it.cast<toml::date>();
      arr.push_back(date_value);
    } else if (py::isinstance(it, time_class)) {
      toml::time time_value = it.cast<toml::time>();
      arr.push_back(time_value);
    } else {
      std::stringstream ss;
      ss << "not a valid type for converstion " << it << std::endl;
      throw py::type_error(ss.str());
    }
  }
  return arr;
}

toml::table py_dict_to_toml_table(const py::dict &object) {
  toml::table t;

  auto PY_DATETIME_MODULE = py::module::import("datetime");
  auto date_class = PY_DATETIME_MODULE.attr("date");
  auto time_class = PY_DATETIME_MODULE.attr("time");
  auto datetime_class = PY_DATETIME_MODULE.attr("datetime");

  for (auto it : object) {
    auto key = it.first;
    auto value = it.second;
    if (!py::isinstance<py::str>(key)) {
      throw py::type_error("key must be a string...");
    } else {
      std::string key_string = std::string(py::str(key));
      bool insert_ok = true;
      if (py::isinstance<py::int_>(value)) {
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
        toml::date_time date_time_value = value.cast<toml::date_time>();
        auto insert = t.insert_or_assign(key_string, date_time_value);
        insert_ok = insert.second;
      } else if (py::isinstance(value, date_class)) {
        toml::date date_value = value.cast<toml::date>();
        auto insert = t.insert_or_assign(key_string, date_value);
        insert_ok = insert.second;
      } else if (py::isinstance(value, time_class)) {
        toml::time time_value = value.cast<toml::time>();
        auto insert = t.insert_or_assign(key_string, time_value);
        insert_ok = insert.second;
      } else {
        std::stringstream ss;
        ss << "cannot convert value " << value << " to proper toml type"
           << std::endl;
        throw py::type_error(ss.str());
      }
      if (!insert_ok) {
        std::stringstream ss;
        ss << "cannot insert key value pair:" << key << "," << value
           << std::endl;
        throw py::type_error(ss.str());
      }
    }
  }
  return t;
}

} // namespace pytomlpp