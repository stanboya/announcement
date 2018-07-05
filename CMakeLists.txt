cmake_minimum_required(VERSION 3.5.1)
project(announce)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -pedantic -pipe -march=native -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ANNOUNCE_SRCS
    src/main.cpp
    src/tools.cpp
    src/tools.h
    src/file.cpp
    src/file.h
    src/utils.cpp
    src/utils.h
    src/announce.cpp
    src/announce.h
)

add_custom_target(glucose COMMAND make r
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sat/parallel
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/belief_revise)

add_executable(announce ${ANNOUNCE_SRCS})

target_include_directories(announce PRIVATE
    src
)

add_dependencies(announce glucose)
