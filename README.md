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
