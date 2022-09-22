submodule (filesystem) no_cpp_fs
!! all compilers without C++ filesystem

implicit none

contains


module procedure with_suffix
allocate(character(get_max_path()) :: with_suffix)

if(len_trim(path) > 0) then
  with_suffix = path(:len_trim(path) - len(suffix(path))) // new
else
  with_suffix = ""
endif
end procedure with_suffix


end submodule no_cpp_fs
