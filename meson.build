project('ffilesystem', 'c', 'cpp', 'fortran',
version: '3.8.0',
meson_version: '>= 0.55.0',
default_options : ['cpp_std=c++17', 'c_std=c99'])

# --- does system work with symlinks

cc = meson.get_compiler('c')
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

has_filesys = cpp.has_header_symbol('filesystem', '__cpp_lib_filesystem')
if not has_filesys
  error('C++ filesystem stdlib support not found')
endif

# --- libdl

if host_machine.system() == 'windows'
  dl_dep = []
  cpp_args = '-DFS_DLL_NAME='
  cpp_args += cc.get_id() == 'msvc' ? '"filesystem.dll"' : '"libfilesystem.dll"'
else
  dl_dep = cc.find_library('dl', required: false)

  have_dladdr = cc.has_function('dladdr', args: '-D_GNU_SOURCE', dependencies: dl_dep)
  if have_dladdr
    cpp_args = '-DHAVE_DLADDR'
  endif
endif

# --- filesystem binary

filesystem = library('filesystem',
sources: files('src/filesystem.f90', 'src/iter.f90', 'src/io.f90', 'src/find.f90',
  'src/fs_cpp.f90', 'src/filesystem.cpp'),
dependencies: [fs_dep, dl_dep],
include_directories: 'include',
install: true,
cpp_args: cpp_args
)
install_headers('include/filesystem.h')

# --- tests
subdir('test')

# --- CLI
cli = executable('filesystem_cli', files('app/main.f90'), link_with: filesystem)
