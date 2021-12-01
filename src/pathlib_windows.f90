submodule (pathlib) pathlib_windows

implicit none (type, external)

contains


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
character :: f

is_absolute = .false.
if(len_trim(path) < 2) return

f = path(1:1)

if (.not. ((f >= "a" .and. f <= "z") .or. (f >= "A" .and. f <= "Z"))) return

is_absolute = path(2:2) == ":"

end procedure is_absolute


module procedure copy_file
!! copy file from source to destination
!! OVERWRITES existing destination files
!!
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/copy
integer :: i,j

character(:), allocatable  :: cmd

cmd = 'copy /y ' // filesep_windows(expanduser(source)) // ' ' // filesep_windows(expanduser(dest))

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // source // " => " // dest

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed
integer :: i,j
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/md

character(:), allocatable  :: buf

buf = expanduser(path)

if(is_directory(buf)) return

call execute_command_line('mkdir ' // filesep_windows(buf), exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not create directory " // path

end procedure mkdir


end submodule pathlib_windows
