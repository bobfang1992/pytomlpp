"""Python wrapper for Toml++ IO methods."""

from . import _impl


def dump(data, fl, mode="w", encoding="utf-8"):
    """Serialise data to TOML.

    Args:
        data: data to serialise
        fl (io.TextIOBase, str or os.Pathlike):
            file-object to write to (supports ``write``)
            or a file name (str, os.Pathlike) that supports ``open``
        mode (str): opening mode. Either "w" or "wt" (text), or "wb" (binary). Defaults to "w".
            Ignored if fl supports ``write``.
        encoding (str): encoding for binary data. Defaults to "utf-8"
    """
    data = _impl.dumps(data)
    if mode == "wb":
        data = data.encode(encoding)
    if hasattr(fl, "write"):
        fl.write(data)
        return
    with open(fl, mode=mode) as fh:
        fh.write(data)



def load(fl, mode="r", encoding="utf-8"):
    """Deserialise from TOML.

    Args:
        fl (io.TextIOBase, str or os.Pathlike):
            file-object to read from (supports ``read``)
            or a file name (str, os.Pathlike) that supports ``open``
        mode (str):  opening mode. Either "r" or "rt" (text) or "rb" (binary). Defaults to "r".
            Ignored if fl supports ``read``.
        encoding (str): encoding for binary data. Defaults to "utf-8"

    Returns:
        deserialised data
    """

    if hasattr(fl, "read"):
        data = fl.read()
    else:
        with open(fl, mode=mode) as fh:
            data = fh.read()
    if isinstance(data, bytes):
        return _impl.loads(data.decode(encoding))
    return _impl.loads(data)
