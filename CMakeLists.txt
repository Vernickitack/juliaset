cmake_minimum_required(VERSION 3.10)
project(juliaset)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

set(Qt5_DIR "C:/Qt/5.15.2/winrt_x64_msvc2019/lib/cmake/Qt5" CACHE PATH "Initial cache" FORCE)

find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)

set(SOURCES
        src/app/main.cpp
        src/app/Window.cpp
        src/base/GLWidget.cpp)

qt5_wrap_cpp(MOC_SOURCES src/app/Window.h src/base/GLWidget.h)

add_executable(juliaset ${SOURCES} ${MOC_SOURCES})

target_link_libraries(juliaset
        Qt5::Core
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL::GL
        )

target_link_libraries(juliaset Qt5::Core Qt5::Widgets Qt5::OpenGL)

target_include_directories(juliaset PRIVATE src)