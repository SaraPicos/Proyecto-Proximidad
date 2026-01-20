# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sara/pico_projects/proximidad/build/_deps/picotool-src"
  "/home/sara/pico_projects/proximidad/build/_deps/picotool-build"
  "/home/sara/pico_projects/proximidad/build/_deps"
  "/home/sara/pico_projects/proximidad/build/picotool/tmp"
  "/home/sara/pico_projects/proximidad/build/picotool/src/picotoolBuild-stamp"
  "/home/sara/pico_projects/proximidad/build/picotool/src"
  "/home/sara/pico_projects/proximidad/build/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sara/pico_projects/proximidad/build/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sara/pico_projects/proximidad/build/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
