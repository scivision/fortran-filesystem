project('FortranPathlib', 'c', 'cpp', 'fortran',
version: '3.0.0',
default_options : ['default_library=static', 'cpp_std=c++17'])

# --- does system work with symlinks

check_sym = files('cmake/check_fs_symlink.cpp')
cpp = meson.get_compiler('cpp')
ret_symlink = cpp.run(check_sym, name: 'has_symlink')
has_symlink = ret_symlink.returncode() == 0

# --- pathlib binary

pathlib = library('pathlib',
sources: files('src/pathlib.f90', 'src/iter.f90', 'src/io.f90', 'src/find.f90', 'src/fs_cpp.f90', 'src/fs.cpp')
)

# --- tests
subdir('src/tests')
