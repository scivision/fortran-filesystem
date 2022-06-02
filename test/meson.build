# --- base class
test_core = executable('test_core', 'test_core.f90', link_with: filesystem)
test('base', test_core)

# --- C++17 filesystem methods
test_cpp_fs = executable('test_cpp_fs', 'test_cpp_fs.f90', link_with: filesystem)
test('ffilesystem core', test_cpp_fs)

test_relto = executable('test_relto', 'test_cpp_relative.f90', link_with: filesystem)
test('relative to', test_relto)

test_match = executable('test_match', 'test_match.f90', link_with: filesystem)
test('match', test_match)

# --- file parts
test_filesystem_parts = executable('test_filesystem_parts', 'test_parts.f90', link_with: filesystem)
test('FileParts', test_filesystem_parts)

# --- expanduser
test_expanduser = executable('test_expanduser', 'test_expanduser.f90', link_with: filesystem)
test('Expanduser', test_expanduser)

# -- symlink
test_symlink = executable('test_symlink', 'test_symlink.f90', link_with: filesystem)
test('symlink', test_symlink)

# --- canonical
test_canonical = executable('test_canonical', 'test_canonical.f90', link_with: filesystem)
test('Canonical', test_canonical)

# --- file size
test_filesize = executable('test_filesize', 'test_filesize.f90', link_with: filesystem)
test('FileSize', test_filesize)

# --- exe detect
test_exe = executable('test_exe', 'test_exe.f90', link_with: filesystem)
test('ExeDetect', test_exe)

# --- file ops
test_fileops = executable('test_fileops', 'test_fileop.f90', link_with: filesystem)
test('FileOps', test_fileops)

# --- read/write text file
test_rw_text = executable('test_rw_text', 'test_rw_text.f90', link_with: filesystem)
test('ReadWriteText', test_rw_text, args: [meson.current_build_dir() / 'test_rw_text.txt'])

# --- find
test_find = executable('test_find', 'test_find.f90', link_with: filesystem)
test('Find', test_find, timeout: 10)

# --- test C++ interface
test_cpp = executable('test_cpp', 'test_cpp_ifc.cpp',
include_directories: '../include',
link_with: filesystem
)
test('C++ interface', test_cpp, timeout: 10)

# --- test C interface
test_c = executable('test_c', 'test_c_ifc.c',
include_directories: '../include',
link_with: filesystem
)
test('C interface', test_c, timeout: 10)
