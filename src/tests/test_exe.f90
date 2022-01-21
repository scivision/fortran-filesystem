program test_exe

use pathlib, only : path_t, is_exe

implicit none (type, external)

type(path_t) :: p1, p2

character(*), parameter :: exe_name = "dummy.exe", noexe_name = "dummy.no.exe"

if(is_exe("")) error stop "is_ext('') should be false"

p1 = path_t(exe_name)
call p1%touch()
call p1%chmod_exe()

if(.not. p1%is_file()) error stop "test_executable: " // exe_name // " is not a file."
if (.not. p1%is_exe()) error stop "%is_exe did not detect executable file " // exe_name
if (.not. is_exe(p1%path())) error stop "is_exe(path) did not detect executable file " // exe_name

p1 = path_t("not-exist-file")
if (p1%is_file()) error stop "test_executable: should not exist."
if (p1%is_exe()) error stop "non-existant file cannot be exectuable"

p2 = path_t(noexe_name)
call p2%touch()
call p2%chmod_no_exe()
if(.not. p2%is_file()) error stop "test_executable: " // noexe_name // " is not a file."
if (p1%is_exe()) error stop "did not detect non-executable file."

print *, "OK: pathlib: executable"

end program
