# meson-template

Template repository for C++ projects using meson.

# Features

* Testing using [UT: C++20 μ(micro)/Unit Testing Framework](https://github.com/boost-ext/ut)
* Documentation:
    * [Doxygen](https://www.doxygen.nl/) meson target: `doxygen`
    * [Sphinx](https://www.sphinx-doc.org/en/master/) meson target: `sphinx`
    * Bridge Doxygen and Sphinx using [Breathe](https://breathe.readthedocs.io/en/latest/)
    * [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) documentation (`docs/shpinx/documentation.rst`)
* Custom `.clang-format`

# Initializing meson

Command to generate release build dir

```
CC=gcc-11 CXX=g++-11 meson setup build --buildtype=release
```
