#!/bin/sh

set -e

"/opt/python/${PYABI}/bin/pip" wheel --no-deps -w /tmp/wheels .

for whl in /tmp/wheels/*.whl; do
    auditwheel show "$whl"
    auditwheel repair --plat manylinux2010_x86_64 -w ${1} "$whl"
    rm -v "$whl"
done
