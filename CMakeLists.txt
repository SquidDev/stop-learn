cmake_minimum_required (VERSION 2.6)
project (StopLearn)

file(GLOB project_SRC
    "*.h"
    "*.c"
)

set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -O3 -Wextra -std=c11")

add_executable(StopLearn ${project_SRC})