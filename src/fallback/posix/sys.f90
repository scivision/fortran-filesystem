submodule (filesystem) posix_sys

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
    call remove(d)
  else
    error stop "filesystem:copy_file: overwrite=.false. and destination file exists: " // d
  endif
endif

cmd = 'cp ' // s // ' ' // d

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "filesystem:copy_file: could not copy " // s // " => " // d

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed

integer :: i, j
character(:), allocatable  :: cmd, wk

wk = expanduser(path)  !< not canonical as it trims path part we want to create with mkdir
if (len_trim(wk) < 1) error stop 'filesystem:mkdir: must specify directory to create'

if(is_dir(wk)) return

cmd = "mkdir -p " // wk

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "filesystem:mkdir: could not make directory " // wk

end procedure mkdir


end submodule posix_sys
