program test_matching

use filesystem, only : match, path_t

implicit none (type, external)

call test_match()
print *, "OK: match"

contains

subroutine test_match()

type(path_t) :: p

if(.not. match("", "")) error stop "match empty"

if(.not. match("abc", "abc")) error stop "match exact failed"
if(.not. match("abc", "a.*")) error stop "match wildcard failed"

if(.not. match("/abc", "a.c")) error stop "match() dot failed"
p = path_t("/abc")
if(.not. p%match("a.c")) error stop "%match dot failed"

if(.not. match("abc34v", "a.c\d{2}")) error stop "match decimal failed"

end subroutine test_match

end program
