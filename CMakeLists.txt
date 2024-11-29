cmake_minimum_required(VERSION 3.22.1)

# Project name
project(p VERSION 0.0.1 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add compile options
set(COMPILE_OPTIONS -Wall -Wextra -Werror -pedantic -O3 -march=native)
add_compile_options(${COMPILE_OPTIONS})

# Define source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/includes)

# Add include directories
include_directories(${INCLUDE_DIR})

# Collect all source files
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Add executable or library target
add_executable(play-man ${SOURCES})
