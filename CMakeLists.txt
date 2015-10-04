cmake_minimum_required (VERSION 2.6)
project (StopLearn)

file(GLOB project_SRC
    "*.h"
    "*.cpp"
    "*.c"
)

# set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -O3 -Wextra -Werror -Wpedantic -g -std=c++11")
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -g -std=c++11 -Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded")

add_executable(StopLearn ${project_SRC})
