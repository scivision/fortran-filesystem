submodule (filesystem) windows_sys

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
    call remove(d)
  else
    error stop "filesystem:copy_file: overwrite=.false. and destination file exists: " // d
  endif
endif

cmd = 'copy /y ' // s // ' ' // d

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

cmd = "mkdir " // as_windows(wk)

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "filesystem:mkdir: could not make directory " // wk

end procedure mkdir


pure function as_windows(path)
!! '/' => '\' for Windows systems
character(*), intent(in) :: path
character(:), allocatable :: as_windows

integer :: i

as_windows = trim(path)

i = index(as_windows, '/')
do while (i > 0)
  as_windows(i:i) = char(92)
  i = index(as_windows, '/')
end do

end function as_windows


end submodule windows_sys
