cmake_minimum_required(VERSION 3.5)

project("Swept Volumes")

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB_RECURSE SOURCE src/*)

list(REMOVE_ITEM SOURCE src/main.cpp)

add_library(swept_volumes_LIB SHARED ${SOURCE})

add_executable(swept_volumes src/main.cpp)

target_link_libraries(swept_volumes swept_volumes_LIB)
