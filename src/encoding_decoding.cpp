#include <pytomlpp/pytomlpp.hpp>
#include <pybind11/stl.h>

namespace pytomlpp {
py::list toml_array_to_py_list(toml::array &&a) {
  py::list result;
  for (auto &&val_ : std::move(a)) {
    std::move(val_).visit([&](auto &&val) {
      if constexpr (toml::is_table<decltype(val)>)
        result.append(toml_table_to_py_dict(std::move(val)));
      else if constexpr (toml::is_array<decltype(val)>)
        result.append(toml_array_to_py_list(std::move(val)));
      else
        result.append(std::move(*val));
    });
  }
  return result;
}

py::dict toml_table_to_py_dict(toml::table &&t) {
  py::dict result;
  for (auto &&[key_, val_] : std::move(t)) {
    std::move(val_).visit([&, key = key_.c_str()](auto &&val) {
      if constexpr (toml::is_table<decltype(val)>)
        result[key] = toml_table_to_py_dict(std::move(val));
      else if constexpr (toml::is_array<decltype(val)>)
        result[key] = toml_array_to_py_list(std::move(val));
      else
        result[key] = std::move(*val);
    });
  }
  return result;
}

toml::array py_list_to_toml_array(const py::list &list) {
  toml::array arr;

  auto PY_DATETIME_MODULE = py::module::import("datetime");
  auto date_class = PY_DATETIME_MODULE.attr("date");
  auto time_class = PY_DATETIME_MODULE.attr("time");
  auto datetime_class = PY_DATETIME_MODULE.attr("datetime");

  for (auto &&it : list) {
    if (py::isinstance<py::str>(it)) {
      std::string string_value = it.cast<py::str>();
      arr.push_back(std::move(string_value));
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
      throw py::type_error(py::str("not a valid type for conversion {}").format(it));
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

  for (auto &&it : object) {
    auto key = it.first;
    auto value = it.second;

    if (!py::isinstance<py::str>(key))
      throw py::type_error("key must be a string...");

    std::string key_string = std::string(py::str(key));
    bool insert_ok = true;
    if (py::isinstance<py::bool_>(value)) {
      bool bool_value = value.cast<py::bool_>();
      auto insert = t.insert_or_assign(key_string, bool_value);
      insert_ok = insert.second;
    } else if (py::isinstance<py::int_>(value)) {
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
      ss << "cannot insert key value pair:" << key << "," << value << std::endl;
      throw py::type_error(ss.str());
    }
  }
  return t;
}
} // namespace pytomlpp
