cmake_minimum_required(VERSION 3.29)
project(Explorer)

set(CMAKE_CXX_STANDARD 20)
find_package(raylib REQUIRED)

# Link necessary macOS frameworks using CMake's framework support
find_library(COCOA_FRAMEWORK Cocoa)
find_library(IOKIT_FRAMEWORK IOKit)
find_library(OPENGL_FRAMEWORK OpenGL)


add_executable(Explorer main.cpp
        InputHandler.cpp
        InputHandler.hpp
        Command.cpp
        Command.h
        Scene.cpp
        Scene.hpp
        Actor.hpp
        Scarfy.cpp
        Scarfy.hpp
        Actor.cpp
        ScarfyScene.cpp
        ScarfyScene.hpp
)

target_link_libraries(Explorer raylib ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK})
