# pytomlpp

[![Build Status](https://github.com/bobfang1992/pytomlpp/workflows/Wheels/badge.svg)](https://github.com/bobfang1992/pytomlpp/actions)
[![Conda Status](https://anaconda.org/conda-forge/pytomlpp/badges/version.svg)](https://anaconda.org/conda-forge/pytomlpp)
[![PyPI version](https://badge.fury.io/py/pytomlpp.svg)](https://badge.fury.io/py/pytomlpp)

This is an python wrapper for `toml++` (https://marzer.github.io/tomlplusplus/).

Some points you may want to know before use:

- Using `toml++` means that this module is fully compatible with TOML [v1.0.0](https://toml.io/en/v1.0.0).
- We convert toml structure to native python data structures (dict/list etc.) when parsing, this is more inline with what `json` module does.
- The binding is using [pybind11](https://github.com/pybind/pybind11).
- The project is tested using [toml-test](https://github.com/BurntSushi/toml-test) and [pytest](https://github.com/pytest-dev/pytest).
- We support all major platforms (Linux, Mac OSX and Windows), for both CPython and Pypy and all recent Python versions. You just need to `pip install` and we have a pre-compiled binaries ready. No need to play with `clang`, `cmake` or any C++ toolchains.

# Example

```
In [1]: import pytomlpp

In [2]: toml_string = 'hello = "世界"'

In [3]: pytomlpp.loads(toml_string)
Out[3]: {'hello': '世界'}

In [4]: type(_)
Out[4]: dict

In [6]: pytomlpp.dumps({"你好": "world"})
Out[6]: '"你好" = "world"'
```

# Why bother?

There are some existing python TOML parsers on the market but from my experience they are implemented purely in python which is a bit slow.

```
Parsing data.toml 1000 times:
     rtoml:   0.540 s
  pytomlpp:   0.542 s ( 1.00x)
     tomli:   2.923 s ( 5.40x)
     qtoml:   8.748 s (16.18x)
   tomlkit:  51.608 s (95.49x)
      toml: Parsing failed. Likely not TOML 1.0.0-compliant.
```

Test it for yourself using [the benchmark script](benchmark/run.py).

# Installing

We recommend you to use `pip` to install this package:

```sh
pip install pytomlpp
```

You can also use `conda` to install this package, on all common platforms & python versions.
If you have an issue with a package from conda-forge, you can raise an issue on the [feedstock](https://github.com/conda-forge/pytomlpp-feedstock)

```sh
conda install -c conda-forge pytomlpp
```

You can also install from source:

```sh
git clone git@github.com:bobfang1992/pytomlpp.git --recurse-submodules=third_party/tomlplusplus --shallow-submodules
cd pytomlpp
pip install .
```

# Development

## Prerequisites

- Python 3.8 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git (for cloning submodules)

## Setting Up Development Environment

1. **Clone the repository with submodules:**

```sh
git clone git@github.com:bobfang1992/pytomlpp.git --recurse-submodules
cd pytomlpp
```

If you already cloned without submodules:

```sh
git submodule update --init --recursive
```

2. **Create a virtual environment (recommended):**

```sh
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

3. **Install in editable mode with development dependencies:**

```sh
# Install package in editable mode
pip install -e .

# Install test dependencies
pip install -r tests/requirements.txt
```

## Building Locally

### Using Modern Build Tools (Recommended)

```sh
# Install build tool
pip install build

# Build wheel and source distribution
python -m build

# Output will be in dist/
ls dist/
# pytomlpp-1.1.0-cp312-cp312-macosx_*.whl
# pytomlpp-1.1.0.tar.gz
```

### Using pip (Development)

```sh
# Install in editable mode (changes to Python code take effect immediately)
pip install -e .

# Rebuild C++ extension after changes
pip install -e . --force-reinstall --no-deps
```

### Using setup.py (Legacy)

```sh
# Build extension in-place
python setup.py build_ext --inplace

# Install
python setup.py install
```

## Running Tests

### Run All Tests

```sh
# From project root
pytest tests/

# With verbose output
pytest tests/ -v

# With coverage
pip install pytest-cov
pytest tests/ --cov=pytomlpp --cov-report=html
```

### Run Specific Tests

```sh
# Run only API tests
pytest tests/python-tests/test_api.py

# Run specific test function
pytest tests/python-tests/test_api.py::test_loads_valid_toml_files -v

# Run with keyword matching
pytest tests/ -k "encode"
```

### Test Against toml-test Suite

```sh
cd tests
./run.sh  # Runs the official TOML test suite
```

## Running Benchmarks

```sh
cd benchmark
pip install -r requirements.txt
python run.py
```

## Code Quality

### Format Code (if contributing)

```sh
# Install pre-commit hooks
pip install pre-commit
pre-commit install

# Run manually
pre-commit run --all-files
```

### Type Checking

```sh
pip install mypy
mypy src/pytomlpp/
```

## Project Structure

```
pytomlpp/
├── src/
│   ├── pytomlpp/          # Python package
│   │   ├── __init__.py
│   │   ├── _io.py
│   │   └── _impl.pyi      # Type stubs
│   ├── pytomlpp.cpp       # C++ bindings (main)
│   ├── type_casters.cpp   # Python/C++ type conversions
│   └── encoding_decoding.cpp  # TOML encoding/decoding
├── include/
│   └── pytomlpp/          # C++ headers
├── third_party/
│   └── tomlplusplus/      # toml++ submodule
├── tests/
│   └── python-tests/      # Python test suite
├── benchmark/             # Performance benchmarks
├── pyproject.toml         # Project metadata (PEP 621)
└── setup.py              # C++ extension build config
```

## Common Issues

### Submodule Not Initialized

```sh
git submodule update --init --recursive
```

### C++ Compilation Errors

Make sure you have a C++17 compatible compiler:

- **Linux**: GCC 7+ or Clang 5+
- **macOS**: Xcode 10+ (Clang 5+)
- **Windows**: Visual Studio 2017 or later

### Import Errors After Changes

Reinstall in editable mode:

```sh
pip install -e . --force-reinstall --no-deps
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Run tests (`pytest tests/`)
5. Commit your changes (`git commit -m 'Add amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines (if available).

## Getting Help

- **Issues**: [GitHub Issues](https://github.com/bobfang1992/pytomlpp/issues)
- **Discussions**: [GitHub Discussions](https://github.com/bobfang1992/pytomlpp/discussions)

---

![Alt](https://repobeats.axiom.co/api/embed/e767bf2c29b32ec317fc591d4cafda263de6c4b0.svg "Repobeats analytics image")
