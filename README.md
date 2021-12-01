# Fortran pathlib

Filesystem path manipulation utilities for standard Fortran

Inspired by Python pathlib and C++17 filesystem.


## API

Copy source to dest, overwriting existing files

```fortran
subroutine copyfile(source, dest)
```

Make directory with parents

```fortran
subroutine mkdir(path)
```

Detect if path is absolute:

```fortran
logical function is_absolute(path)
```

Does directory exist:

```fortran
logical function is_directory(path)
```

Does file exist:

```fortran
logical function is_file(path)
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

if path is absolute, return expanded path
if path is relative, top_path / path

```fortran
character(:), allocatable make_absolute(path, top_path)
```

'/' => '\' for Windows paths

```fortran
character(:), allocatable function filesep_windows(path)
```

 '\' => '/' for Unix paths

```fortran
character(:), allocatable function filesep_unix(path)
```

Resolve home directory as Fortran does not understand tilde

```fortran
character(:), allocatable function expanduser(in)
```

Get home directory, or empty string if not found

```fortran
character(:), allocatable function home()
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
