#include <chrono>

#include <pytomlpp/pytomlpp.hpp>

namespace {
bool ENABLE_PROFILING = false;

void enable_profiling() { ENABLE_PROFILING = true; }

void disable_profiling() { ENABLE_PROFILING = false; }

bool profiling_status() { return ENABLE_PROFILING; }

struct profiling_stats_entry {
  long long counter;
  double total_time_in_ns;

  std::string get_summary() {
    std::stringstream ss;
    ss << "(counter = " << counter
       << ", total_time_in_ns = " << total_time_in_ns
       << ", average_time_in_ns = " << (total_time_in_ns / counter) << ")";
    return ss.str();
  }
};

std::unordered_map<std::string, profiling_stats_entry> profiling_stats;

void clear_profiling_stats() { profiling_stats.clear(); }

std::string get_profiling_stats_summary() {
  if (!profiling_stats.empty()) {
    std::stringstream ss;
    ss << "Summary of Profiling:\n";
    for (auto event : profiling_stats) {
      ss << event.first << " : " << event.second.get_summary() << "\n";
    }
    ss << "\n";
    return ss.str();
  } else {
    return "profiling not enabled or no profiling stats has been collected "
           "yet...";
  }
}

class profiling_guard {
  std::string event;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;

public:
  profiling_guard(const std::string &event) : event(event) {
    if (profiling_status()) {
      start = std::chrono::high_resolution_clock::now();
    }
  }

  ~profiling_guard() {
    if (profiling_status()) {
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::nano> duration = end - start;
      double execution_time_in_ns = duration.count();
      profiling_stats_entry &event_entry = profiling_stats[event];
      event_entry.counter += 1;
      event_entry.total_time_in_ns += execution_time_in_ns;
    }
  }
};
} // namespace

std::string TPP_VERSION = std::to_string(TOML_LIB_MAJOR) + "." +
                          std::to_string(TOML_LIB_MINOR) + "." +
                          std::to_string(TOML_LIB_PATCH);

py::dict loads(std::string_view toml_stirng) {
  try {
    profiling_guard guard_total("loads.total");
    auto tbl = toml::parse(toml_stirng);
    py::dict d;
    {
      profiling_guard guard_convert("loads.convert");
      d = std::move(pytomlpp::toml_table_to_py_dict(tbl));
    }
    return d;
  } catch (const toml::parse_error &e) {
    std::stringstream ss;
    ss << e;
    auto source_region = e.source();
    auto s_begin = source_region.begin;
    auto s_end = source_region.end;
    auto path = source_region.path;
    throw pytomlpp::DecodeError(ss.str(), static_cast<int>(s_begin.line),
                                static_cast<int>(s_begin.column),
                                static_cast<int>(s_end.line),
                                static_cast<int>(s_end.column), path);
  }
}

std::string dumps(py::dict object) {
  try {
    profiling_guard guard_total("dumps.total");
    toml::table t;
    {
      profiling_guard guard_convert("dumps.convert");
      t = std::move(pytomlpp::py_dict_to_toml_table(object));
    }
    std::stringstream ss;
    ss << t;
    return ss.str();
  } catch (const std::runtime_error &e) {
    throw py::type_error(e.what());
  }
}

PYBIND11_MODULE(_impl, m) {
  m.doc() = "tomlplusplus python wrapper";
  m.attr("lib_version") = TPP_VERSION;
  m.def("loads", &loads);
  m.def("dumps", &dumps);

  m.def("__enable_profiling__", &enable_profiling);
  m.def("__disable_profiling__", &disable_profiling);
  m.def("__profiling_status__", &profiling_status);
  m.def("__clear_profiling_stats__", &clear_profiling_stats);
  m.def("__get_profiling_stats_summary__", &get_profiling_stats_summary);

  py::register_exception<pytomlpp::DecodeError>(m, "DecodeError");
}
