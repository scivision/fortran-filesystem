submodule (pathlib) pathlib_intel

implicit none (type, external)

contains

module procedure is_directory

type(path) :: p

p = self%expanduser()

inquire(directory=p%path, exist=is_directory)

end procedure is_directory

end submodule pathlib_intel
