from setuptools import setup, Extension


class PyBind11Include:
    def __str__(self):
        import pybind11

        return pybind11.get_include()


setup(
    name='pytomlpp',
    version='0.1.0',
    author='Bob Fang',
    author_email='bob.fang.london@gmail.com',
    description='A python wrapper for tomp++',
    long_description='',
    ext_modules=[
        Extension(
            'pytomlpp',
            ['src/pytomlpp.cpp'],
            include_dirs=[
                'src',
                'third_party',
                PyBind11Include(),
            ],
            extra_compile_args=['-std=c++17'],
            language='c++',
        ),
    ],
    setup_requires=['pybind11~=2.5'],
    zip_safe=False,
)
