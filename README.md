# Fortran pathlib

[![DOI](https://zenodo.org/badge/433875623.svg)](https://zenodo.org/badge/latestdoi/433875623)
[![ci_cmake](https://github.com/scivision/fortran-pathlib/actions/workflows/ci_cmake.yml/badge.svg)](https://github.com/scivision/fortran-pathlib/actions/workflows/ci_cmake.yml)
[![intel-oneapi](https://github.com/scivision/fortran-pathlib/actions/workflows/intel-oneapi.yml/badge.svg)](https://github.com/scivision/fortran-pathlib/actions/workflows/intel-oneapi.yml)

Platform independent (Linux, macOS, Windows), object-oriented Fortran filesystem path manipulation library.
The C Runtime Library is used where native Fortran procedures do not exist.
Inspired by
[Python pathlib](https://docs.python.org/3/library/pathlib.html)
and
[C++ filesystem](https://en.cppreference.com/w/cpp/filesystem).

Currently tested with compilers:

* GCC Gfortran &ge; 8.5
* Intel oneAPI &ge; 2021

Would be happy to support additional Fortran 2018 compilers as available for testing.
For example, NAG, Cray, IBM XL, etc.

Fortran "pathlib" module contains one Fortran type "path_t" that contains properties and methods.
The "path_t" type uses getter and setter procedure to access the path as a string `character(:), allocatable`.

```fortran
use pathlib, only : path_t

type(path_t) :: p

p = path_t("my/path")  !< setter

print *, "path: ", p%path() !< getter
```

Due to compiler limitations, currently Fortran-pathlib only officially supports ASCII characters.
You may find that some features work on a particular computer with non-ASCII character, but this is not supported.

## Build

Can also use Pathlib in your CMake project via FetchContent or ExternalProject.

```sh
cmake -B build
cmake --build build
# optional
ctest --test-dir build
```

This creates build/libpathlib.a or similar.

Please see the [API docs](./API.md) for extensive list of functions/subroutines.

## Command line

For user convenience, we provide a demo executable "pathlib_cli" that allows simple testing of what the pathlib routines do.
To build the pathlib_cli utility:

```sh
cmake -B build -DBUILD_UTILS=on
cmake --build build
```

## Notes

A few topics on unsupported features:

### non-ASCII characters

The UCS
[selected_char_kind('ISO_10646')](https://gcc.gnu.org/onlinedocs/gfortran/SELECTED_005fCHAR_005fKIND.html),
is an *optional* feature of Fortran 2003 standard.
Intel oneAPI does not support `selected_char_kind('ISO_10646')` as of this writing.

pathlib currently uses the default Fortran `character` kind, which is ASCII.
This means that UTF-8 / UTF-16 / UTF-32 strings are not supported.
You may find a particular compiler and computer passes some non-ASCII strings, but this is not supported.

### Unsupported compilers

At this time, these compilers aren't supported for reasons including:

#### Nvidia HPC-SDK

nvfortran 22.1 does not support `character(:), allocatable` from Fortran 2003, which is used everywhere in pathlib.
New Fortran language standard features aren't being added to nvfortran until the Flang f18 LLVM project is ready to use. I would estimate this as being in a couple years from now.
