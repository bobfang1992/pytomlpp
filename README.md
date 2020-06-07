# pytomlpp

[![Build Status](https://travis-ci.com/bobfang1992/pytomlpp.svg?branch=master)](https://travis-ci.com/bobfang1992/pytomlpp)


This is an unofficial python wrapper for tomlplusplus (https://marzer.github.io/tomlplusplus/).

Some points you may want to know before use:
* Using `tomlplusplus` means that this module is fully compatible with TOML v1.0.0. 
* We convert toml structure to native python data structures when parsing, this is more inline with what `json`.
* The binding is usind [pybind11](https://github.com/pybind/pybind11)
* The project is tested using [toml-test](https://github.com/BurntSushi/toml-test), test are written in pytest and googletest.

Some feature not supported yet (but I am working on...):

* `dump` and `dumps` is still in the making.

# Example
```
In [1]: import pytomlpp                                                                                                                                                                                                                                                                            

In [2]: toml_string = 'hello = "世界"'                                                                                                                                                                                                                                                             

In [3]: pytomlpp.loads(toml_string)                                                                                                                                                                                                                                                                
Out[3]: {'hello': '世界'}

In [4]: type(_)                                                                                                                                                                                                                                                                                    
Out[4]: dict
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

In [4]: %timeit run_parser(toml.load)                                                                                                                                                                                                                                                              
3.27 s ± 151 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)

In [5]: %timeit run_parser(pytomlpp.load)                                                                                                                                                                                                                                                          
307 ms ± 23.1 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)
```