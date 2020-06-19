# pytomlpp

[![Build Status](https://travis-ci.com/bobfang1992/pytomlpp.svg?branch=master)](https://travis-ci.com/bobfang1992/pytomlpp)
[![Conda Status](https://anaconda.org/dorafmon/pytomlpp/badges/version.svg)](https://anaconda.org/dorafmon/pytomlpp)

This is an unofficial python wrapper for tomlplusplus (https://marzer.github.io/tomlplusplus/).

Some points you may want to know before use:
* Using `tomlplusplus` means that this module is fully compatible with TOML v1.0.0. 
* We convert toml structure to native python data structures (dict/list etc.) when parsing, this is more inline with what `json` module does.
* The binding is using [pybind11](https://github.com/pybind/pybind11).
* The project is tested using [toml-test](https://github.com/BurntSushi/toml-test), test are written in pytest and googletest.

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
There are some exisitng python TOML parser on the market but from my experience they are all purely implemented in python which is a bit slow. 

```
In [1]: import pytomlpp                                                                                                                                                                                                                                                                                                  

In [2]: import toml                                                                                                                                                                                                                                                                                                      

In [3]: def run_parser(parser_func): 
   ...:     for i in range(1000): 
   ...:         parser_func('Cargo.toml') 
   ...:                                                                                                                                                                                                                                                                                                                  

In [4]: %timeit run_parser(pytomlpp.load)                                                                                                                                                                                                                                                                                
310 ms ± 56.7 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)

In [5]: %timeit run_parser(toml.load)                                                                                                                                                                                                                                                                                    
3.5 s ± 162 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)

In [6]: pytomlpp.lib_version                                                                                                                              
Out[6]: '1.3.2'
```

# Installing

We recommand you to use `conda` to install this package:

```sh
conda install -c dorafmon pytomlpp
```

If you are not using `conda` then please install from source:

```
git clone git@github.com:bobfang1992/pytomlpp.git
cd pytomlpp
python setup.py install
```

## Why not pypi?
Pypi has some rules on how to distribute pre-compiled binary for different platforms. I do not have enough experties in this area. I would love to see contribution to make this package avaliable on pypi.
