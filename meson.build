project('ffilesystem', 'c', 'cpp', 'fortran',
version: '3.4.3',
meson_version: '>= 0.55.0',
default_options : ['cpp_std=c++17'])

# --- does system work with symlinks

check_sym = files('cmake/check_fs_symlink.cpp')
cpp = meson.get_compiler('cpp')

if cpp.get_id() == 'gcc' and cpp.version().version_compare('<9.1.0')
  fs_dep = cpp.find_library('stdc++fs', required : true)
else
  fs_dep = []
endif

ret_symlink = cpp.run(check_sym, name: 'has_symlink', dependencies: fs_dep)
has_symlink = ret_symlink.returncode() == 0

has_filesys = cpp.has_header_symbol('filesystem', '__cpp_lib_filesystem')
if has_filesys
  has_experimental_filesys = false
else
  has_experimental_filesys = cpp.check_header('experimental/filesystem')
endif
if not (has_filesys or has_experimental_filesys)
  error('C++ filesystem stdlib support not found')
endif

# --- filesystem binary

c = configuration_data()
if has_symlink
  c.set('has_symlink', '.true.')
else
  c.set('has_symlink', '.false.')
endif
if has_filesys
  c.set('cpp_full_filesystem', '.true.')
else
  c.set('cpp_full_filesystem', '.false.')
endif
platform_src = configure_file(input : 'src/platform.in.f90', output : 'platform.f90', configuration : c)

filesystem = library('filesystem',
sources: [platform_src, files('src/filesystem.f90', 'src/iter.f90', 'src/io.f90', 'src/find.f90', 'src/fs_cpp.f90', 'src/fs.cpp')],
dependencies: fs_dep,
include_directories: 'src',
install: true
)
install_headers('src/filesystem.h')

# --- tests
subdir('tests')

# --- CLI
if get_option('BUILD_UTILS')
  cli = executable('filesystem_cli', files('app/main.f90'), link_with: filesystem)
endif
