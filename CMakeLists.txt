cmake_minimum_required(VERSION 3.29)
include(FetchContent)
project(Explorer)

set(CMAKE_CXX_STANDARD 20)
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)

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
        Command.hpp
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
        tileson.hpp
        TileMap2DScene.cpp
        TileMap2DScene.hpp
        TileMapLoader.cpp
        TileMapLoader.hpp
        pocketlzma.hpp
        TrackingCamera2D.cpp
        TrackingCamera2D.hpp
        TileMap2D.cpp
        TileMap2D.hpp
        Common.hpp
        CharacterActor.hpp
        CharacterActor.cpp
        DrawPhysics.hpp
        DrawPhysics.cpp
)
# Adding raylib-cpp (raylib c++ wrapper)
add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/raylib-cpp)
target_link_libraries(Explorer PRIVATE raylib_cpp)

# raygui header file
target_include_directories(Explorer PRIVATE ${CMAKE_SOURCE_DIR}/ThirdParty/raygui)


#target_link_libraries(Explorer raylib ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK} ${GLUT_FRAMEWORK} ${COREVIDEO_FRAMEWORK})
target_link_libraries(Explorer PRIVATE raylib box2d ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK})