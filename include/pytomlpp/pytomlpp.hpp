#pragma once

// toml++ config
#define TOML_OPTIONAL_TYPE tl::optional
#define TOML_WINDOWS_COMPAT 0
#define TOML_LARGE_FILES 1
#define TOML_HEADER_ONLY 0
#define TOML_UNDEF_MACROS 0 // leaves some toml++'s macros for us to use

// pytomlpp config
#ifndef PYTOMLPP_PROFILING
#define PYTOMLPP_PROFILING 0 // see a profiling summary on shutdown
#endif

// common includes
#include <tomlplusplus/include/toml++/toml_preprocessor.h>
TOML_DISABLE_WARNINGS
#include <optional.hpp>
#include <pybind11/pybind11.h>
#include <sstream>
#include <tomlplusplus/include/toml++/toml.h>
TOML_ENABLE_WARNINGS

// namespace and type forward declarations
namespace py = pybind11;
namespace pytomlpp {
using namespace pybind11::literals;

[[nodiscard]] py::list toml_array_to_py_list(toml::array &&);
[[nodiscard]] py::dict toml_table_to_py_dict(toml::table &&);
[[nodiscard]] toml::table py_dict_to_toml_table(const py::dict &);
[[nodiscard]] toml::array py_list_to_toml_array(const py::list &);

struct DecodeError : public std::exception {
  std::string err_message;
  int start_line;
  int start_col;
  int end_line;
  int end_col;
  std::shared_ptr<const std::string> path;

  DecodeError(const std::string &message) noexcept : err_message(message) {}

  DecodeError(const std::string &message, int start_line, int start_col,
              int end_line, int end_col,
              const std::shared_ptr<const std::string> &path) noexcept
      : err_message(message), start_line(start_line), start_col(start_col),
        end_line(end_line), end_col(end_col), path(path) {}

  const char *what() const noexcept override { return err_message.c_str(); }
};
} // namespace pytomlpp

namespace pybind11::detail {
template <> class type_caster<toml::date> {
public:
  using type = toml::date;
  bool load(handle, bool);
  static handle cast(const type &, return_value_policy /* policy */,
                     handle /* parent */);
  PYBIND11_TYPE_CASTER(type, _("datetime.date"));
};

template <> class type_caster<toml::time> {
public:
  using type = toml::time;
  bool load(handle, bool);
  static handle cast(const type &, return_value_policy /* policy */,
                     handle /* parent */);
  PYBIND11_TYPE_CASTER(type, _("datetime.time"));
};

template <> class type_caster<toml::date_time> {
public:
  using type = toml::date_time;
  bool load(handle, bool);
  static handle cast(const type &src, return_value_policy /* policy */,
                     handle /* parent */);
  PYBIND11_TYPE_CASTER(type, _("datetime.datetime"));
};
} // namespace pybind11::detail
