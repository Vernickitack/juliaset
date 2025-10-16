#!/bin/sh

rm -rf build
mkdir build
cd build || exit 1

QT5_PATH="C:\Qt\5.15.2\msvc2019_64"
Qt5_DIR="$QT5_PATH/lib/cmake/Qt5"

cmake -G "Visual Studio 16 2019" -A x64 .. > /dev/null
cmake --build . --config release > /dev/null

mkdir -p release/shaders
cp -r ../src/app/shaders/* release/shaders/

cp "$QT5_PATH\bin\Qt5Core.dll" release/
cp "$QT5_PATH\bin\Qt5Widgets.dll" release/
cp "$QT5_PATH\bin\Qt5Gui.dll" release/
cp "$QT5_PATH\bin\Qt5OpenGL.dll" release/

mkdir release/platforms
cp "$QT5_PATH\plugins\platforms\qwindows.dll" release/platforms/
cp "$QT5_PATH\bin\opengl32sw.dll" release/

cd release
./juliaset.exe