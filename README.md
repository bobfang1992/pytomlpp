# pytomlpp

[![Build Status](https://github.com/bobfang1992/pytomlpp/workflows/Wheels/badge.svg)](https://github.com/bobfang1992/pytomlpp/actions)
[![Conda Status](https://anaconda.org/conda-forge/pytomlpp/badges/version.svg)](https://anaconda.org/conda-forge/pytomlpp)
[![PyPI version](https://badge.fury.io/py/pytomlpp.svg)](https://badge.fury.io/py/pytomlpp)

This is an python wrapper for `toml++` (https://marzer.github.io/tomlplusplus/).

Some points you may want to know before use:
* Using `toml++` means that this module is fully compatible with TOML [v1.0.0](https://toml.io/en/v1.0.0). 
* We convert toml structure to native python data structures (dict/list etc.) when parsing, this is more inline with what `json` module does.
* The binding is using [pybind11](https://github.com/pybind/pybind11).
* The project is tested using [toml-test](https://github.com/BurntSushi/toml-test) and [pytest](https://github.com/pytest-dev/pytest).
* We support all major platforms (Linux, Mac OSX and Windows), for both CPython and Pypy and all recent Python versions. You just need to `pip install` and we have a pre-compiled binaries ready. No need to play with `clang`, `cmake` or any C++ toolchains.

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
  pytomlpp:   0.914 s
     rtoml:   1.148 s ( 1.25x)
     tomli:   4.850 s ( 5.30x)
     qtoml:  11.882 s (12.99x)
   tomlkit:  72.140 s (78.89x)
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

```
git clone git@github.com:bobfang1992/pytomlpp.git --recurse-submodules=third_party/tomlplusplus --shallow-submodules
cd pytomlpp
pip install .
```

---

![Alt](https://repobeats.axiom.co/api/embed/e767bf2c29b32ec317fc591d4cafda263de6c4b0.svg "Repobeats analytics image")
