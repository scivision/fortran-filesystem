program test_setenv

use filesystem

implicit none

valgrind : block
character(:), allocatable :: k, v, buf

k = "TEST_ENV_VAR"
v = "test_value"

call setenv(k, v)
buf = getenv(k)

if (buf /= v) error stop "setenv/getenv failed: " // buf // " /= " // v

end block valgrind

end program
