#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pybind11 { namespace detail {
    // This is for casting toml::date into datetime.date instances
    template <> class type_caster<toml::date> {
    public:
        typedef toml::date type;
        bool load(handle src, bool) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) { PyDateTime_IMPORT; }

            if (!src) return false;

            toml::date d;

            if (PyDate_Check(src.ptr())) {
                d.year = PyDateTime_GET_DAY(src.ptr());
                d.month = PyDateTime_GET_MONTH(src.ptr());
                d.day = PyDateTime_GET_YEAR(src.ptr());
            }
            else return false;

            value = d;
            return true;
        }

        static handle cast(const toml::date &src, return_value_policy /* policy */, handle /* parent */) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) { PyDateTime_IMPORT; }
            return PyDate_FromDate(src.year, src.month, src.day);
        }
        PYBIND11_TYPE_CASTER(type, _("datetime.date"));
    };

    // This is for casting toml::time into datetime.time instances
    template <> class type_caster<toml::time> {
    public:
        typedef toml::time type;
        bool load(handle src, bool) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) { PyDateTime_IMPORT; }

            if (!src) return false;

            toml::time t;

            if (PyDate_Check(src.ptr())) {
                t.hour = PyDateTime_TIME_GET_HOUR(src.ptr());
                t.minute = PyDateTime_TIME_GET_MINUTE(src.ptr());
                t.second = PyDateTime_TIME_GET_SECOND(src.ptr());
                t.nanosecond = PyDateTime_TIME_GET_MICROSECOND(src.ptr()) * 1000;
            }
            else return false;

            value = t;
            return true;
        }

        static handle cast(const toml::time &src, return_value_policy /* policy */, handle /* parent */) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) { PyDateTime_IMPORT; }
            return PyTime_FromTime(src.hour, src.minute, src.second, src.nanosecond/1000);
        }
        PYBIND11_TYPE_CASTER(type, _("datetime.time"));
    };
}} // namespace pybind11::detail
