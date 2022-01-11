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
logical :: ow

d = expanduser(dest)
s = expanduser(src)

ow = .false.
if(present(overwrite)) ow = overwrite
if (is_file(d)) then
  if(ow) then
    call unlink(d)
  else
    error stop "pathlib:copy_file: overwrite=.false. and destination file exists: " // d
  endif
endif

cmd = 'cp ' // s // ' ' // d

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "pathlib:copy_file: could not copy " // s // " => " // d

end procedure copy_file

end submodule posix_sys
