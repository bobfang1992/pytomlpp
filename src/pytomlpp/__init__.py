"""

Python wrapper for Toml++.

Install from [PyPI](https://pypi.org/project/pytomlpp/)


"""

__all__ = ["DecodeError", "dumps", "loads", "dump", "load"]

from ._impl import DecodeError, lib_version
from ._io import dump, dumps, load, loads
