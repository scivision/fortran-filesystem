submodule (filesystem) find_filesystem
!! procedures that find files

implicit none

contains


module procedure get_filename

character(:), allocatable :: path1
character(4), parameter :: suff(3) = [character(4) :: '.h5', '.nc', '.dat']
integer :: i

allocate(character(get_max_path()) :: get_filename)

get_filename = path
!! avoid undefined return

if(len(path) == 0) return

if(present(name)) then
  if(index(path, name, back=.true.) == 0) then
    !> assume we wish to append stem to path
    get_filename = path // '/' // name
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


end submodule find_filesystem
