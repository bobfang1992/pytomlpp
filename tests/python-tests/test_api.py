import pytest

import pytomlpp


def test_keys():
    toml_string = "a = 3"
    table = pytomlpp.loads(toml_string)
    keys = table.keys()
    assert len(keys) == 1
    assert keys[0] == 'a'