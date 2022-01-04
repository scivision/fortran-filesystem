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

Copy path to dest. Optionally, overwrite existing file.
This is implemented with execute_command_line() because there isn't a simple function in CRT for this.

```fortran
character(*) :: dest = "new/file.ext"

call p%copy_file(dest)
! or
call copy_file("original.txt", "acopy.txt")

! overwrite
call copy_file("original.txt", "acopy.txt", overwrite=.true.)
```

Make directory with parent directories if specified

```fortran
p = path_t("my/new/dir")
! suppose only directory "my" exists
call p%mkdir()
! now directory my/new/dir exists
! OR
call mkdir("my/new/dir")
```

Delete file

```fortran
call p%unlink()
! or
call unlink("my/file.txt")
```

write text in character variable to file (overwriting existing file)

```fortran
call p%write_text(text)
! or
call write_text(filename, text)
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

Resolve (canonicalize) path.

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

## integer

These procedures emit an integer value.

len_trim() of p%path()

```fortran
p%length()
```

File size:

```fortran
p%size_bytes()
! or
size_bytes("my/file.txt")
```

## logical

These methods emit a logical value.

Does directory exist:

```fortran
p%is_dir()
! or
is_dir("my/dir")
```

Does file exist:

```fortran
p%is_file()
! or
is_file("my/file.txt")
```

Is path absolute:

```fortran
p%is_absolute()
! or
is_absolute("my/path")
```

Does path "p" resolve to the same path as "other":

```fortran
p%same_file(other)
! or
same_file(path1, path2)
```

is path executable file:

```fortran
p%is_exe()
! or
is_exe("my/file.exe")
```

## character(:), allocatable

These procedures emit a string.

'\\' => '/' for Unix paths, dropping redundant file separators "//"

```fortran
as_posix("my\path")
```

'/' => '\\' for Windows paths

```fortran
as_windows("my/path")
```

Drop duplicated file separator "//"

```fortran
drop_sep("my//path")  !< "my/path"
```

Split path_t into path components.
Path separators are discarded.

```fortran
character(:), allocatable :: parts

p = path_t("/a1/b23/c456/")

parts = p%parts()

! parts == [character(4) :: "a1", "b23", "c456"]

! OR

pts = parts("/a1/b23/c456/")

! pts == [character(4) :: "a1", "b23", "c456"]
```

Join path_t with other path string using posix separators.
The paths are treated like strings.
No path resolution is used, so non-sensical paths are possible for non-sensical input.

```fortran
join("a/b", "c/d") ! "a/b/c/d"
```

Get file suffix: extracts path suffix, including the final "." dot

```fortran
p%suffix()
! or
suffix("my/file.txt")  !< ".txt"
```

Swap file suffix

```fortran
with_suffix("to/my.h5", ".hdf5")  !< "to/my.hdf5"
```

Get parent directory of path:

```fortran
p%parent()
! or
parent("my/file.txt")  !< "my"
```

Get file name without path:

```fortran
p%file_name()
! or
file_name("my/file.txt")  ! "file.txt"
```

Get file name without path and suffix:

```fortran
p%stem()
! or
stem("my/file.txt")  !< "file"
```

Get drive root. E.g. Unix "/"  Windows "c:"
Requires absolute path or will return empty string.

```fortran
p%root()
! or
root("/a/b/c")
```

Expand user home directory:

```fortran
expanduser("~/my/path")
```

Resolve (canonicalize) path.

```fortran
resolve("~/../b")

! --- relative path resolved to current working directory
resolve("../b")
```

Get path relative to other path.
This is a string operation and does not resolve or expand paths.

```fortran
relative_to("/a/b/c", "/a/b")  !< "c"

p = path_t("/a/b/c")
p%relative_to("/a")  !< "b/c"
```

Read text from file into character variable (up to max_length characters).

```fortran
text = p%read_text(filename)
text = p%read_text(filename, 16384)  !< default length
! or
text = read_text(filename)
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

## Command line

For user convenience, we provide a demo executable "pathlib_cli" that allows simple testing of what the pathlib routines do.
