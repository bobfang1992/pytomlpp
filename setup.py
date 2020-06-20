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
    name='pytomlpp',
    version='0.1.0',
    author='Bob Fang',
    author_email='bob.fang.london@gmail.com',
    description='A python wrapper for tomp++',
    long_description=open("README.md", encoding="utf-8").read(),
    long_description_content_type='text/markdown',
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Natural Language :: English",
        "Topic :: Utilities",
    ],
    keywords='toml parser serilization deserialization serdes',
    ext_modules=[
        Extension(
            'pytomlpp',
            ['src/pytomlpp.cpp'],
            include_dirs=[
                'include',
                'third_party',
                PyBind11Include(),
            ],
            extra_compile_args=extra_compile_args,
            language='c++',
        ),
    ],
    setup_requires=['pybind11~=2.5'],
    zip_safe=False,
)
