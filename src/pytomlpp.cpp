#define TOML_IMPLEMENTATION
#include <pytomlpp/pytomlpp.hpp>
#if PYTOMLPP_PROFILING
#include <chrono>
#include <iomanip>
#include <pybind11/iostream.h>
#endif // PYTOMLPP_PROFILING

PYTOMLPP_PUSH_OPTIMIZATIONS;

namespace {
#if PYTOMLPP_PROFILING
using namespace std::string_view_literals;

struct profiling_stats_entry {
  long long counter;
  double total_time_in_ns;
};

std::unordered_map<std::string_view, profiling_stats_entry> profiling_stats;

void print_profiling_stats() noexcept {
  py::print("\npytomlpp profiling summary:\n-----------------------------");
  if (profiling_stats.empty())
    py::print("no profiling stats have been collected.");
  else {
    std::ostringstream oss;
    for (const auto &[key, entry] : profiling_stats) {
      oss << std::setw(15) << key << ": "sv
          << " counter = "sv << std::setw(7) << entry.counter
          << ", total_time_in_ns = "sv << std::setw(12)
          << (long long)entry.total_time_in_ns << ", average_time_in_ns = "sv
          << std::setw(7) << (long long)(entry.total_time_in_ns / entry.counter)
          << "\n"sv;
    }
    py::print(oss.str());
  }
}

class profiling_guard {
  profiling_stats_entry &entry;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;

public:
  profiling_guard(profiling_stats_entry &entry) noexcept
      : entry{entry}, start{std::chrono::high_resolution_clock::now()} {}

  ~profiling_guard() noexcept {
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration = end - start;
    double execution_time_in_ns = duration.count();

    entry.counter += 1;
    entry.total_time_in_ns += execution_time_in_ns;
  }
};

#define PROFILE_SCOPE_2(line, name)                                            \
  static auto &profiling_stats_entry##line = profiling_stats[name];            \
  profiling_guard profiling_guard##line { profiling_stats_entry##line }
#define PROFILE_SCOPE_1(line, name) PROFILE_SCOPE_2(line, name)
#define PROFILE_SCOPE(name) PROFILE_SCOPE_1(__LINE__, name)
#else
#define PROFILE_SCOPE(name) (void)0
#endif // PYTOMLPP_PROFILING

std::string TPP_VERSION = std::to_string(TOML_LIB_MAJOR) + "." +
                          std::to_string(TOML_LIB_MINOR) + "." +
                          std::to_string(TOML_LIB_PATCH);

py::dict loads(std::string_view toml_string) {
  try {
    PROFILE_SCOPE("loads.total");
    toml::table tbl;
    {
      PROFILE_SCOPE("loads.parse");
      tbl = toml::parse(toml_string);
    }
    py::dict d;
    {
      PROFILE_SCOPE("loads.convert");
      d = pytomlpp::toml_table_to_py_dict(std::move(tbl));
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
    PROFILE_SCOPE("dumps.total");
    toml::table t;
    {
      PROFILE_SCOPE("dumps.convert");
      t = pytomlpp::py_dict_to_toml_table(object);
    }
    std::stringstream ss;
    ss << t;
    return ss.str();
  } catch (const std::runtime_error &e) {
    throw py::type_error(e.what());
  }
}

} // namespace

PYBIND11_MODULE(_impl, m) {
  m.doc() = "tomlplusplus python wrapper";
  m.attr("lib_version") = TPP_VERSION;
  m.def("loads", &loads);
  m.def("dumps", &dumps);

#if PYTOMLPP_PROFILING
  auto atexit = py::module::import("atexit");
  atexit.attr("register")(py::cpp_function(print_profiling_stats));
#endif

  py::register_exception<pytomlpp::DecodeError>(m, "DecodeError");
}
