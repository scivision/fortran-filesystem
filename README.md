# Ffilesystem: for Fortran using C or C++

[![DOI](https://zenodo.org/badge/433875623.svg)](https://zenodo.org/badge/latestdoi/433875623)
[![ci](https://github.com/scivision/fortran-filesystem/actions/workflows/ci.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/ci.yml)
[![ci_fpm](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_fpm.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_fpm.yml)
[![oneapi-linux](https://github.com/scivision/fortran-filesystem/actions/workflows/oneapi-linux.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/oneapi-linux.yml)

Platform independent (Linux, macOS, Windows, Cygwin, WSL, ...), object-oriented Fortran filesystem "Ffilesystem" path manipulation library.
The library also provides header
[ffilesystem.h](./include/ffilesystem.h)
that can be used from C and C++ project code--see
[example](./example).
Ffilesystem uses
[C++ stdlib filesystem](https://en.cppreference.com/w/cpp/filesystem)
or vendored
[CWalk](https://github.com/likle/cwalk)
and C runtime library.
Inspired by
[Python pathlib](https://docs.python.org/3/library/pathlib.html).

Fortran "filesystem" module contains Fortran type "path_t" that contains properties and methods.
The "path_t" type uses getter and setter procedure to access the path as a string `character(:), allocatable`.

```fortran
use filesystem, only : path_t

type(path_t) :: p

p = path_t("my/path")  !< setter

print *, "path: ", p%path() !< getter
```

## Compiler support

Ffilesystem supports compilers including:

* GCC &ge; 7 (gcc/g++, gfortran)
* LLVM Clang &ge; 7 (clang/clang++, flang or gfortran)
* Intel oneAPI (icx, ifx, ifort)
* AMD AOCC (clang/clang++, flang)
* NVidia HPC SDK (nvc++, nvfortran)
* Visual Studio (C/C++)
* Cray: using Cray compilers alone (cc, CC, ftn) or using GCC or Intel backend

```sh
cmake -Bbuild -Dcpp=no
```

### libstdc++

Some systems have broken, obsolete, or incompatible libstdc++.

**Intel**: If Intel compiler linker errors use GCC >= 9.1.
This can be done by setting environment variable CXXFLAGS to the top level GCC >= 9.1 directory.
Set environment variable CXXFLAGS for
[Intel GCC toolchain](https://www.intel.com/content/www/us/en/develop/documentation/oneapi-dpcpp-cpp-compiler-dev-guide-and-reference/top/compiler-reference/compiler-options/compiler-option-details/compatibility-options/gcc-toolchain.html)
like:

```sh
export CXXFLAGS=--gcc-toolchain=/opt/rh/gcc-toolset-10/root/usr/
```

which can be determined like:

```sh
scl enable gcc-toolset-10 "which g++"
```

## Build

Ffilesystem can be built with CMake or Fortran Package Manager (FPM).

"libffilesystem.a" is the library binary built that contains the Fortran "filesystem" module--it is the only binary you need to use in your project.

Please see the [API docs](./API.md) for extensive list of functions/subroutines.

CMake:

```sh
cmake -B build
cmake --build build
# optional
ctest --test-dir build
```

Fortran Package Manager (C- and Fortran-only functions):

```sh
fpm build
```

For user convenience, we provide a demo executable "filesystem_cli" that allows simple testing of what the filesystem routines do.

## Usage from other projects

The [example](./example) directory contains a use pattern from external projects.
One can either `cmake --install build` or use ExternalProject from the other project.
[ffilesystem.cmake](./cmake/ffilesystem.cmake) would be included from the other project to find or build Ffilesystem automatically.
It provides the appropriate imported targets for shared or static builds, including Windows DLL handling.

## Concepts

The [concepts](./concepts/) directory shows a few concepts for future consideration.

[exe_dir](./concepts/exe_dir/)
is a working example of how to determine an exeucutable's full path no matter what the current working directory is.
This can be useful when a data file is known to exist relative to an executable.
This is relevant to say CMake installed project that has an executable and associated data files installed.
Assuming the user knows the path to the MAIN executable in the installed directory, the program can determine its own full path and
then a priori know the relative path to the data file(s).

## Notes

A few topics on unsupported features:

### non-ASCII characters

Due to compiler limitations, currently Ffilesystem only officially supports ASCII characters.

The UCS
[selected_char_kind('ISO_10646')](https://gcc.gnu.org/onlinedocs/gfortran/SELECTED_005fCHAR_005fKIND.html),
is an *optional* feature of Fortran 2003 standard.
Intel oneAPI does not support `selected_char_kind('ISO_10646')` as of this writing.

filesystem currently uses the default Fortran `character` kind, which is ASCII.
This means that UTF-8 / UTF-16 / UTF-32 strings are not supported.
You may find a particular compiler and computer passes some non-ASCII strings, but this is not supported.
