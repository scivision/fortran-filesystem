REM batch file since Github Actions doesn't support shell cmd well,
REM and cmd is needed for oneAPI Windows

call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"

echo "configure"
cmake --preset multi -DCMAKE_INSTALL_PREFIX=%RUNNER_TEMP%
if %errorlevel% neq 0 (
  type build\CMakeFiles\CMakeError.log & exit /b %errorlevel%
)

echo "Release build"
cmake --build --preset release
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Release unit test"
ctest --preset release --schedule-random -V
if %errorlevel% neq 0 exit /b %errorlevel%

echo "install project"
cmake --install build
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Debug build"
cmake --build --preset debug
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Debug unit test"
ctest --preset debug --schedule-random -V
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Example config"
cmake -B example/build -S example -DCMAKE_PREFIX_PATH=%RUNNER_TEMP%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Example build"
cmake --build example/build
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Example test"
ctest --test-dir example/build -V
if %errorlevel% neq 0 exit /b %errorlevel%
