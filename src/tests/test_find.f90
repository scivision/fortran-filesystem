program test_find

use, intrinsic :: iso_fortran_env, only : stderr => error_unit
use pathlib, only : remove, get_filename, mkdir, make_absolute, sys_posix, touch

implicit none (type, external)

call test_get_filename()
print *, "OK: get_filename"

call test_make_absolute()
print *, "OK: make_absolute"

contains

subroutine test_get_filename()

character(:), allocatable:: fn
integer :: i

character(*), parameter :: th5 = "test-pathlib.h5", tnc = "test-pathlib.nc", name = 'test-pathlib'


if(get_filename(' ') /= '') error stop 'empty 1'
if(get_filename(' ',' ') /= '') error stop 'empty 2'
!! " " instead of "" to avoid compile-time glitch error with GCC-10 with -Og

call remove(th5)

if(len(get_filename(th5)) > 0) error stop 'not exist full 1'

fn = get_filename(name)
if(len(fn) > 0) then
  write(stderr,*) 'ERROR: ',fn, len(fn)
  error stop 'not exist stem 1'
endif

!> touch empty file
call touch(th5)

if(get_filename(th5) /= th5) error stop 'exist full 1'
if(get_filename(name) /= th5) error stop 'exist stem 1'

fn = get_filename('.', name)
if(fn /= './' // th5) error stop 'exist stem 2: ' // fn

fn = get_filename('./' // name, name)
if(fn /= './' // th5) error stop 'exist parts 2: ' // fn

fn = get_filename('./' // th5, name)
if(fn /= './' // th5) error stop 'exist full 2: ' // fn

call remove(th5)

open(newunit=i, file=tnc, status='replace')
close(i)

if(get_filename(tnc) /= tnc) error stop 'exist full 1a'
if(get_filename(name) /= tnc) error stop 'exist stem 1a'
if(get_filename('.', name) /= './' // tnc) error stop 'exist stem 2a'
if(get_filename('./' // name, name) /= './' // tnc) error stop 'exist parts 2a'

call remove(tnc)

call mkdir('temp1/temp2')
call remove('temp1/temp2/' // th5)
fn = get_filename('temp1/temp2', name)
if (fn /= '') error stop 'non-exist dir'
open(newunit=i, file='temp1/temp2/' // th5, status='replace')
close(i)
fn = get_filename('temp1/temp2', name)
if (fn /= 'temp1/temp2/' // th5) error stop 'exist dir full 2'

fn = get_filename('./temp1/temp2', name)
if (fn /= './temp1/temp2/' // th5) error stop 'exist dir full 2a'

end subroutine test_get_filename


subroutine test_make_absolute()

character(16) :: fn2

if (sys_posix()) then
  fn2 = make_absolute("rel", "/foo")
  if (fn2 /= "/foo/rel") error stop "did not make_absolute Unix /foo/rel, got: " // fn2
else
  fn2 = make_absolute("rel", "j:/foo")
  if (fn2 /= "j:/foo/rel") error stop "did not make_absolute Windows j:/foo/rel, got: " // fn2
endif

if(make_absolute("rel", "") /= "/rel") error stop "make_absolute empty root"

if(make_absolute("", "") /= "/") error stop "make_absolute empty both"

if(make_absolute("", "rel") /= "rel/") error stop "make_absolute empty base: " //make_absolute("", "rel")

end subroutine test_make_absolute

end program
