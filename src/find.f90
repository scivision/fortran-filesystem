submodule (filesystem) find_filesystem
!! procedures that find files

implicit none

contains


module procedure get_filename

character(:), allocatable :: path1, suff(:)
integer :: i

allocate(character(get_max_path()) :: get_filename)

if(present(suffixes)) then
  allocate(character(len(suffixes)) :: suff(size(suffixes)))
  suff = suffixes
else
  allocate(character(4) :: suff(3))
  suff = [character(4) :: '.h5', '.nc', '.dat']
endif

get_filename = path
!! avoid undefined return

if(len(get_filename) == 0) return

if(present(name)) then
  if(index(get_filename, name, back=.true.) == 0) then
    !> assume we wish to append stem to path
    get_filename = get_filename // '/' // name
  elseif(index(get_filename, '.', back=.true.) > 4) then
    !> it's a stem-matching full path with a suffix
    if(.not. is_file(get_filename)) get_filename = ''
    return
  endif
endif

if(is_file(get_filename)) return

allocate(character(get_max_path()) :: path1)
path1 = get_filename

do i = 1, size(suff)
  get_filename = path1 // trim(suff(i))
  if (is_file(get_filename)) return
enddo

get_filename = ''
if(present(name)) then
  write(stderr,*) 'filesystem:get_filename: ',name,' not found in ', path
else
  write(stderr,*) 'filesystem:get_filename: file not found: ',path
endif

end procedure get_filename


module procedure make_absolute

character(:), allocatable :: p

allocate(character(get_max_path()) :: p)
allocate(character(get_max_path()) :: make_absolute)

p = expanduser(path)
if (is_absolute(p)) then
  make_absolute = p
else
  make_absolute = expanduser(top_path) // '/' // p
endif

end procedure make_absolute


module procedure exe_path
integer :: L, ierr
character(:), allocatable :: buf

allocate(character(get_max_path()) :: buf)

call get_command_argument(0, buf, length=L, status=ierr)
if(ierr /= 0) error stop "ERROR: get_command_argument(0) failed"
if(L < 2) error stop "ERROR: get_command_argument(0) returned L < 2: " // trim(buf)

!! gfortran (Windows): full path
!! gfortran (Linux): relative path
!! ifort/ifx (Windows or Linux), nvfortran, flang: relative path

buf = canonical(buf)

allocate(character(len_trim(buf)) :: exe_path)
exe_path = buf

end procedure exe_path



end submodule find_filesystem
