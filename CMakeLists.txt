cmake_minimum_required (VERSION 3.21)
project(sudokurse VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp src/game.cpp src/sudoku.cpp src/stopwatch.cpp)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARY})

