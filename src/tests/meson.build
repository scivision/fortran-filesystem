# --- base class
test_pathlib = executable('test_pathlib', 'test_pathlib.f90', link_with: pathlib)
test('base', test_pathlib)

# --- C++17 filesystem methods
test_cpp_fs = executable('test_cpp_fs', 'test_cpp_fs.f90', link_with: pathlib)
test('CPP_filesystem', test_cpp_fs)

# --- file parts
test_pathlib_parts = executable('test_pathlib_parts', 'test_parts.f90', link_with: pathlib)
test('FileParts', test_pathlib_parts)

# --- expanduser
test_expanduser = executable('test_expanduser', 'test_expanduser.f90', link_with: pathlib)
test('Expanduser', test_expanduser)

# -- symlink
test_symlink = executable('test_symlink', 'test_symlink.f90', link_with: pathlib)
test('symlink', test_symlink)

# --- canonical
test_canonical = executable('test_canonical', 'test_canonical.f90', link_with: pathlib)
test('Canonical', test_canonical)

# --- file size
test_filesize = executable('test_filesize', 'test_filesize.f90', link_with: pathlib)
test('FileSize', test_filesize)

# --- exe detect
# non-exe doesn't work on WSL due to filesystem interface

#dummy_exe = files(meson.current_build_dir() / 'dummy.exe')
# file(['TOUCH', dummy_exe])
# file(['CHMOD', dummy_exe, 'PERMISSIONS', 'OWNER_EXECUTE', 'OWNER_READ', 'GROUP_EXECUTE'])
#if 'NOT', 'CMAKE_HOST_SYSTEM_VERSION', 'MATCHES', 'microsoft-standard'
 # dummy_noexe = [cmake_current_binary_dir, '/dummy']
  # file(['TOUCH', dummy_noexe])
#endif

#test_exe = executable('test_exe', 'test_exe.f90', link_with: pathlib)
#test('ExeDetect', test_exe, args: [dummy_exe, dummy_noexe])


# --- file ops
test_fileops = executable('test_fileops', 'test_fileop.f90', link_with: pathlib)
test('FileOps', test_fileops)

# --- read/write text file
test_rw_text = executable('test_rw_text', 'test_rw_text.f90', link_with: pathlib)
test('ReadWriteText', test_rw_text, args: [meson.current_build_dir() / 'test_rw_text.txt'])

# --- find
test_find = executable('test_find', 'test_find.f90', link_with: pathlib)
test('Find', test_find)
