# Fortran filesystem

[![DOI](https://zenodo.org/badge/433875623.svg)](https://zenodo.org/badge/latestdoi/433875623)
[![ci_cmake](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_cmake.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_cmake.yml)
[![ci_fpm](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_fpm.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_fpm.yml)
[![oneapi-linux](https://github.com/scivision/fortran-filesystem/actions/workflows/oneapi-linux.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/oneapi-linux.yml)
[![ci_meson](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_meson.yml/badge.svg)](https://github.com/scivision/fortran-filesystem/actions/workflows/ci_meson.yml)

Platform independent (Linux, macOS, Windows, Cygwin, WSL, ...), object-oriented Fortran filesystem path manipulation library.
The library also provides header
[filesystem.h](./include/filesystem.h)
that can be used from C and C++ project code--see
[example](./example).
This Fortran library uses
[C++ stdlib filesystem](https://en.cppreference.com/w/cpp/filesystem)
internally.
Also inspired by
[Python pathlib](https://docs.python.org/3/library/pathlib.html).

Fortran "filesystem" module contains Fortran type "path_t" that contains properties and methods.
The "path_t" type uses getter and setter procedure to access the path as a string `character(:), allocatable`.

```fortran
use filesystem, only : path_t

type(path_t) :: p

p = path_t("my/path")  !< setter

print *, "path: ", p%path() !< getter
```

Due to compiler limitations, currently Fortran-filesystem only officially supports ASCII characters.

## Compiler support

Full C++ filesystem support and hence full Fortran-filesystem features are available with any of these compilers:

* GCC &ge; 8
* Clang &ge; 7
* Intel oneAPI (icx, ifx, icpc, ifort, icl)
* Visual Studio (C++) + oneAPI (Fortran)
* Cray (when using GCC or Intel backend with cray.cmake toolchain)

Expected to work with other
[C++17 compilers](https://en.cppreference.com/w/cpp/compiler_support)
and Fortran 2008 compilers yet to be tested.
E.g. IBM XL, NAG, et al.
In particular, the compiler and the libstdc++ must both support filesystem as well as Fortran 2008.

For compilers without functioning C++ filesystem, we provide a fallback set of filesystem features using the C runtime library and our own Fortran routines.
This fallback is enabled automatically when C++ filesystem is not available when CMake option `-Dfallback_auto=yes` is specified.
The installed CMake package provides BOOL CMake variable `ffilesystem_fallback` that can be used to check if the fallback routines are enabled.
To force enable the fallback routines, for example for testing:

```sh
cmake -B build -Dfallback=on
```

Note: to avoid end users missing features inadvertently, by default the fallback is disabled unless requested or enabled.
To allow automatic fallback, set:

```sh
cmake -B build -Dfallback_auto=on
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

**Cray PE** works with GCC or Intel backends.
The Cray compiler itself works with the non-C++ fallback.

## Build

Fortran-filesystem can be built with CMake, Fortran Package Manager (FPM), or Meson.

[lib]filesystem.a is the library binary built that contains the Fortran "filesystem" module--it is the only binary you need to use in your project.

Please see the [API docs](./API.md) for extensive list of functions/subroutines.

CMake:

```sh
cmake -B build
cmake --build build
# optional
ctest --test-dir build
```

Fortran Package Manager (fallback functions):

```sh
fpm build
```

Meson:

```sh
meson setup build
meson compile -C build
# optional
meson test -C build
```

## Command line

For user convenience, we provide a demo executable "filesystem_cli" that allows simple testing of what the filesystem routines do.
To build the filesystem_cli utility:

```sh
cmake -B build -DBUILD_UTILS=on
cmake --build build
```

## Usage from other projects

The [example](./example) directory contains a use pattern from external projects.
One can either `cmake --install build` or use ExternalProject from the other project.
[ffilesystem.cmake](./cmake/ffilesystem.cmake) would be included from the other project to find or build Fortran-filesystem automatically.
It provides the appropriate imported targets for shared or static builds, including Windows DLL handling.

## Notes

A few topics on unsupported features:

### non-ASCII characters

The UCS
[selected_char_kind('ISO_10646')](https://gcc.gnu.org/onlinedocs/gfortran/SELECTED_005fCHAR_005fKIND.html),
is an *optional* feature of Fortran 2003 standard.
Intel oneAPI does not support `selected_char_kind('ISO_10646')` as of this writing.

filesystem currently uses the default Fortran `character` kind, which is ASCII.
This means that UTF-8 / UTF-16 / UTF-32 strings are not supported.
You may find a particular compiler and computer passes some non-ASCII strings, but this is not supported.

### Unsupported compilers

At this time, these compilers aren't supported for reasons including:

#### Nvidia HPC-SDK

nvfortran 22.1 does not support `character(:), allocatable` from Fortran 2003, which is used everywhere in filesystem.
nvc++ 22.1 does not support C++ filesystem, which is essential for filesystem.
New Fortran language standard features aren't being added to nvfortran until the Flang f18 LLVM project is ready to use. I would estimate this as being in a couple years from now.
