submodule (filesystem) find_filesystem
!! procedures that find files

implicit none (type, external)

contains


module procedure get_filename

character(:), allocatable :: path1, suff(:)
integer :: i

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
    if(.not. is_file(get_filename)) get_filename = ''
    return
  endif
endif

if(is_file(get_filename)) return

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

p = expanduser(path)
if (is_absolute(p)) then
  make_absolute = p
else
  make_absolute = expanduser(top_path) // '/' // p
endif

end procedure make_absolute


end submodule find_filesystem
