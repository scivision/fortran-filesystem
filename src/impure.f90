submodule (pathlib) impure_pathlib

implicit none (type, external)

contains

module procedure unlink
integer :: u

open(newunit=u, file=self%path_str, status='old')
close(u, status='delete')

end procedure unlink


module procedure resolve
resolve = self%expanduser()
resolve%path_str = canonical(resolve%path_str)
end procedure resolve


module procedure same_file
type(path_t) :: r1, r2

r1 = self%resolve()
r2 = other%resolve()
same_file = r1%path_str == r2%path_str
end procedure same_file


module procedure is_file
inquire(file=expanduser(self%path_str), exist=is_file)
if(is_file .and. self%is_dir()) is_file = .false.
end procedure is_file


module procedure pathlib_is_dir
pathlib_is_dir = is_dir(self%path_str)
end procedure pathlib_is_dir


module procedure pathlib_expanduser
ex%path_str = expanduser(self%path_str)
ex = ex%as_posix()
end procedure pathlib_expanduser


module procedure expanduser
character(:), allocatable :: homedir

expanduser = trim(adjustl(path))

if (len(expanduser) < 1) return
if(expanduser(1:1) /= '~') return

homedir = home()
if (len_trim(homedir) == 0) return

if (len_trim(expanduser) < 2) then
  !! ~ alone
  expanduser = homedir
else
  !! ~/...
  expanduser = homedir // expanduser(2:)
endif

end procedure expanduser

end submodule impure_pathlib
