#include <chrono>

#include <pytomlpp/pytomlpp.hpp>

namespace {
bool ENABLE_PROFILING = false;

void enable_profiling() { ENABLE_PROFILING = true; }

void disable_profiling() { ENABLE_PROFILING = false; }

bool profiling_status() { return ENABLE_PROFILING; }

struct profiling_stats_entry {
  long long counter;
  double total_time_in_ms;

  std::string get_summary() {
    std::stringstream ss;
    ss << "(counter = " << counter
       << ", total_time_in_ms = " << total_time_in_ms
       << ", average_time_in_ms = " << (total_time_in_ms / counter) << ")";
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
      std::chrono::duration<double, std::milli> duration = end - start;
      double execution_time_in_ms = duration.count();
      // std::cout << "execution_time = " << execution_time_in_ms << std::endl;
      profiling_stats_entry &event_entry = profiling_stats[event];
      event_entry.counter += 1;
      event_entry.total_time_in_ms += execution_time_in_ms;
    }
  }
};
} // namespace

std::string TPP_VERSION = std::to_string(TOML_LIB_MAJOR) + "." +
                          std::to_string(TOML_LIB_MINOR) + "." +
                          std::to_string(TOML_LIB_PATCH);

py::dict loads(std::string_view toml_stirng) {
  try {
    profiling_guard("loads.total");
    auto tbl = toml::parse(toml_stirng);
    profiling_guard("loads.convert");
    return pytomlpp::toml_table_to_py_dict(tbl);
  } catch (const std::runtime_error &e) {
    throw pytomlpp::DecodeError(e.what());
  }
}

std::string dumps(py::dict object) {
  try {
    profiling_guard("dumps.total");
    toml::table t;
    {
      profiling_guard("dumps.convert");
      t = pytomlpp::py_dict_to_toml_table(object);
    }
    std::stringstream ss;
    ss << t;
    return ss.str();
  } catch (const std::runtime_error &e) {
    throw py::type_error(e.what());
  }
}

PYBIND11_MODULE(pytomlpp, m) {
  m.doc() = "tomlplusplus python wrapper";
  m.attr("lib_version") = TPP_VERSION;
  m.def("loads", &loads);
  m.def("dumps", &dumps);

  m.def("enable_profiling", &enable_profiling);
  m.def("disable_profiling", &disable_profiling);
  m.def("profiling_status", &profiling_status);
  m.def("clear_profiling_stats", &clear_profiling_stats);
  m.def("get_profiling_stats_summary", &get_profiling_stats_summary);
  py::register_exception<pytomlpp::DecodeError>(m, "DecodeError");
}
