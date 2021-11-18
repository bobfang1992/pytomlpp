"""Python wrapper for Toml++."""

__all__ = ["DecodeError", "dumps", "loads", "dump", "load"]

from ._impl import DecodeError, lib_version
from ._io import dump, dumps, load, loads
