submodule (pathlib) posix_sys

implicit none (type, external)

contains

module procedure copy_file
!! copy file from src to dst
!! OVERWRITES existing destination files
!!
!! https://linux.die.net/man/1/cp
integer :: i, j
character(:), allocatable  :: cmd

type(path_t) :: d, s

d%path_str = dest
d = d%expanduser()
s = self%expanduser()

cmd = 'cp -f ' // s%path_str // ' ' // d%path_str

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // self%path_str // " => " // dest

end procedure copy_file

end submodule posix_sys
