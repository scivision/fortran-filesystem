program pathlib_test

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use pathlib, only : copy_file, mkdir, expanduser, is_absolute, is_file, is_directory, &
file_name, parent, stem, suffix, filesep_unix, filesep_windows, assert_is_directory, assert_is_file

implicit none (type, external)

call test_filesep()

call test_manip()

call test_expanduser()

call test_is_directory()

call test_assert()

call test_absolute()


contains


subroutine test_filesep()

if (filesep_unix("") /= "") error stop "filesep_unix empty"
if (filesep_windows("") /= "") error stop "filesep_windows empty"

if(filesep_unix("/") /= "/") error stop "filesep_unix '/' failed"
if(filesep_unix(char(92)) /= "/") error stop "filesep_unix char(92) failed"

if(filesep_windows("/") /= char(92)) error stop "filesep_windows '\' failed"
if(filesep_windows(char(92)) /= char(92)) error stop "filesep_windows char(92) failed"

print *, "OK: pathlib: filesep"

end subroutine test_filesep


subroutine test_manip()

if (stem("hi.a.b") /= "hi.a") error stop "stem failed"
if (stem(stem("hi.a.b")) /= "hi") error stop "stem nest failed"
if (stem("hi") /= "hi") error stop "stem idempotent failed"

if (parent("a/b/c") /= "a/b") error stop "parent failed"
if (parent(parent("a/b/c")) /= "a") error stop "parent nest failed"
if (parent("a") /= ".") error stop "parent idempotent failed"

if (file_name("a/b/c") /= "c") error stop "file_name failed"
if (file_name("c") /= "c") error stop "file_name idempotent failed"

if (suffix("hi.a.b") /= ".b") error stop "suffix failed"
if (suffix(suffix("hi.a.b")) /= "") error stop "suffix nest failed"
if (suffix("hi") /= "") error stop "suffix idempotent failed"

end subroutine test_manip


subroutine test_expanduser()

character(:), allocatable :: fn
integer :: i

if (expanduser(expanduser("~")) /= expanduser("~")) error stop "expanduser idempotent failed"

fn = expanduser("~/")
i = len(fn)
if (fn(i:i) /= "/") error stop "expanduser preserve separator failed"

end subroutine test_expanduser


subroutine test_is_directory()

integer :: i

if(.not.(is_directory('.'))) error stop "did not detect '.' as directory"
if(is_file('.')) error stop "detected '.' as file"

open(newunit=i, file='test-pathlib.h5', status='replace')
close(i)

call assert_is_file('test-pathlib.h5')
call copy_file('test-pathlib.h5', 'test-pathlib.h5.copy')
call assert_is_file('test-pathlib.h5.copy')

if((is_directory('test-pathlib.h5'))) error stop "detected file as directory"
call unlink('test-pathlib.h5')
call unlink('test-pathlib.h5.copy')

if(is_directory('not-exist-dir')) error stop "not-exist-dir should not exist"

print *," OK: pathlib: is_directory"
end subroutine test_is_directory


subroutine test_assert()

call assert_is_directory('.')

call mkdir('test-pathlib')
call assert_is_directory('test-pathlib')


end subroutine test_assert


subroutine test_absolute()

character(:), allocatable:: fn

logical :: is_unix

fn = expanduser("~")
is_unix = fn(1:1) == "/"

if (is_absolute("")) error stop "blank is not absolute"

if (is_unix) then
  if (.not.is_absolute("/")) error stop "is_absolute('/') on Unix should be true"
  if (is_absolute("c:/")) error stop "is_absolute('c:/') on Unix should be false"
else
  if (.not.is_absolute("J:/")) error stop "is_absolute('J:/') on Windows should be true"
  if (.not.is_absolute("j:/")) error stop "is_absolute('j:/') on Windows should be true"
  if (is_absolute("/")) error stop "is_absolute('/') on Windows should be false"
endif

print *, "OK: pathlib: expanduser,is_absolute"

end subroutine test_absolute


subroutine unlink(path)
character(*), intent(in) :: path
integer :: i
logical :: e

inquire(file=path, exist=e)
if (.not.e) return

open(newunit=i, file=path, status='old')
close(i, status='delete')
end subroutine unlink

end program
