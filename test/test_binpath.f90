program test_binpath

use filesystem, only : exe_path, lib_path, is_macos, is_windows

implicit none


call test_exe_path()

call test_lib_path()

contains


subroutine test_exe_path()

character(:), allocatable :: binpath
integer :: i

binpath = exe_path()
i = index(binpath, 'test_binpath')
if (i<1) error stop "ERROR:test_binpath: exe_path not found correctly: " // binpath

print *, "OK: exe_path: ", binpath
end subroutine


subroutine test_lib_path()

character(:), allocatable :: binpath, name
integer :: i, L
character :: s
logical :: shared

if(command_argument_count() /= 1) error stop "need argument 0 for static or 1 for shared"

call get_command_argument(1, s, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath: get_command_argument failed"
if(L/=1) error stop "ERROR:test_binpath: expected argument 0 for static or 1 for shared"
shared = s == '1'

if(.not. shared) then
  print *, "SKIPPED: lib_path: static library"
  return
endif

binpath = lib_path()

if (is_macos()) then
  name = 'filesystem.dylib'
elseif(is_windows()) then
  name = 'filesystem.dll'
else
  name = 'libfilesystem.so'
endif

i = index(binpath, name)
if (i<1) error stop "ERROR:test_binpath: lib_path not found correctly: " // binpath // ' with name ' // name

print *, "OK: lib_path: ", binpath
end subroutine

end program
