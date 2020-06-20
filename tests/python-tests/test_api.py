from __future__ import print_function

import pytest

import json

try:
    import pathlib
except ImportError:
    import pathlib2 as pathlib

from dateutil import parser as dateutil_parser

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

_toml_dir = pathlib.Path(__file__).parent.parent / "toml-test" / "tests"
valid_toml_files = list((_toml_dir / "valid").glob("*.toml"))
invalid_toml_files = list((_toml_dir / "invalid").glob("*.toml"))
if len(valid_toml_files) <= 0 or len(invalid_toml_files) <= 0:
    raise Exception("toml-test files are not present")


def test_keys():
    toml_string = "a = 3"
    table = pytomlpp.loads(toml_string)
    keys = table.keys()
    assert len(keys) == 1
    assert list(keys)[0] == "a"


def assert_matches_json(yaml_obj, json_obj):
    if (
        isinstance(json_obj, dict)
        and set(json_obj.keys()) == {"type", "value"}
    ):
        if json_obj["type"] == "integer":
            assert yaml_obj == int(json_obj["value"])
        elif json_obj["type"] == "float":
            assert yaml_obj == float(json_obj["value"])
        elif json_obj["type"] == "bool":
            exp = {"true": True, "false": False}[json_obj["value"]]
            assert yaml_obj == exp
        elif json_obj["type"] == "string":
            assert yaml_obj == str(json_obj["value"])
        elif json_obj["type"] == "datetime":
            assert yaml_obj == dateutil_parser.isoparse(json_obj["value"])
        elif json_obj["type"] == "array":
            assert len(yaml_obj) == len(json_obj["value"])
            for yaml_item, json_item in zip(yaml_obj, json_obj["value"]):
                assert_matches_json(yaml_item, json_item)
        else:
            raise ValueError(json_obj)
    elif isinstance(json_obj, dict):
        assert isinstance(yaml_obj, dict)
        for key in json_obj:
            assert_matches_json(yaml_obj[key], json_obj[key])
    elif isinstance(json_obj, list):
        assert isinstance(yaml_obj, list)
        assert len(yaml_obj) == len(json_obj)
        for yaml_item, json_item in zip(yaml_obj, json_obj):
            assert_matches_json(yaml_item, json_item)
    else:
        assert yaml_obj == json_obj


@pytest.mark.parametrize(
    "toml_file", [pytest.param(p, id=p.stem) for p in valid_toml_files]
)
def test_valid_toml_files(toml_file):
    if toml_file.stem in VALID_EXCLUDE_FILE:
        pytest.skip()
    with open(toml_file, "r") as f:
        toml_file_string = f.read()
        table = pytomlpp.loads(toml_file_string)
        table_json = json.loads(toml_file.with_suffix(".json").read_text())
        assert_matches_json(table, table_json)


@pytest.mark.parametrize(
    "toml_file", [pytest.param(p, id=p.stem) for p in invalid_toml_files]
)
def test_invalid_toml_files(toml_file):
    if toml_file.stem in INVALID_EXCLUDE_FILE:
        pytest.skip()
    with pytest.raises(RuntimeError):
        with open(toml_file, "r") as f:
            toml_file_string = f.read()
            pytomlpp.loads(toml_file_string)
