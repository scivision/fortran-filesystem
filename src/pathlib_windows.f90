submodule (pathlib) pathlib_windows

implicit none (type, external)

contains


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
character :: f

is_absolute = .false.
if(len_trim(self%path_str) < 2) return

f = self%path_str(1:1)

if (.not. ((f >= "a" .and. f <= "z") .or. (f >= "A" .and. f <= "Z"))) return

is_absolute = self%path_str(2:2) == ":"

end procedure is_absolute


module procedure root

if (self%is_absolute()) then
  root = self%path_str(1:2)
else
  root = ""
end if

end procedure root


module procedure copy_file
!! copy file from source to destination
!! OVERWRITES existing destination files
!!
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


module procedure mkdir
!! create a directory, with parents if needed
integer :: i,j
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/md

type(path_t) :: p

p = self%expanduser()
p = p%as_windows()

if(p%is_directory()) return

call execute_command_line('mkdir ' // p%path_str, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not create directory " // p%path_str

end procedure mkdir


end submodule pathlib_windows
