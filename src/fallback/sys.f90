submodule (filesystem) system_call

implicit none (type, external)

contains


module procedure copy_file
!! copy file from src to dst

integer :: i, j
character(:), allocatable  :: cmd, s, d
logical :: ow

if(len_trim(src) == 0) then
  write(stderr,'(a)') "ERROR:filesystem:copy_file: source path must not be empty"
  if (present(status)) then
    status = 1
    return
  endif
  error stop
endif

if(len_trim(dest) == 0) then
  write(stderr,'(a)') "ERROR:filesystem:copy_file: destination path must not be empty"
  if (present(status)) then
    status = 1
    return
  endif
  error stop
endif

if(is_windows()) then
  d = as_windows(expanduser(dest))
  s = as_windows(expanduser(src))
  cmd = 'copy /y ' // s // ' ' // d
else
  d = expanduser(dest)
  s = expanduser(src)
  cmd = 'cp ' // s // ' ' // d
endif

ow = .false.
if(present(overwrite)) ow = overwrite
if (is_file(d)) then
  if(ow) then
    call remove(d)
  elseif (present(status)) then
    status = 1
    write(stderr,'(a)') "ERROR:filesystem:copy_file: overwrite=.false. and destination file exists: " // d
  else
    error stop "ERROR:filesystem:copy_file: overwrite=.false. and destination file exists: " // d
  endif
endif


call execute_command_line(cmd, exitstat=i, cmdstat=j)

if((i /= 0 .or. j /= 0)) then
  write(stderr,'(a)') "ERROR:filesystem:copy_file: could not copy " // s // " => " // d
  if(present(status)) then
    status = i
    if(i == 0) status = j
    return
  endif
  error stop
endif

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed

integer :: i, j
character(:), allocatable  :: cmd, wk

if(len_trim(path) == 0) then
  write(stderr,'(a)') "ERROR:filesystem:mkdir: source path must not be empty"
  if (present(status)) then
    status = 1
    return
  endif
  error stop
endif

wk = expanduser(path)
!! not canonical as it trims path part we want to create with mkdir

if(is_dir(wk)) return

if(is_windows()) then
  cmd = "mkdir " // as_windows(wk)
else
  cmd = "mkdir -p " // wk
endif

call execute_command_line(cmd, exitstat=i, cmdstat=j)

if((i /= 0 .or. j /= 0)) then
  write(stderr,'(a)') "ERROR:filesystem:mkdir: could not make directory " // wk
  if(present(status)) then
    status = i
    if(i == 0) status = j
    return
  endif
  error stop
endif

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


end submodule system_call
