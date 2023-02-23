message(STATUS "${PROJECT_NAME} ${PROJECT_VERSION} CMake ${CMAKE_VERSION} Toolchain ${CMAKE_TOOLCHAIN_FILE}")

option(cpp "Use C++ filesystem for full functionality" on)
option(fortran "use the Fortran interaces to C functions" on)
option(cli "Build CLI" on)
option(BUILD_SHARED_LIBS "Build shared libraries" on)
option(fallback "Fallback to non-C++ filesystem.c if C++ stdlib is not working" on)

option(coverage "Code coverage tests")
option(tidy "Run clang-tidy on the code")


include(GNUInstallDirs)

# Rpath options necessary for shared library install to work correctly in user projects
set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR})
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

# Necessary for shared library with Visual Studio / Windows oneAPI
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

file(GENERATE OUTPUT .gitignore CONTENT "*")
