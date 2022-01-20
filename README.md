# Fortran pathlib

[![DOI](https://zenodo.org/badge/433875623.svg)](https://zenodo.org/badge/latestdoi/433875623)
[![ci_cmake](https://github.com/scivision/fortran-pathlib/actions/workflows/ci_cmake.yml/badge.svg)](https://github.com/scivision/fortran-pathlib/actions/workflows/ci_cmake.yml)
[![intel-oneapi](https://github.com/scivision/fortran-pathlib/actions/workflows/intel-oneapi.yml/badge.svg)](https://github.com/scivision/fortran-pathlib/actions/workflows/intel-oneapi.yml)

Platform independent (Linux, macOS, Windows), object-oriented Fortran filesystem path manipulation library.
This Fortran library uses
[C++17 filesystem](https://en.cppreference.com/w/cpp/filesystem)
internally.
That is, we do not use compiler extensions unless C++17 filesystem isn't available.
For those old compilers, pathlib falls back to C stdlib and vendor extensions to standard Fortran.
Also inspired by
[Python pathlib](https://docs.python.org/3/library/pathlib.html).

Currently tested with compilers below, all of which use C++17 filesystem except GCC 7.

* GCC 7
* GCC 8, 9, 10, 11
* Clang
* Intel oneAPI

Should work with other C++17 and Fortran 2008 compilers, but we haven't tested them.
E.g. Cray, IBM XL, NAG, et al.

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

Fortran-pathlib can be built with your choice of: Makefile, CMake, Meson, Fortran Package Manager (FPM).

[lib]pathlib.a is the library binary built that contains the Fortran "pathlib" module--it is the only binary you need to use in your project.

Please see the [API docs](./API.md) for extensive list of functions/subroutines.

GNU Make creates /src/pathlib.a:

```sh
make -C src
```

CMake:

```sh
cmake -B build
cmake --build build
# optional
ctest --test-dir build
```

Meson build system:

```sh
meson setup build
meson compile -C build
# optional
meson test -C build
```

Fortran Package Manager (FPM):

```sh
fpm build
```

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
nvc++ 22.1 does not support C++17 filesystem, which is essential for pathlib.
New Fortran language standard features aren't being added to nvfortran until the Flang f18 LLVM project is ready to use. I would estimate this as being in a couple years from now.
