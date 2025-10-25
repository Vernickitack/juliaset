cmake_minimum_required(VERSION 3.10)
project(juliaset)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

set(Qt5_DIR "C:/Qt/5.15.2/winrt_x64_msvc2019/lib/cmake/Qt5" CACHE PATH "Initial cache" FORCE)

find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)

set(SOURCES
        src/app/main.cpp
        src/app/Window.cpp
        src/base/GLWidget.cpp
        src/base/Engine.cpp)

qt5_wrap_cpp(MOC_SOURCES src/app/Window.h src/base/GLWidget.h src/base/Engine.h)

qt5_add_resources(SHADERS src/app/shaders/shaders.qrc)

add_executable(juliaset
        ${SOURCES}
        ${MOC_SOURCES}
        ${SHADERS}
        )

target_compile_options(juliaset PRIVATE /W4 /WX)

target_link_libraries(juliaset
        Qt5::Core
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL::GL
        )

target_link_libraries(juliaset Qt5::Core Qt5::Widgets Qt5::OpenGL)

target_include_directories(juliaset PRIVATE src)