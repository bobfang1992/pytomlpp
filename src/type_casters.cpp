#include <pytomlpp/pytomlpp.hpp>
#include <datetime.h>

namespace {
void lazy_init_py_date_time() noexcept {
  if (PyDateTimeAPI)
    return;

  PyDateTime_IMPORT;
}
} // namespace

namespace pybind11::detail {
// python date -> toml date
bool type_caster<toml::date>::load(handle src, bool) {
  if (!src)
    return false;

  lazy_init_py_date_time();
  toml::date d;

  if (PyDate_Check(src.ptr())) {
    d.year = PyDateTime_GET_YEAR(src.ptr());
    d.month = PyDateTime_GET_MONTH(src.ptr());
    d.day = PyDateTime_GET_DAY(src.ptr());
  } else
    return false;

  value = d;
  return true;
}

// toml date -> python date
handle type_caster<toml::date>::cast(const toml::date &src,
                                     return_value_policy /* policy */,
                                     handle /* parent */) {
  lazy_init_py_date_time();
  return PyDate_FromDate(src.year, src.month, src.day);
}

// python time -> toml time
bool type_caster<toml::time>::load(handle src, bool) {
  if (!src)
    return false;

  lazy_init_py_date_time();
  toml::time t;

  if (PyTime_Check(src.ptr())) {
    t.hour = PyDateTime_TIME_GET_HOUR(src.ptr());
    t.minute = PyDateTime_TIME_GET_MINUTE(src.ptr());
    t.second = PyDateTime_TIME_GET_SECOND(src.ptr());
    t.nanosecond = PyDateTime_TIME_GET_MICROSECOND(src.ptr()) * 1000;
  } else
    return false;

  value = t;
  return true;
}

// toml time -> python time
handle type_caster<toml::time>::cast(const toml::time &src,
                                     return_value_policy /* policy */,
                                     handle /* parent */) {
  lazy_init_py_date_time();
  return PyTime_FromTime(src.hour, src.minute, src.second,
                         src.nanosecond / 1000);
}

bool type_caster<toml::date_time>::load(handle src, bool) {
  if (!src)
    return false;

  lazy_init_py_date_time();
  toml::date_time dt;

  if (PyDateTime_Check(src.ptr())) {
    toml::date d;
    d.year = PyDateTime_GET_YEAR(src.ptr());
    d.month = PyDateTime_GET_MONTH(src.ptr());
    d.day = PyDateTime_GET_DAY(src.ptr());

    toml::time t;
    t.hour = PyDateTime_DATE_GET_HOUR(src.ptr());
    t.minute = PyDateTime_DATE_GET_MINUTE(src.ptr());
    t.second = PyDateTime_DATE_GET_SECOND(src.ptr());
    t.nanosecond = PyDateTime_DATE_GET_MICROSECOND(src.ptr()) * 1000;

    py::object tz_info = src.attr("tzinfo");

    if (!tz_info.is_none()) {
      toml::time_offset to;
      py::object time_delta = tz_info.attr("utcoffset")(src);
      py::object total_seconds_object = time_delta.attr("total_seconds")();
      int total_seconds = total_seconds_object.cast<py::int_>();
      int total_minutes = total_seconds / 60;
      to.minutes = total_minutes;
      dt.offset = to;
    }

    dt.date = d;
    dt.time = t;
  } else
    return false;

  value = dt;
  return true;
}

handle type_caster<toml::date_time>::cast(const toml::date_time &src,
                                          return_value_policy /* policy */,
                                          handle /* parent */) {
  lazy_init_py_date_time();
  auto datetime_module = py::module::import("datetime");
  py::object timezone_obj = py::none();

  if (src.offset) {
    auto minutes = src.offset.value().minutes;
    auto delta = datetime_module.attr("timedelta")("minutes"_a = minutes);
    timezone_obj = datetime_module.attr("timezone")(delta);
  }

  return PyDateTimeAPI->DateTime_FromDateAndTime(
      src.date.year, src.date.month, src.date.day, src.time.hour,
      src.time.minute, src.time.second, src.time.nanosecond / 1000,
      timezone_obj.ptr(), PyDateTimeAPI->DateTimeType);
}
} // namespace pybind11::detail
