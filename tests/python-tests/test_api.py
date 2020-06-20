from __future__ import print_function

import pytest
import glob
import os

try:
    import pathlib
except ImportError:
    import pathlib2 as pathlib

import pytomlpp

VALID_EXCLUDE_FILE = []
INVALID_EXCLUDE_FILE = [
    'array-mixed-types-arrays-and-ints',
    'array-mixed-types-ints-and-floats',
    'array-mixed-types-strings-and-ints',
    'float-leading-zero',
    'float-leading-zero-neg',
    'float-leading-zero-pos',
    'float-no-leading-zero',
    'float-no-trailing-digits',
    'key-single-open-bracket',
]

@pytest.fixture
def valid_toml_files():
    current_path = os.path.dirname(__file__)
    toml_files = glob.glob(current_path + "/../toml-test/tests/valid/*.toml")
    return [pathlib.Path(p) for p in toml_files]

@pytest.fixture
def invalid_toml_files():
    current_path = os.path.dirname(__file__)
    toml_files = glob.glob(current_path + "/../toml-test/tests/invalid/*.toml")
    return [pathlib.Path(p) for p in toml_files]
         

def test_keys():
    toml_string = "a = 3"
    table = pytomlpp.loads(toml_string)
    keys = table.keys()
    assert len(keys) == 1
    assert list(keys)[0] == "a"

def test_valid_toml_files(valid_toml_files):
    for t in valid_toml_files:
        if t.stem in VALID_EXCLUDE_FILE:
            continue
        print("parsing", t)
        table = pytomlpp.load(str(t))
        assert type(table) == dict

def test_invalid_toml_files(invalid_toml_files):
    for t in invalid_toml_files:
        if t.stem in INVALID_EXCLUDE_FILE:
            print("skiping", t.stem)
            continue
        print("parsing", t)
        with pytest.raises(RuntimeError):
            pytomlpp.load(str(t))
