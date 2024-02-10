program test_windows

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem

implicit none

character(1000) :: buf, buf2, buf3
integer :: i

call get_environment_variable("PROGRAMFILES", buf, status=i)
if (i /= 0) then
    write(stderr, '(a)') "Error getting PROGRAMFILES"
    error stop 77
endif

print '(a)', "PROGRAMFILES: " // trim(buf)

buf2 = long2short(buf)
print '(a)', trim(buf) // " => " // trim(buf2)
if(len_trim(buf2) == 0) then
    write(stderr, '(a)') "Error converting long path to short path: " // trim(buf2)
    error stop
endif

buf3 = short2long(buf2)
print '(a)', trim(buf2) // " => " // trim(buf3)
if(len_trim(buf3) == 0) then
    write(stderr, '(a)') "Error converting short path to long path: " // trim(buf3)
    error stop
endif

if(buf /= buf3) then
    write(stderr, '(a)') "Error: long2short(short2long(x)) != x"
    error stop
endif

end program
