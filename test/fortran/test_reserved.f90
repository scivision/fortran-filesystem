program reserved

use filesystem, only : path_t, is_char_device, is_reserved, is_unix, is_windows

implicit none

logical :: b

block
type(path_t) :: p1, p

if (is_reserved("a")) error stop "a is not reserved"
p1 = path_t("a")
if (p1%is_reserved()) error stop "a is not reserved"

b = is_reserved("NUL")
p = path_t("NUL")
if (is_windows()) then
    if (.not. b) error stop "NUL is reserved on Windows"
    if (.not. p%is_reserved()) error stop "NUL is reserved on Windows"
else
    if (b) error stop "NUL is not reserved on Unix"
    if (p%is_reserved()) error stop "NUL is not reserved on Unix"
endif


if(is_char_device("a")) error stop "a is not a char device"
if (p1%is_char_device()) error stop "a is not a char device"

b = is_char_device("/dev/null")
p = path_t("/dev/null")
if(is_unix()) then
    if (.not. b) error stop "/dev/null is a char device on Unix"
    if (.not. p%is_char_device()) error stop "/dev/null is a char device on Unix"
else
    if (b) error stop "/dev/null is not a char device on non-Unix systems"
    if (p%is_char_device()) error stop "/dev/null is not a char device on non-Unix systems"
endif

end block

end program
