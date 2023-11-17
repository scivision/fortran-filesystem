program test_canon

use filesystem, only : path_t,  same_file, mkdir, is_dir, is_file

implicit none

call test_same_file()
print *, "OK: same_file"

contains

subroutine test_same_file()

type(path_t) :: p1, p2

integer :: shaky, ierr
character :: buf

shaky = 0
if(command_argument_count() > 0) then
  call get_command_argument(1, buf, status=ierr)
  read(buf, '(i1)') shaky
endif

call mkdir("test-a/b/")

if(.not. is_dir("test-a/b")) error stop "mkdir test-a/b failed"

p1 = path_t("test-a/c")
call p1%touch()
if(.not. is_file("test-a/c")) error stop "touch test-a/c failed"

p2 = path_t("test-a/b/../c")

if (.not. p1%same_file(p2)) error stop 'ERROR: %same_file'
if (.not. same_file(p1%path(), p2%path())) error stop 'ERROR: same_file()'

if (.not. same_file("~", "~")) error stop 'ERROR: same_file(~,~)'

if(shaky /= 0) then
  print '(a)', "SKIP: test_same shaky tests"
  return
endif

if(same_file("not-exist-same", "not-exist-same")) error stop 'ERROR: same_file(not-exist-same, not-exist-same)'


end subroutine test_same_file

end program
