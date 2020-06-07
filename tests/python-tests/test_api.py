import pytest
import glob
import os
import pathlib

import pytomlpp

EXCLUDE_LIST = []

@pytest.fixture
def valid_toml_files():
    current_path = os.path.dirname(__file__)
    toml_files = glob.glob(current_path + "/../toml-test/tests/valid/*.toml")
    return [pathlib.Path(p) for p in toml_files]
         

def test_keys():
    toml_string = "a = 3"
    table = pytomlpp.loads(toml_string)
    keys = table.keys()
    assert len(keys) == 1
    assert list(keys)[0] == "a"

def test_valid_toml_files(valid_toml_files):
    for t in valid_toml_files:
        if t.stem in EXCLUDE_LIST:
            continue
        print(f"parsing {t}")
        table = pytomlpp.load(str(t))
        assert type(table) == dict
