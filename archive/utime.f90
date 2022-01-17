
interface
!! C standard library

integer(c_int) function utime_c(path) bind(C, name="utime_cf")
import c_int, c_char
character(kind=c_char), intent(in) :: path(*)
end function utime_c

end interface



module procedure utime
!! Sets file access_time and modified_time to current time.

character(kind=c_char, len=:), allocatable :: wk
integer(c_int) :: ierr

wk = expanduser(filename)

ierr = utime_c(wk // C_NULL_CHAR)
if(ierr /= 0) error stop "pathlib:utime: could not update mod time for file: " // filename

end procedure utime
