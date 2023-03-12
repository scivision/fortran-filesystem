program reserved

use filesystem, only : path_t, is_reserved, is_windows

implicit none

logical :: b
type(path_t) :: p

if (is_reserved("a")) error stop "a is not reserved"

b = is_reserved("NUL")
if (is_windows()) then
    if (.not. b) error stop "NUL is reserved on Windows"
else
    if (b) error stop "NUL is not reserved on Unix"
endif

p = path_t("a")
if (p%is_reserved()) error stop "ais not reserved"

p = path_t("NUL")
if(is_windows()) then
    if (.not. p%is_reserved()) error stop "NUL is reserved on Windows"
else
    if (p%is_reserved()) error stop "NUL is not reserved on Unix"
endif


end program
