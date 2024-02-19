program test_canon

use filesystem

implicit none

call test_same_file()
print *, "OK: same_file"

contains

subroutine test_same_file()

type(path_t) :: p1, p2

!> hardlink resolves to the same file
if(.not. same_file(".", get_cwd())) error stop 'ERROR: same_file(., get_cwd())'

call mkdir("test-a/b/")

p1 = path_t("test-a/c")
call p1%touch()
if(.not. is_file("test-a/c")) error stop "touch test-a/c failed"

p2 = path_t("test-a/b/../c")

if (.not. p1%same_file(p2)) error stop 'ERROR: %same_file'
if (.not. same_file(p1%path(), p2%path())) error stop 'ERROR: same_file()'

if (.not. same_file("~", "~")) error stop 'ERROR: same_file(~,~)'
if (.not. same_file("~", "~/")) error stop 'ERROR: same_file(~,~/)'

if(same_file("not-exist-same", "not-exist-same")) error stop 'ERROR: same_file(not-exist-same, not-exist-same)'


end subroutine test_same_file

end program
