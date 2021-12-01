submodule (pathlib) pathlib_intel

implicit none (type, external)

contains

module procedure is_directory

inquire(directory=expanduser(path), exist=exists)

end procedure is_directory

end submodule pathlib_intel
