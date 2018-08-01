cmake_minimum_required(VERSION 3.5.1)
project(announce)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(BASE_COMPILE_OPTIONS -Wall -Wextra -pedantic -pipe -march=native -fopenmp )
set(DEBUG_OPTIONS ${BASE_COMPILE_OPTIONS} -g -O0)
set(RELEASE_OPTIONS ${BASE_COMPILE_OPTIONS} -O3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

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
    src/interactive.cpp
    src/interactive.h
    src/UI/add_entry_dialog.cpp
    src/UI/add_entry_dialog.h
    src/UI/add_entry_dialog.ui
    src/UI/mainwindow.h
    src/UI/mainwindow.cpp
    src/UI/mainwindow.ui
)

add_custom_target(glucose COMMAND make r
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sat/parallel
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/belief_revise)

add_executable(announce ${ANNOUNCE_SRCS})

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

target_compile_options(announce PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
target_compile_options(announce PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

target_include_directories(announce PRIVATE
    src
)

add_dependencies(announce glucose)

target_link_libraries(announce Qt5::Widgets)

