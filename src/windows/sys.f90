submodule (pathlib) windows_sys

implicit none (type, external)

contains


module procedure copy_file
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/copy
integer :: i,j
character(:), allocatable  :: cmd, s, d
logical :: ow

d = as_windows(expanduser(dest))
s = as_windows(expanduser(src))

ow = .false.
if(present(overwrite)) ow = overwrite
if (is_file(d)) then
  if(ow) then
    call unlink(d)
  else
    error stop "pathlib:copy_file: overwrite=.false. and destination file exists: " // d
  endif
endif

cmd = 'copy /y ' // s // ' ' // d

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "pathlib:copy_file: could not copy " // s // " => " // d

end procedure copy_file


end submodule windows_sys
