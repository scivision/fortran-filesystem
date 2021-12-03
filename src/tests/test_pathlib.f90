program pathlib_test

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use pathlib, only : path_t

implicit none (type, external)


call test_setter_getter()

call test_filesep()

call test_manip()

call test_expanduser()

call test_is_directory()

call test_mkdir()

call test_absolute()

call test_executable()


contains


subroutine test_setter_getter()

type(path_t) :: p1

p1 = path_t("a/b/c")

if (p1%path(2,3) /= "/b") error stop "getter start,end"
if (p1%path(3,3) /= "b") error stop "getter same"
if (p1%path(2) /= "/b/c") error stop "getter start only"

print *, "OK: getter setter"

end subroutine test_setter_getter


subroutine test_filesep()

type(path_t) :: p1, p2, p3

p1 = path_t("")

p2 = p1%as_posix()
if (p2%path() /= "") error stop "as_posix empty"

p2 = p1%as_windows()
if (p2%path() /= "") error stop "as_windows empty"

p1 = path_t("/")
p3 = p1%as_posix()
if(p3%path() /= "/") error stop "as_posix '/' failed"

p2 = path_t(char(92))
p3 = p2%as_posix()
if(p3%path() /= "/") error stop "as_posix char(92) failed"

p3 = p1%as_windows()
if(p3%path() /= char(92)) error stop "as_windows '\' failed"

p3 = p2%as_windows()
if(p3%path() /= char(92)) error stop "as_windows char(92) failed"

print *, "OK: pathlib: filesep"

end subroutine test_filesep


subroutine test_manip()

type(path_t) :: p1, p2
logical :: is_unix

p1 = path_t("/")
is_unix = p1%is_absolute()

p1 = path_t("hi.a.b")
if (p1%stem() /= "hi.a") error stop "stem failed"
p2 = path_t(p1%stem())
if (p2%stem() /= "hi") error stop "stem nest failed"
p2 = path_t("hi")
if (p2%stem() /= "hi") error stop "stem idempotent failed"

if (p1%suffix() /= ".b") error stop "suffix failed"
p2 = path_t(p1%suffix())
if (p2%suffix() /= "") error stop "suffix nest failed on " // p2%path()
p2 = path_t(p2%suffix())
if (p2%suffix() /= "") error stop "suffix idempotent failed"

p1 = path_t("a/b/c")
if (p1%parent() /= "a/b") error stop "parent failed" // p1%path()
p2 = path_t(p1%parent())
if (p2%parent() /= "a") error stop "parent nest failed" // p1%path()
p2 = path_t("a")
if (p2%parent() /= ".") error stop "parent idempotent failed" // p2%path()

if (p1%file_name() /= "c") error stop "file_name failed"
if (p2%file_name() /= "a") error stop "file_name idempotent failed"

p1 = path_t("/etc")
p2 = path_t("c:/etc")
if(is_unix) then
  if(p1%root() /= "/") error stop "unix root failed"
  if(p2%root() /= "") error stop "unix root failed"
else
  if(p1%root() == "/") error stop "windows root failed"
  if(p2%root() /= "c:") error stop "windows root failed"
endif

p1 = path_t("my/file.h5")
p2 = p1%with_suffix(".hdf5")

if (p2%path() /= "my/file.hdf5") error stop "with_suffix failed: " // p2%path()

print *, "OK: pathlib: manip"

end subroutine test_manip


subroutine test_expanduser()

character(:), allocatable :: fn
integer :: i

type(path_t) :: p1, p2, p3

p1 = path_t("")
p2 = path_t("~")

p1 = p1%expanduser()
p2 = p2%expanduser()

if(p1%path() /= "") error stop "expanduser blank failed"
p3 = path_t(p2%path())
p3 = p3%expanduser()
if (p3%path() /= p2%path()) error stop "expanduser idempotent failed"

p1 = path_t("~/")
p1 = p1%expanduser()
fn = p1%path()
i = len(fn)
if (fn(i:i) /= "/") error stop "expanduser preserve separator failed"

print *, "OK: pathlib: expanduser"

end subroutine test_expanduser


subroutine test_is_directory()

integer :: i

type(path_t) :: p1,p2,p3

p1 = path_t(".")

if(.not. p1%is_directory()) error stop "did not detect '.' as directory"
if(p1%is_file()) error stop "detected '.' as file"

p2 = path_t('test-pathlib.h5')
open(newunit=i, file=p2%path(), status='replace')
close(i)

if(.not. p2%is_file()) error stop "did not detect " // p2%path() // " as file"
p3 = path_t('test-pathlib.h5.copy')
call p2%copy_file(p3%path())
if(.not. p3%is_file()) error stop "did not detect " // p3%path() // " as file"

if (p2%is_directory()) error stop "detected file as directory"
call unlink('test-pathlib.h5')
call unlink('test-pathlib.h5.copy')

p3 = path_t("not-exist-dir")
if(p3%is_directory()) error stop "not-exist-dir should not exist"

print *," OK: pathlib: is_directory"

end subroutine test_is_directory


subroutine test_mkdir()

type(path_t) :: p

p = path_t("test-pathlib-dir")

call p%mkdir()

if(.not.p%is_directory()) error stop "did not create directory" // p%path()

print *, "OK: pathlib: mkdir"

end subroutine test_mkdir


subroutine test_absolute()

type(path_t) :: p1,p2
logical :: is_unix

p1 = path_t("/")
is_unix = p1%is_absolute()

p1 = path_t("")
if (p1%is_absolute()) error stop "blank is not absolute"

if (is_unix) then
  p2 = path_t("/")
  if (.not. p2%is_absolute()) error stop p2%path() // "on Unix should be absolute"
  p2 = path_t("c:/")
  if (p2%is_absolute()) error stop p2%path() // "on Unix is not absolute"
else
  p2 = path_t("J:/")
  if (.not. p2%is_absolute()) error stop p2%path() // "on Windows should be absolute"
  p2 = path_t("j:/")
  if (.not. p2%is_absolute()) error stop p2%path() // "on Windows should be absolute"
  p2 = path_t("/")
  if (p2%is_absolute()) error stop p2%path() // "on Windows is not absolute"
endif

print *, "OK: pathlib: is_absolute"

end subroutine test_absolute


subroutine test_executable()

type(path_t) :: p1
character(4096) :: buf
integer :: i

call get_command_argument(1, buf, status=i)
if (i/=0) error stop "test_executable: input path to an executable file"

p1 = path_t(trim(buf))
if (.not.p1%executable()) error stop "did not detect executable file " // p1%path()

call get_command_argument(2, buf, status=i)
if (i/=0) error stop "test_executable: input path to an non-executable file"

p1 = path_t(trim(buf))
if (p1%executable()) error stop "did not detect non-executable file " // p1%path()


p1 = path_t("not-exist-file")
if (p1%executable()) error stop "non-existant file cannot be exectuable " // p1%path()

print *, "OK: pathlib: executable"
end subroutine test_executable


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
