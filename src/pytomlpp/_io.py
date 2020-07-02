"""Python wrapper for Toml++ IO methods."""

from . import _impl


def dump(data, fl):
    """Serialise data to TOML.

    Args:
        data: data to serialise
        fl (io.TextIOBase): file-object to write to (supports ``write``)
    """

    fl.write(_impl.dumps(data))


def load(fl):
    """Deserialise from TOML.

    Args:
        fl (io.TextIOBase): file-object to read from (supports ``read``)

    Returns:
        deserialised data
    """

    return _impl.loads(fl.read())
