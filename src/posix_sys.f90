submodule (pathlib) posix_sys

implicit none (type, external)

contains

module procedure copy_file
!! copy file from src to dst
!! OVERWRITES existing destination files
!!
!! https://linux.die.net/man/1/cp
integer :: i, j
character(:), allocatable  :: cmd, s, d

d = expanduser(dest)
s = expanduser(src)

cmd = 'cp -f ' // s // ' ' // d

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // s // " => " // d

end procedure copy_file

end submodule posix_sys
