program test_symlink

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : path_t, is_symlink, is_file, is_dir, parent, create_symlink, remove, touch

implicit none

integer :: i, L

block
type(path_t) :: p_sym, p_tgt
character(1000) :: buf
integer :: stat, shaky
character :: buf1

character(:), allocatable :: tgt, cmake_link, link, linko, tgt_dir, link_dir

if(is_symlink("not-exist-file")) error stop "is_symlink() should be false for non-existant file"
if(is_symlink("")) error stop "is_symlink('') should be false"

if (command_argument_count() == 0) error stop "please give test link file"
call get_command_argument(1, buf, status=i, length=L)
if(i /= 0 .or. L == 0) error stop "could not get test link file from command line"
cmake_link = buf(1:L)
tgt_dir = parent(cmake_link)

if(.not.is_symlink(cmake_link)) error stop "is_symlink() should be true for symlink file: " // cmake_link

tgt = tgt_dir // "/test.txt"
call touch(tgt)

p_tgt = path_t(tgt)

link = tgt_dir // "/test.link"
linko = tgt_dir // "/test_oo.link"
link_dir = tgt_dir // "/my_link.dir"

! print *, "TRACE:test_symlink: target: " // tgt
! print *, "TRACE:test_symlink: link: " // link

shaky = 0
if(command_argument_count() > 1) then
  call get_command_argument(2, buf1, status=i, length=L)
  if(i /= 0 .or. L == 0) error stop "could not get shaky from command line"
  read(buf1, '(i1)') shaky
endif

if(shaky == 0) then
  call create_symlink(tgt, "", stat)
  if (stat == 0) error stop "create_symlink() should fail with empty link"
  print '(a)', "PASSED: create_symlink: empty link"
  if(is_symlink(tgt)) error stop "is_symlink() should be false for non-symlink file: " // tgt

  call create_symlink("", link, stat)
  if (stat == 0) error stop "create_symlink() should fail with empty target"
  print '(a)', "PASSED: create_symlink: empty target"
endif

p_sym = path_t(linko)

if (is_symlink(link)) then
  print *, "deleting old symlink " // link
  call remove(link)
endif
call create_symlink(tgt, link)
print '(a)', "PASSED: created symlink " // link

if (p_sym%is_symlink()) then
  print *, "deleting old symlink " // p_sym%path()
  call p_sym%remove()
endif
call p_tgt%create_symlink(linko)
print '(a)', "PASSED: created symlink " // p_sym%path()

!> directory symlinks
if (is_symlink(link_dir)) then
  print *, "deleting old symlink " // link_dir
  call remove(link_dir)
endif
call create_symlink(tgt_dir, link_dir)

!> checks
! call create_symlink("", "")  !< this error stops

!> file symlinks
if(is_symlink(tgt)) error stop "is_symlink() should be false for non-symlink file"
if(p_tgt%is_symlink()) error stop "%is_symlink() should be false for non-symlink file"
if(.not. is_file(link)) error stop "is_file() should be true for existing regular file: " // link

if(.not. is_symlink(link)) error stop "is_symlink() should be true for symlink file: " // link
if(.not. p_sym%is_symlink()) error stop "%is_symlink() should be trum for symlink file: " // p_sym%path()
if(.not. is_file(link)) error stop "is_file() should be true for existing symlink file: " // link

print '(a)', "PASSED: test_symlink: file"

!> directory symlinks
if(is_symlink(tgt_dir)) error stop "is_symlink() should be false for non-symlink dir"
if(.not. is_dir(link_dir)) error stop "is_dir() should be true for existing regular dir" // link_dir

if(.not. is_symlink(link_dir)) error stop "is_symlink() should be true for symlink dir: " // link_dir
if(.not. is_dir(link_dir)) error stop "is_dir() should be true for existing symlink dir: " // link_dir
end block

print *, "OK: filesystem symbolic links"

end program
