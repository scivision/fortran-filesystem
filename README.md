# Fortran pathlib

[![DOI](https://zenodo.org/badge/433875623.svg)](https://zenodo.org/badge/latestdoi/433875623)
[![ci_cmake](https://github.com/scivision/fortran-pathlib/actions/workflows/ci_cmake.yml/badge.svg)](https://github.com/scivision/fortran-pathlib/actions/workflows/ci_cmake.yml)
[![intel-oneapi](https://github.com/scivision/fortran-pathlib/actions/workflows/intel-oneapi.yml/badge.svg)](https://github.com/scivision/fortran-pathlib/actions/workflows/intel-oneapi.yml)

Platform independent (Linux, macOS, Windows), object-oriented Fortran filesystem path manipulation library.
Currently tested with GCC Gfortran and Intel oneAPI compilers.
Would be happy to support additional Fortran 2018 compilers as available for testing.
Inspired by
[Python pathlib](https://docs.python.org/3/library/pathlib.html)
and
[C++ filesystem](https://en.cppreference.com/w/cpp/filesystem).

Fortran "pathlib" module contains one Fortran type "path_t" that contains properties and methods.
The "path_t" type uses getter and setter procedure to access the path as a string `character(:), allocatable`.

```fortran
use pathlib, only : path_t

type(path_t) :: p

p = path_t("my/path")  !< setter

print *, "path: ", p%path() !< getter
```

The retrieved path string may be indexed like:

```fortran
p%path(2,4)  !< character index 2:4

p%path(2) !< character index 2:end
```

In all the examples, we assume "p" is a pathlib path_t.

Build in CMake (can also use in your project via FetchContent or ExternalProject):

```sh
cmake -B build
cmake --build build
# optional
ctest --test-dir build
```

This creates build/libpathlib.a or similar.

## subroutines

These subroutines are available in the "pathlib" module.

Copy path to dest, overwriting existing file

```fortran
character(*) :: dest = "new/file.ext"

call p%copy_file(dest)
```

Make directory with parent directories if specified

```fortran
p = path_t("my/new/dir")
! suppose only directory "my" exists
call p%mkdir()
! now directory my/new/dir exists
```

Delete file

```fortran
call p%unlink()
```

## path_t

These methods emit a new "path_t" object.
It can be a new path_t object, or reassign to the existing path_t object.

Expand home directory, swapping file separators "\" for "/" and drop redundant file separators "//".

```fortran
! Fortran does not understand tilde "~"

p = path_t("~/my/path")
p = p%expanduser()
```

Resolve (canonicalize) path. This transparently uses C Runtime Library.

```fortran
p = path_t("~/../b")
p = p%resolve()

p%path() == "<absolute path of user home directory>/b"

! --- relative path resolved to current working directory
p = path_t("../b")
p = p%resolve()

p%path() == "<absolute path of current working directory>/b"
```

'/' => '\\' for Windows paths

```fortran
p = p%as_windows()
```

 '\\' => '/' for Unix paths, dropping redundant file separators "//"

```fortran
p = p%as_posix()
```

Swap file suffix

```fortran
p = path_t("my/file.h5")

p = p%with_suffix(".hdf5")

! p%path() == "my/file.hdf5"
```

Drop duplicated file separator "//"

```fortran
p = p%drop_sep()
```

Join path_t with other path string using posix separators.
The paths are treated like strings.
No path resolution is used, so non-sensical paths are possible for non-sensical input.

```fortran
p = path_t("a/b")

p = p%join("c/d")

! p%path == "a/b/c/d"
```

Split path_t into path components.
Path separators are discarded.

```fortran
character(:), allocatable :: parts

p = path_t("/a1/b23/c456/")

parts = p%parts()

! parts == [character(4) :: "a1", "b23", "c456"]
```

## integer

These methods emit an integer value.

len_trim() of %path()

```fortran
p%length()
```

---

File size:

```fortran
p%size_bytes()
```

or:

```fortran
size_bytes("my/file.txt")
```


## logical

These methods emit a logical value.

Does directory exist:

```fortran
p%is_dir()
```

or plain function:

```fortran
is_dir("my/dir")
```

---

Does file exist:

```fortran
p%is_file()
```

or plain function:

```fortran
is_file("my/file.txt")
```

```

Is path absolute:

```fortran
p%is_absolute()
```

Does path "p" resolve to the same path as "other":

```fortran
p%same_file(other)
```

is path executable file:

```fortran
p%is_exe()
```

## character(:), allocatable

These methods emit a string.

Get file suffix: extracts path suffix, including the final "." dot

```fortran
p%suffix()
```

Get parent directory of path:

```fortran
p%parent()
```

Get file name without path:

```fortran
p%file_name()
```

Get file name without path and suffix:

```fortran
p%stem()
```

Get drive root. E.g. Unix "/"  Windows "c:"
Requires absolute path or will return empty string.

```fortran
p%root()
```

Expand user home directory as a plain function:

```fortran
expanduser("~/my/path")
```

## System

Get home directory, or empty string if not found

```fortran
use pathlib, only : home

character(:), allocatable :: homedir

homedir = home()
```

Get current working directory

```fortran
use pathlib, only : cwd

character(:), allocatable :: cur

cur = cwd()
```
