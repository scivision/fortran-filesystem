submodule (pathlib) windows_sys

implicit none (type, external)

contains


module procedure copy_file
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/copy
integer :: i,j

character(:), allocatable  :: cmd, s, d

d = as_windows(expanduser(dest))
s = as_windows(expanduser(src))

cmd = 'copy /y ' // s // ' ' // d

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // s // " => " // d

end procedure copy_file


end submodule windows_sys
