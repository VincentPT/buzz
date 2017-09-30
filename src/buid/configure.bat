setlocal
@echo off

SET CURR_DIR=%cd%
SET THIS_SCRIPT_PATH=%~dp0
cd %THIS_SCRIPT_PATH%

cmake -G "Visual Studio 14 2015 Win64" ^
        -DCINDER_INCLUDE_DIR=E:/Projects/externals/cinder/include -DCINDER_LIB_DIR=E:/Projects/externals/cinder/lib/vc14/x64 ^
        -DOPENCV_INCLUDE_DIR=E:/working/trunk/3rdparties/windows/opencv-3.1.0/include -DOPENCV_LIB_DIR=E:/working/trunk/3rdparties/windows/opencv-3.1.0/vc14/lib/x64 ^
        ..
cd %CURR_DIR%

endlocal