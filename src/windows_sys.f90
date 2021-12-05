submodule (pathlib) windows_sys

implicit none (type, external)

contains


module procedure copy_file
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/copy
integer :: i,j

character(:), allocatable  :: cmd

type(path_t) :: s, d

d%path_str = dest
d = d%expanduser()
d = d%as_windows()

s = self%expanduser()
s = s%as_windows()

cmd = 'copy /y ' // s%path_str // ' ' // d%path_str

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // self%path_str // " => " // dest

end procedure copy_file


end submodule windows_sys
