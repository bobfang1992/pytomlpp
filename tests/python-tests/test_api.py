from __future__ import print_function

import pytest

import json

try:
    import pathlib
except ImportError:
    import pathlib2 as pathlib

from dateutil import parser as dateutil_parser

import pytomlpp

TOML_0_4_SPECIFIC = [
    'array-mixed-types-arrays-and-ints',
    'array-mixed-types-ints-and-floats',
    'array-mixed-types-strings-and-ints',
    'key-single-open-bracket',
]

_toml_dir = pathlib.Path(__file__).parent.parent / "toml-test" / "tests"
_marks = {
    s: (pytest.mark.skip(reason="TOML spec v0.4 specific test"),)
    for s in TOML_0_4_SPECIFIC
}
valid_toml_files = [
    pytest.param(p, id=p.stem)
    for p in (_toml_dir / "valid").glob("*.toml")
]
invalid_toml_files = [
    pytest.param(p, id=p.stem, marks=_marks.get(p.stem, ()))
    for p in (_toml_dir / "invalid").glob("*.toml")
]
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


@pytest.mark.parametrize("toml_file", valid_toml_files)
def test_valid_toml_files(toml_file):
    with open(str(toml_file), "r") as f:
        toml_file_string = f.read()
        table = pytomlpp.loads(toml_file_string)
        table_json = json.loads(toml_file.with_suffix(".json").read_text())
        assert_matches_json(table, table_json)


@pytest.mark.parametrize("toml_file", invalid_toml_files)
def test_invalid_toml_files(toml_file):
    with pytest.raises(pytomlpp.DecodeError):
        with open(str(toml_file), "r") as f:
            toml_file_string = f.read()
            pytomlpp.loads(toml_file_string)


@pytest.mark.parametrize("toml_file", valid_toml_files)
def test_round_trip_for_valid_toml_files(toml_file):
    with open(str(toml_file), "r") as f:
        text = f.read()
    print(text.strip(), end="\n\n")
    table = pytomlpp.loads(text)
    print(table, end="\n\n")

    text2 = pytomlpp.dumps(table)
    print(text2, end="\n\n")
    table2 = pytomlpp.loads(text2)
    assert table == table2


def test_invalid_encode():
    class A:
        pass
    with pytest.raises(TypeError):
        pytomlpp.dumps({'a': A()})
