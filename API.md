# Fortran pathlib API

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

C++17 filesystem is used extensively within Fortran-pathlib to implement functions in a platform-agnostic and robust way.
The fallback functions use C stdlib when C++17 filesystem isn't available.
For the interchange of character strings between Fortran and C/C++, a fixed buffer length is used.
This buffer length is defined as MAXP in src/pathlib.f90.
Currently, MAXP = 4096; that is, 4096 ASCII characters is the maximum path length.
The operating system and filesystem may have stricter limits.
If this fixed buffer length becomes an issue, we may be able to update pathlib to make the length dynamic.

## System capabilities

Not every system is capable of every pathlib feature. At the moment, this limitation applies to Windows MinGW GCC with symbolic (soft) links.
We provide the status of the symlink feature via `logical function pathlib_has_symlink()` to avoid user program errors--check if pathlib has a feature before using the feature.
This function does NOT tell if a particular drive is capable of symlinks.

```fortran
use pathlib

if (pathlib_has_symlink()) then
  call create_symlink("my/path", "my/symlink")
endif
```

## subroutines

These subroutines are available in the "pathlib" module.

Copy path to dest. Optionally, overwrite existing file.

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

Touch file (create empty file if not a file).
The directories containing the file must already exist.
Also updates the file access/modification times to current time.

```fortran
call p%touch()
! or
call touch("myfile.ext")
```

Delete file, empty directory, or symbolic link (the target of a symbolic link is not deleted).

```fortran
call p%remove()
! or
call remove("my/file.txt")
```

write text in character variable to file (overwriting existing file)

```fortran
call p%write_text(text)
! or
call write_text(filename, text)
```

create symbolic link to file or directory:

```fortran
call p%create_symlink(link)
! or
call create_symlink(target, link)
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

Normalize path, a lexical operation removing ".." and "." and duplicate file separators "//".
The path need not exist.

```fortran
p = p%normal()
! or
normal("./my/path/../b")  !< "my/b"
```

Join path_t with other path string using posix separators.
The paths are treated like strings.
No path resolution is used, so non-sensical paths are possible for non-sensical input.

```fortran
p = path_t("a/b")

p = p%join("c/d")

! p%path == "a/b/c/d"
```

## integer(int64)

These procedures emit an 64-bit integer value.

len_trim() of p%path()

```fortran
p%length()
```

File size (bytes):

```fortran
p%file_size()
! or
file_size("my/file.txt")
```

## logical

These methods emit a logical value.

Does directory exist:

```fortran
p%is_dir()
! or
is_dir("my/dir")
```

Error stop if directory does not exist

```fortran
call assert_is_dir("my/dir")
```

Is "path" a file or directory (or a symbolic link to existing file or directory):

```fortran
p%exists()
! or
exists("my/file.txt")
```

Does file exist (or a symbolic link to an existing file):

```fortran
p%is_file()
! or
is_file("my/file.txt")
```

Error stop if file does not exist

```fortran
call assert_is_file("my/dir")
```

Is path a symbolic link:

```fortran
p%is_symlink()
! or
is_symlink("my/path")
```

Is path absolute:

```fortran
p%is_absolute()
! or
is_absolute("my/path")
```

Does path "p" resolve to the same path as "other".
To be true:

* path must exist
* path must be traversable  E.g. "a/b/../c" resolves to "a/c" iff a/b also exists.

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

Split path_t into path components.
Path separators are discarded.
`file_parts()` is a subroutine because GCC < 9 was buggy with `character(:), allocatable, dimension(:)` functions.
The functional method %parts() was OK at least to GCC >= 7.5.

```fortran
character(:), allocatable :: pts

p = path_t("/a1/b23/c456/")

pts = p%parts()

! pts == [character(4) :: "a1", "b23", "c456"]

! OR

call file_parts("/a1/b23/c456/", pts)

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

Get parent directory of path. The parent of the top-most relative path is ".".

```fortran
p%parent()
! or
parent("my/file.txt")  !< "my"

parent("a") !< "."
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
root("/a/b/c") !< "/" on Unix, "" on Windows

root ("c:/a/b/c") !< "c:" on Windows, "" on Unix
```

Expand user home directory:

```fortran
expanduser("~/my/path")   !< "/home/user/my/path" on Unix, "<root>/Users/user/my/path" on Windows
```

Resolve (canonicalize) path.
First attempts to resolve an existing path.
If that fails, the path is resolved as far as possible with existing path components, and then ".", ".." are lexiographically resolved.

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

Filessystem file separator:

```fortran
character :: sep
sep = filesep()
```

Get home directory, or empty string if not found

```fortran
character(:), allocatable :: home

home = get_homedir()
```

Get current working directory

```fortran
use pathlib, only : get_cwd

character(:), allocatable :: cur

cur = get_cwd()
```

Get system temporary directory:

```fortran
character(:), allocatable :: get_tempdir
```

Find a file "name" under "path"

```fortran
use pathlib, only : get_filename

function get_filename(path, name, suffixes)
!! given a path, stem and vector of suffixes, find the full filename
!! assumes:
!! * if present, "name" is the file name we wish to find (without suffix or directories)
!! * if name not present, "path" is the directory + filename without suffix
!!
!! suffixes is a vector of suffixes to check. Default is [character(4) :: '.h5', '.nc', '.dat']
!! if file not found, empty character is returned

character(*), intent(in) :: path
character(*), intent(in), optional :: name, suffixes(:)
character(:), allocatable :: get_filename
```

Make a path absolute if relative:

```fortran
function make_absolute(path, top_path)
!! if path is absolute, return expanded path
!! if path is relative, top_path / path
!!
!! idempotent iff top_path is absolute

character(:), allocatable :: make_absolute
character(*), intent(in) :: path, top_path
```

Tell if system is POSIX-like (MacOS, Unix, Linux, BSD, ...) or not (Windows)

```fortran
pure logical function sys_posix()
```
