submodule (pathlib) pathlib_unix
!! It was observed to be more reliable to use execute_command_line() rather
!! than using the C library directly.

implicit none (type, external)

contains


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib

is_absolute = .false.

if(len_trim(self%path_str) > 0) is_absolute = self%path_str(1:1) == "/"

end procedure is_absolute


module procedure root

if(self%is_absolute()) then
  root = self%path_str(1:1)
else
  root = ""
end if

end procedure root


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

cmd = 'cp -rf ' // s%path_str // ' ' // d%path_str

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // self%path_str // " => " // dest

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed
integer :: i, j
type(path_t) :: p

p = self%expanduser()

if(p%is_directory()) return

call execute_command_line('mkdir -p ' // p%path_str, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not create directory " // p%path_str

end procedure mkdir

end submodule pathlib_unix
