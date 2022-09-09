project('ffilesystem', 'c', 'cpp', 'fortran',
version: '3.7.2',
meson_version: '>= 0.55.0',
default_options : ['cpp_std=c++17'])

# --- does system work with symlinks

check_sym = files('cmake/check_fs_symlink.cpp')
cpp = meson.get_compiler('cpp')
fc = meson.get_compiler('fortran')

fs_dep = []
if cpp.get_id() == 'gcc'
  if cpp.version().version_compare('<9.1.0')
    fs_dep = cpp.find_library('stdc++fs', required : true)
  endif
endif

if fc.get_id() == 'gcc'
  add_project_arguments(['-Wno-maybe-uninitialized', '-Wno-uninitialized'], language: 'fortran')
endif

ret_symlink = cpp.run(check_sym, name: 'has_symlink', dependencies: fs_dep)
has_symlink = ret_symlink.returncode() == 0

has_filesys = cpp.has_header_symbol('filesystem', '__cpp_lib_filesystem')
if not has_filesys
  error('C++ filesystem stdlib support not found')
endif

# --- filesystem binary
cpp_args = has_symlink ? '-DHAVE_SYMLINK' : ''

filesystem = library('filesystem',
sources: files('src/filesystem.f90', 'src/iter.f90', 'src/io.f90', 'src/find.f90',
  'src/fs_cpp.f90', 'src/filesystem.cpp'),
dependencies: fs_dep,
include_directories: 'include',
install: true,
cpp_args: cpp_args
)
install_headers('include/filesystem.h')

# --- tests
subdir('test')

# --- CLI
cli = executable('filesystem_cli', files('app/main.f90'), link_with: filesystem)
