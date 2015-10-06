cmake_minimum_required (VERSION 2.6)
project (StopLearn)

include_directories(${CMAKE_SOURCE_DIR}/scoring)

set (project_SRC
    "main.cpp"
    "position.h" "position.cpp"
    "board.h" "board.cpp"
    "scoring/controller.h" "scoring/controller.cpp"
    "scoring/floodFill.h" "scoring/floodFill.cpp"
    "scoring/scoring.h"
    "moving.h" "moving.cpp"
)

# set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -O3 -Wextra -Werror -Wpedantic -g -std=c++11")
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -g -std=c++11 -Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded")

add_executable(StopLearn ${project_SRC})
