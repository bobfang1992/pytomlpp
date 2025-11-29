# Python Packaging Migration

## Modern Python Packaging (v1.1.0+)

This project has been migrated to modern Python packaging standards (PEP 517/518/621).

### File Structure

- **`pyproject.toml`** - Primary configuration file (PEP 621)
  - Contains all project metadata
  - Defines build system requirements
  - Uses SPDX license format
  - **This is the source of truth for metadata**

- **`setup.py`** - C++ extension build configuration only
  - Used by setuptools to compile the C++ extension
  - No metadata (all moved to pyproject.toml)
  - Still required for native extensions

- **`setup.cfg`** - DEPRECATED
  - Kept for backward compatibility only
  - All metadata moved to pyproject.toml
  - Will be removed in a future release

### Why This Migration?

1. **Standards Compliance**: PEP 517/518/621 are the modern Python packaging standards
2. **Tool Compatibility**: Better support for modern tools (pip, build, poetry, etc.)
3. **License Format**: Fixes setuptools deprecation warning for license classifiers
4. **Clarity**: Single source of truth for metadata (pyproject.toml)
5. **Future-Proof**: Aligns with Python packaging direction

### For Developers

#### Building from source

```bash
# Install build tool
pip install build

# Build wheel and sdist
python -m build

# Or use legacy method (still works)
pip install .
```

#### Editable Install

```bash
# Modern way
pip install -e .

# Still works with pyproject.toml
```

#### Version Updates

Update version in **one place**: `pyproject.toml`

```toml
[project]
version = "1.2.0"  # Update here
```

### Backward Compatibility

- `setup.cfg` is kept but deprecated
- `pip install .` still works
- `python setup.py install` still works (but discouraged)
- All existing workflows remain functional

### What Changed

**Before (v1.0.x):**
```
setup.py         - Metadata + Extension build
setup.cfg        - Additional metadata
```

**After (v1.1.0+):**
```
pyproject.toml   - All metadata (PEP 621)
setup.py         - Extension build only
setup.cfg        - DEPRECATED
```

### References

- [PEP 517 - Build System](https://peps.python.org/pep-0517/)
- [PEP 518 - Build Requirements](https://peps.python.org/pep-0518/)
- [PEP 621 - Project Metadata](https://peps.python.org/pep-0621/)
- [PEP 639 - License Expression](https://peps.python.org/pep-0639/)
- [Packaging Guide](https://packaging.python.org/en/latest/guides/writing-pyproject-toml/)
