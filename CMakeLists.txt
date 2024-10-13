cmake_minimum_required(VERSION 3.29)
project(Explorer)

set(CMAKE_CXX_STANDARD 20)
set(RAYGUI_DIR "${CMAKE_SOURCE_DIR}/raygui-master")

#include raygui header
include_directories(${RAYGUI_DIR}/src)

#find raylib
find_package(raylib REQUIRED)

# Link necessary macOS frameworks using CMake's framework support
find_library(COCOA_FRAMEWORK Cocoa)
find_library(IOKIT_FRAMEWORK IOKit)
find_library(OPENGL_FRAMEWORK OpenGL)
#find_library(COREVIDEO_FRAMEWORK CoreVideo)
#find_library(GLUE_FRAMEWORK GLUT)


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
        MainMenuScene.cpp
        MainMenuScene.hpp
)

#target_link_libraries(Explorer raylib ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK} ${GLUT_FRAMEWORK} ${COREVIDEO_FRAMEWORK})
target_link_libraries(Explorer raylib ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK})