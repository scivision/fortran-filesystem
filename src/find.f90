submodule (pathlib) find_pathlib
!! procedures that find files

implicit none (type, external)

contains


module procedure get_filename

character(:), allocatable :: path1, suff(:)
integer :: i
logical :: exists

if(present(suffixes)) then
  suff = suffixes
else
  suff = [character(4) :: '.h5', '.nc', '.dat']
endif

get_filename = trim(path)  !< first to avoid undefined return

if(len(get_filename) == 0) return

if(present(name)) then
  if(index(get_filename, name, back=.true.) == 0) then
    !> assume we wish to append stem to path
    get_filename = get_filename // '/' // name
  elseif(index(get_filename, '.', back=.true.) > 4) then
    !> it's a stem-matching full path with a suffix
    inquire(file=get_filename, exist=exists)
    if(.not. exists) get_filename = ''
    return
  endif
endif

inquire(file=get_filename, exist=exists)
if(exists) return

path1 = get_filename

do i = 1, size(suff)
  get_filename = path1 // trim(suff(i))
  inquire(file=get_filename, exist=exists)
  if (exists) return
enddo

get_filename = ''
if(present(name)) then
  write(stderr,*) 'pathlib:get_filename: ',name,' not found in ', path
else
  write(stderr,*) 'pathlib:get_filename: file not found: ',path
endif

end procedure get_filename


end submodule find_pathlib
