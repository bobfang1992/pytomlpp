# pytomlpp

[![Build Status](https://github.com/bobfang1992/pytomlpp/workflows/Wheels/badge.svg)](https://github.com/bobfang1992/pytomlpp/actions)
[![PyPI version](https://badge.fury.io/py/pytomlpp.svg)](https://badge.fury.io/py/pytomlpp)

This is an python wrapper for `toml++` (https://marzer.github.io/tomlplusplus/).

Some points you may want to know before use:
* Using `toml++` means that this module is fully compatible with TOML [v1.0.0](https://toml.io/en/v1.0.0). 
* We convert toml structure to native python data structures (dict/list etc.) when parsing, this is more inline with what `json` module does.
* The binding is using [pybind11](https://github.com/pybind/pybind11).
* The project is tested using [toml-test](https://github.com/BurntSushi/toml-test) and [pytest](https://github.com/pytest-dev/pytest).

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
Parsing data.toml 5000 times:
  pytomlpp:    0.846 s
     tomli:    3.317 s (3.9x slower)
      toml:    5.697 s (6.7x slower)
     qtoml:    8.473 s (10.0x slower)
   tomlkit:   43.250 s (51.0x slower)
```
Test it for yourself using [the benchmark script](benchmark/run.py).

# Installing

We recommand you to use `pip` to install this package:
```sh
pip install pytomlpp
```

You can also install from source:

```
git clone git@github.com:bobfang1992/pytomlpp.git --recurse-submodules=third_party/tomlplusplus --shallow-submodules
cd pytomlpp
pip install .
```
