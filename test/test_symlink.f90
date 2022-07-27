program test_symlink

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : path_t, is_symlink, is_file, parent, create_symlink, remove

implicit none (type, external)

integer :: i, L
type(path_t) :: p_sym, p_tgt
character(1000) :: buf

character(:), allocatable :: tgt, link, linko

call get_command_argument(0, buf, status=i, length=L)
if(i /= 0 .or. L == 0) error stop "could not get own exe name"

tgt = trim(buf)
p_tgt = path_t(tgt)

link = parent(tgt) // "/test.link"
linko = parent(tgt) // "/test_oo.link"

! print *, "TRACE:test_symlink: target: " // tgt
! print *, "TRACE:test_symlink: link: " // link

p_sym = path_t(linko)

if (is_symlink(link)) then
  print *, "deleting old symlink " // link
  call remove(link)
endif
call create_symlink(tgt, link, status=i)
if(i < 0) then
  write(stderr,'(a)') "platform does not support symlinks"
  stop 77
elseif(i /= 0) then
  error stop "could not create symlink " // link
endif

if (p_sym%is_symlink()) then
  print *, "deleting old symlink " // p_sym%path()
  call p_sym%remove()
endif
call p_tgt%create_symlink(linko)

! call create_symlink("", "")  !< this error stops

if(is_symlink("")) error stop "is_symlink('') should be false"
if(is_symlink("not-exist-path.nobody")) error stop "is_symlink() should be false for non-existant path"

if(is_symlink(tgt)) error stop "is_symlink() should be false for non-symlink path"
if(p_tgt%is_symlink()) error stop "%is_symlink() should be false for non-symlink path"
if(.not. is_file(link)) error stop "is_file() should be true for existing regular file"

if(.not. is_symlink(link)) error stop "is_symlink() should be true for symlink path"
if(.not. p_sym%is_symlink()) error stop "%is_symlink() should be trum for symlink path"
if(.not. is_file(link)) error stop "is_file() should be true for existing symlink path"

print *, "OK: filesystem symbolic links"

end program
