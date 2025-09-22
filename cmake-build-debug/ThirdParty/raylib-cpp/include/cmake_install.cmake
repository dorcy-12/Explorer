# Install script for directory: /Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/AudioDevice.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/AudioStream.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/AutomationEventList.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/BoundingBox.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Camera2D.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Camera3D.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Color.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/FileData.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/FileText.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Font.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Functions.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Gamepad.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Image.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Keyboard.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Material.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Matrix.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Mesh.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/MeshUnmanaged.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Model.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/ModelAnimation.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Mouse.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Music.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Ray.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/RayCollision.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/RaylibException.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/raylib-cpp-utils.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/raylib-cpp.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/raylib.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/raymath.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Rectangle.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/RenderTexture.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/ShaderUnmanaged.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Shader.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Sound.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Text.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Texture.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/TextureUnmanaged.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Touch.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Vector2.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Vector3.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Vector4.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/VrStereoConfig.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Wave.hpp"
    "/Users/hakizimanaemmanuel/CLionProjects/Explorer/ThirdParty/raylib-cpp/include/Window.hpp"
    )
endif()

