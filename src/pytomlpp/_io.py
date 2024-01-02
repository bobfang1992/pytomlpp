"""Python wrapper for Toml++ IO methods."""

import os
from typing import Any, BinaryIO, Dict, TextIO, Union, Optional

from . import _impl

FilePathOrObject = Union[str, TextIO, BinaryIO, os.PathLike]


def dumps(data: Dict[Any, Any]) -> str:
    """Serialise data to TOML string.

    Args:
        data (Dict[Any, Any]): input data

    Returns:
        str: seralised data
    """
    return _impl.dumps(data)


def dump(data: Dict[Any, Any], fl: FilePathOrObject, mode: str = "w", encoding: Optional[str] = None) -> None:
    """Serialise data to TOML file

    Args:
        data (Dict[Any, Any]): input data
        fl (FilePathOrObject): file like object or path
        mode (str, optional): mode to write the file, support "w", "wt" (text) or "wb" (binary). Defaults to "w".
        encoding (str): defaults to None. If None, local enconding will be selected. 
        NOTE: ``If mode is binary mode, encoding optional argument will be negligible.``
    """
    data = _impl.dumps(data)
    if mode == "wb":
        data = data.encode("utf-8")
    if hasattr(fl, "write"):
        fl.write(data)
        return
    with open(fl, mode=mode, encoding=encoding) as fh:
       fh.write(data)


def loads(data: str) -> Dict[Any, Any]:
    """Deserialise from TOML string to python dict.

    Args:
        data (str): TOML string

    Returns:
        Dict[Any, Any]: deserialised data
    """
    return _impl.loads(data)


def load(fl: FilePathOrObject, mode: str = "r", encoding: Optional[str] = None) -> Dict[Any, Any]:
    """Deserialise from TOML file to python dict.

    Args:
        fl (FilePathOrObject): file like object or path
        mode (str, optional): mode to read the file, support "r", "rt" (text) or "rb" (binary). Defaults to "r".
        encoding (str): defaults to None. If None, local enconding will be selected. 
        NOTE: ``If mode is binary mode, encoding optional argument will be negligible.``
          
    Returns:
        Dict[Any, Any]: deserialised data
    """
    if hasattr(fl, "read"):
        data = fl.read()
    else:
        with open(fl, mode=mode, encoding=encoding) as fh:
            data = fh.read()
    if isinstance(data, bytes):
        return _impl.loads(data.decode("utf-8"))
    return _impl.loads(data)
