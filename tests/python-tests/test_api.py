from __future__ import print_function

import pytest

import json

try:
    import pathlib
except ImportError:
    import pathlib2 as pathlib

from dateutil import parser as dateutil_parser

import pytomlpp

TOML_TESTS_SKIP = [
    'string-escapes'
]

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
_valid_marks = {
    s: (pytest.mark.skip(reason="Skipping for these tests for now"))
    for s in TOML_TESTS_SKIP
}
valid_toml_files = [
    pytest.param(p, id=p.stem, marks=_valid_marks.get(p.stem, ()))
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


def value_from_json(json_obj):
    if (
        isinstance(json_obj, dict)
        and set(json_obj.keys()) == {"type", "value"}
    ):
        if json_obj["type"] == "integer":
            return int(json_obj["value"])
        elif json_obj["type"] == "float":
            return float(json_obj["value"])
        elif json_obj["type"] == "bool":
            return {"true": True, "false": False}[json_obj["value"]]
        elif json_obj["type"] == "string":
            return str(json_obj["value"])
        elif json_obj["type"] == "datetime":
            return dateutil_parser.isoparse(json_obj["value"])
        elif json_obj["type"] == "array":
            return [value_from_json(item) for item in json_obj["value"]]
        else:
            raise ValueError(json_obj)
    elif isinstance(json_obj, dict):
        return {key: value_from_json(value) for key, value in json_obj.items()}
    elif isinstance(json_obj, list):
        return [value_from_json(item) for item in json_obj]
    else:
        return json_obj


@pytest.mark.parametrize("toml_file", valid_toml_files)
def test_loads_valid_toml_files(toml_file):
    with open(str(toml_file), "r") as f:
        table = pytomlpp.load(f)
        table_json = json.loads(toml_file.with_suffix(".json").read_text())
        table_expected = value_from_json(table_json)
        assert table == table_expected
    table = pytomlpp.load(toml_file)
    assert table == table_expected

@pytest.mark.parametrize("toml_file", invalid_toml_files)
def test_loads_invalid_toml_files(toml_file):
    with pytest.raises(pytomlpp.DecodeError):
        with open(str(toml_file), "r") as f:
            pytomlpp.load(f)
    with pytest.raises(pytomlpp.DecodeError):
        pytomlpp.load(str(toml_file))

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
    with pytest.raises(TypeError):
        pytomlpp.dumps({'a': [A()]})

@pytest.mark.parametrize("toml_file", valid_toml_files)
def test_decode_encode_binary(toml_file, tmp_path):
    data = pytomlpp.load(toml_file)
    pytomlpp.dump(data, str(tmp_path / "tmp.toml"), mode="wb")
    assert pytomlpp.load(str(tmp_path / "tmp.toml"), mode="rb") == data
