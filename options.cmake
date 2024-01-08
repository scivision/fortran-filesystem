option(ffilesystem_cpp "Use C++ filesystem for full functionality" on)
option(ffilesystem_fortran "use the Fortran interaces to C functions" on)
option(ffilesystem_cli "Build CLI" on)
option(ffilesystem_fallback "Fallback to non-C++ filesystem.c if C++ stdlib is not working")

option(BUILD_SHARED_LIBS "Build shared libraries")
option(coverage "Code coverage tests")
option(tidy "Run clang-tidy on the code")
option(cppcheck "Run cppcheck on the code")
option(iwyu "Run include-what-you-use on the code")

option(CMAKE_TLS_VERIFY "Verify TLS certificates" on)


option(${PROJECT_NAME}_BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL})

if(PROJECT_IS_TOP_LEVEL)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/local" CACHE PATH "default install loc" FORCE)
  endif()
endif()

# Necessary for shared library with Visual Studio / Windows oneAPI
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

file(GENERATE OUTPUT .gitignore CONTENT "*")
