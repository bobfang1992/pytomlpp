#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pybind11 { namespace detail {
    // This is for casting toml::date into datetime.datetime instances
    template <> class type_caster<toml::date> {
    public:
        typedef toml::date type;
        bool load(handle src, bool) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) { PyDateTime_IMPORT; }

            if (!src) return false;

            toml::date d;

            if (PyDate_Check(src.ptr())) {
                int year = src.attr("year").cast<py::int_>();
                int month = src.attr("month").cast<py::int_>();
                int day = src.attr("day").cast<py::int_>();

                d.year = year;
                d.month = month;
                d.day = day;
            }
            else return false;

            value = d;
            return true;
        }

        static handle cast(const toml::date &date, return_value_policy /* policy */, handle /* parent */) {
            auto PY_DATETIME_MODULE = py::module::import("datetime");
            py::object py_date =
                PY_DATETIME_MODULE.attr("date")(date.year, date.month, date.day);
            return py_date;
        }
        PYBIND11_TYPE_CASTER(type, _("datetime.datetime"));
    };
}} // namespace pybind11::detail
