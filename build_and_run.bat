@echo off
echo Building juliaset...

if exist build rmdir /s /q build
mkdir build
cd build

set QT5_PATH=C:\Qt\5.15.2\msvc2019_64
set Qt5_DIR=%QT5_PATH%\lib\cmake\Qt5

cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release

echo Build successful!

if not exist "Release\shaders" mkdir Release\shaders
xcopy /y /s "..\src\app\shaders" "Release\shaders\"

copy "%QT5_PATH%\bin\Qt5Core.dll" Release\
copy "%QT5_PATH%\bin\Qt5Widgets.dll" Release\
copy "%QT5_PATH%\bin\Qt5Gui.dll" Release\
copy "%QT5_PATH%\bin\Qt5OpenGL.dll" Release\

if not exist "Release\platforms" mkdir Release\platforms
copy "%QT5_PATH%\plugins\platforms\qwindows.dll" Release\platforms\

copy "%QT5_PATH%\bin\opengl32sw.dll" Release\ 2>nul

echo.
echo Starting juliaset...
cd Release
juliaset.exe

pause