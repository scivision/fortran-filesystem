program test_which

use filesystem

implicit none

valgrind : block

character(:), allocatable :: buf

if (is_windows()) then
    buf = which("cmd.exe")
else
    buf = which("ls")
endif

print '(a)', "which: " // buf

if (len_trim(buf) == 0) error stop "ERROR:test_exe: which() failed"

end block valgrind

print '(a)', "OK: which()"

end program
