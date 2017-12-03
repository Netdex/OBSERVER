cmake_minimum_required(VERSION 3.6)
project(observer)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror")

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES "src/*.c")

add_executable(observer ${SOURCE_FILES})