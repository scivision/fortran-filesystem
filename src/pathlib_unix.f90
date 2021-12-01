submodule (pathlib) pathlib_unix
!! It was observed to be more reliable to use execute_command_line() rather
!! than using the C library directly.

implicit none (type, external)

contains


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib

is_absolute = .false.

if(len_trim(path) > 0) is_absolute = path(1:1) == "/"

end procedure is_absolute


module procedure copy_file
!! copy file from src to dst
!! OVERWRITES existing destination files
!!
!! https://linux.die.net/man/1/cp
integer :: i, j
character(:), allocatable  :: cmd

cmd = 'cp -rf ' // expanduser(source) // ' ' // expanduser(dest)

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // source // " => " // dest

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed
integer :: i, j
character(:), allocatable  :: buf

buf = expanduser(path)

if(is_directory(buf)) return

call execute_command_line('mkdir -p ' // buf, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not create directory " // path

end procedure mkdir

end submodule pathlib_unix
