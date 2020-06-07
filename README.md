# pytomlpp

[![Build Status](https://travis-ci.com/bobfang1992/pytomlpp.svg?branch=master)](https://travis-ci.com/bobfang1992/pytomlpp)


This is an unofficial python wrapper for tomlplusplus (https://marzer.github.io/tomlplusplus/).

Some points you may want to know before use:

* The binding is usind [pybind11](https://github.com/pybind/pybind11)
* The project is tested using [toml-test](https://github.com/BurntSushi/toml-test), test are written in pytest and googletest.
* We convert toml structure to native python data structures when parsing, this is more inline with what `json`.

Some feature not supported yet (but I am working on...):

* `dump` and `dumps` is still in the making.

