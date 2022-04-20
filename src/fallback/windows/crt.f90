submodule (filesystem) c_windows

use, intrinsic :: iso_c_binding, only: c_int, c_long, c_char, c_null_char

implicit none (type, external)

interface

subroutine fullpath_c(absPath, relPath, maxLength) bind(c, name='_fullpath')
!! char *_fullpath(char *absPath, const char *relPath, size_t maxLength)
!! https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fullpath-wfullpath?view=vs-2019
import c_char, c_long
character(kind=c_char), intent(in) :: relPath(*)
character(kind=c_char), intent(out) :: absPath(*)
integer(c_long), intent(in) :: maxLength
end subroutine fullpath_c

end interface


contains


module procedure canonical

character(kind=c_char, len=:), allocatable :: wk
character(kind=c_char):: c_buf(MAXP)
character(MAXP) :: buf
integer :: i

if(len_trim(path) == 0) then
  canonical = ""
  return
endif

wk = expanduser(path)

!! some systems e.g. old MacOS can't handle leading "." or ".."

if (wk(1:1) == ".") wk = get_cwd() // "/" // wk

if(len(wk) > MAXP) error stop "filesystem:canonical: path too long: " // wk

call fullpath_c(c_buf, wk // c_null_char, MAXP)

do i = 1, MAXP
  if (c_buf(i) == c_null_char) exit
  buf(i:i) = c_buf(i)
enddo

canonical = as_posix(buf(:i-1))

end procedure canonical


end submodule c_windows
