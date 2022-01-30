
REM workaround since Github Actions doesn't support shell cmd well,
REM and cmd is needed for oneAPI Windows

call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"

REM doesn't work
@REM call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files (x86)\Intel\oneAPI\compiler\latest\env\vars.bat"

REM configure
cmake --preset multi
if %errorlevel% neq 0 exit /b %errorlevel%

REM Release build
cmake --build --preset release
if %errorlevel% neq 0 (
  type build\CMakeFiles\CMakeError.log & exit /b %errorlevel%
)
REM Release unit test
ctest --preset release --schedule-random -V
if %errorlevel% neq 0 exit /b %errorlevel%

REM debug build
cmake --build --preset debug
if %errorlevel% neq 0 exit /b %errorlevel%
REM debug unit test
ctest --preset debug --schedule-random -V
if %errorlevel% neq 0 exit /b %errorlevel%

REM example config
cmake -B examples/build -S examples
if %errorlevel% neq 0 exit /b %errorlevel%

REM example build
cmake --build examples/build
if %errorlevel% neq 0 exit /b %errorlevel%

REM example test
ctest --test-dir examples/build -V
if %errorlevel% neq 0 exit /b %errorlevel%
