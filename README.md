# Fortran pathlib

Filesystem path manipulation utilities for standard Fortran

Inspired by
[Python pathlib](https://docs.python.org/3/library/pathlib.html)
and
[C++ filesystem](https://en.cppreference.com/w/cpp/filesystem).

## Manipulate filesystem

These procedures generally require access to the filesystem to manipulate paths.
These procedures are by definition
[impure](https://www.intel.com/content/www/us/en/develop/documentation/fortran-compiler-oneapi-dev-guide-and-reference/top/language-reference/a-to-z-reference/h-to-i/impure.html).

Resolve home directory as Fortran does not understand tilde

```fortran
character(:), allocatable function expanduser(in)
```

Get home directory, or empty string if not found

```fortran
character(:), allocatable function home()
```

Does directory exist:

```fortran
logical function is_directory(path)
```

Does file exist:

```fortran
logical function is_file(path)
```

Copy source to dest, overwriting existing files

```fortran
subroutine copy_file(source, dest)
```

Make directory with parent directories

```fortran
subroutine mkdir(path)
```

if path is absolute, return expanded path. If path is relative, top_path / path

```fortran
character(:), allocatable make_absolute(path, top_path)
```

## Pure procedures

These procedures do not access the filesystem and are therefore
[pure](https://www.intel.com/content/www/us/en/develop/documentation/fortran-compiler-oneapi-dev-guide-and-reference/top/language-reference/a-to-z-reference/o-to-p/pure.html).

Detect if path is absolute:

```fortran
logical function is_absolute(path)
```

Get file suffix: extracts path suffix, including the final "." dot

```fortran
character(:), allocatable function suffix(filename)
```

Get parent directory of path:

```fortran
character(:), allocatable function parent(path)
```

Get file name without path:

```fortran
character(:), allocatable function file_name(path)
```

Get file name without path and suffix:

```fortran
character(:), allocatable function stem(path)
```

'/' => '\\' for Windows paths

```fortran
character(:), allocatable function filesep_windows(path)
```

 '\\' => '/' for Unix paths

```fortran
character(:), allocatable function filesep_unix(path)
```

## assert

throw error if directory does not exist

```fortran
subroutine assert_is_directory(path)
```

throw error if file does not exist

```fortran
subroutine assert_is_file(path)
```
