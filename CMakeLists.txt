cmake_minimum_required(VERSION 3.22.1)

#
# Project Settings
#

# Project name
project(p VERSION 0.0.1 LANGUAGES C CXX)

#
# Compiler Settings
#

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add compile options
set(COMPILE_OPTIONS -Wall -Wextra -Werror -pedantic -O3 -march=native)
add_compile_options(${COMPILE_OPTIONS})

#
# Sources
#

# Define source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/includes)

# Add include directories
include_directories(${INCLUDE_DIR})

set(LIBRARY_NAME play-man.lib)
set(EXECUTABLE_NAME play-man)

# Collect all source files
file(GLOB_RECURSE LIB_SOURCES "${LIB_DIR}/*.cpp")

#
# Create Targets
#

# Add the library target (static library in this case)
add_library(${LIBRARY_NAME} STATIC ${LIB_SOURCES})

# Specify include directories for the library
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)

# Create the executable target
add_executable(${EXECUTABLE_NAME} src/main.cpp)

# Link the executable to the library
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARY_NAME})

#
# Unit testing
#

# Add subdirectory for tests
add_subdirectory(unit-tests)

#
# Install targets in other directory
#

install(
	TARGETS ${EXECUTABLE_NAME}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
)
