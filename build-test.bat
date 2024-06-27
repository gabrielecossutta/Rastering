@echo off
set BAT_BASE_PATH=%~dp0
set PRJ_BASE_PATH=%BAT_BASE_PATH%
set BIN_PATH=%PRJ_BASE_PATH%\bin\test
set PRJ_PATH=%PRJ_BASE_PATH%\test
set APPL_PATH=%PRJ_BASE_PATH%\appl
set MESSAGE=Build for test OK

RMDIR /s /q %BIN_PATH%
MD "%BIN_PATH%"

clang ^
    -I"%APPL_PATH%\include" ^
    -I"%APPL_PATH%\libraries\sdl2\include" ^
    -I"%APPL_PATH%\libraries\sdl2_image\include" ^
    -o "%BIN_PATH%\appl.lib" "%APPL_PATH%\src\*.c" -fuse-ld=llvm-lib ^
    -D__TEST_BUILD

IF %ERRORLEVEL% NEQ 0 (
    set MESSAGE=Failure compiling appl as library
    GOTO QUIT
)

clang ^
    -I"%PRJ_PATH%\include" ^
    -I"%APPL_PATH%\include" ^
    -I"%APPL_PATH%\libraries\sdl2\include" ^
    -I"%APPL_PATH%\libraries\sdl2_image\include" ^
    -o "%BIN_PATH%\test.exe" "%PRJ_PATH%\src\*.c" ^
    ^ -L"%BIN_PATH%" -lappl

IF %ERRORLEVEL% NEQ 0 (
    set MESSAGE=Failure compiling test
    GOTO QUIT
)

IF %ERRORLEVEL% EQU 0 (
    XCOPY /E /Q /Y "%PRJ_PATH%\resources\" "%BIN_PATH%\resources\"
)

:QUIT
echo %MESSAGE%