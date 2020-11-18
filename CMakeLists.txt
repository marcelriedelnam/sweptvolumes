cmake_minimum_required(VERSION 3.5)

project("Swept Volumes")

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(include)

file(GLOB_RECURSE SOURCE src/*)

list(REMOVE_ITEM SOURCE src/main.cpp)

add_library(swept_volumes_LIB STATIC ${SOURCE})

add_executable(swept_volumes src/main.cpp)

target_link_libraries(swept_volumes swept_volumes_LIB)
