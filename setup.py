from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "DynamicLib",
        ["DynamicLibBindings.cpp", "DynamicLib.cpp"],
        define_macros=[("VERSION_INFO", "0.1")],
        extra_compile_args=["/openmp"],  # Enable OpenMP for Windows
    ),
]

setup(
    name="DynamicLib",
    version="0.1",
    author="OverrLoss",
    description="Dynamic library with parallel computation using OpenMP.",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)