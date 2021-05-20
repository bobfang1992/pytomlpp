#!/usr/bin/env python3

import platform
from setuptools import setup, Extension


class PyBind11Include:
    def __str__(self):
        import pybind11

        return pybind11.get_include()


extra_compile_args = []
if platform.system() == "Windows":
    extra_compile_args.append('/std:c++17')
else:
    extra_compile_args.append('-std=c++17')


setup(
    ext_modules=[
        Extension(
            'pytomlpp._impl',
            ['src/pytomlpp.cpp', 'src/type_casters.cpp', 'src/encoding_decoding.cpp'],
            include_dirs=[
                'include',
                'third_party',
                PyBind11Include(),
            ],
            extra_compile_args=extra_compile_args,
            language='c++',
        ),
    ],
)
