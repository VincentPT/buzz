setlocal
@echo off

SET CURR_DIR=%cd%
SET THIS_SCRIPT_PATH=%~dp0
cd %THIS_SCRIPT_PATH%
del /Q CMakeCache.txt
cmake -G "Visual Studio 14 2015 Win64" -DCINDER_INCLUDE_DIR=E:/Projects/externals/cinder/include -DCINDER_LIB_DIR=E:/Projects/externals/cinder/lib/vc14/x64 ..
cd %CURR_DIR%

endlocal