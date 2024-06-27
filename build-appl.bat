@echo off
set BAT_BASE_PATH=%~dp0
set PRJ_BASE_PATH=%BAT_BASE_PATH%
set BIN_PATH=%PRJ_BASE_PATH%\bin\appl
set PRJ_PATH=%PRJ_BASE_PATH%\appl

RMDIR /s /q %BIN_PATH%
MD "%BIN_PATH%"
clang ^
    -I"%PRJ_PATH%\include" ^
    -I"%PRJ_PATH%\libraries\sdl2\include" ^
    -I"%PRJ_PATH%\libraries\sdl2_image\include" ^
    -L"%PRJ_PATH%\libraries\sdl2\lib\x64" ^
    -L"%PRJ_PATH%\libraries\sdl2_image\lib\x64" ^
    -lSDL2 ^
    -lSDL2_image ^
    -o "%BIN_PATH%\appl.exe" "%PRJ_PATH%\src\*.c"

IF %ERRORLEVEL% EQU 0 (
    COPY /Y "%PRJ_PATH%\libraries\sdl2\lib\x64\*.dll" "%BIN_PATH%"
    COPY /Y "%PRJ_PATH%\libraries\sdl2_image\lib\x64\*.dll" "%BIN_PATH%"
    XCOPY /E /Q /Y "%PRJ_PATH%\resources\" "%BIN_PATH%\resources\"
)