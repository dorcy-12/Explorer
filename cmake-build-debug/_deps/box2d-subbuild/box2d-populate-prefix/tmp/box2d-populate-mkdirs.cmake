# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-src"
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-build"
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix"
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix/tmp"
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix/src/box2d-populate-stamp"
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix/src"
  "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix/src/box2d-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix/src/box2d-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/hakizimanaemmanuel/CLionProjects/Explorer/cmake-build-debug/_deps/box2d-subbuild/box2d-populate-prefix/src/box2d-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
