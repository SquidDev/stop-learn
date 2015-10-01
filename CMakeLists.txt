cmake_minimum_required (VERSION 2.6)
project (StopLearn)

file(GLOB project_SRC
    "*.h"
    "*.c"
)

# set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -O3 -Wextra -Werror -Wpedantic -g -std=c11")
set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-O3 -g -std=c11 -Weverything -Werror -Wno-padded")

add_executable(StopLearn ${project_SRC})
