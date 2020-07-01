"""Python wrapper for Toml++."""

__all__ = ["DecodeError", "dumps", "loads", "dump", "load"]

from ._impl import DecodeError
from ._impl import dumps
from ._impl import loads
from ._io import dump
from ._io import load
