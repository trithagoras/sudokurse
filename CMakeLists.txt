cmake_minimum_required (VERSION 3.21)

# Set the default toolchain to use a Vcpkg submodule.
#if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#    set(CMAKE_TOOLCHAIN_FILE
#        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
#        CACHE STRING "Vcpkg toolchain file")
#endif()

project(sudokurse VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp src/game.cpp src/sudoku.cpp)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARY})

