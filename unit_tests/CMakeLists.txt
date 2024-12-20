
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.0
)

FetchContent_MakeAvailable(Catch2)

# Creates a test executable
set(GAMEBOY_UNIT_TEST_EXECUTABLE gameboy_unit_tests)

add_executable(${GAMEBOY_UNIT_TEST_EXECUTABLE} gameboy/Test.cpp)

target_link_libraries(${GAMEBOY_UNIT_TEST_EXECUTABLE} PRIVATE ${LIBRARY_NAME})
target_link_libraries(${GAMEBOY_UNIT_TEST_EXECUTABLE} PRIVATE Catch2::Catch2WithMain)

# Enable Catch2's CMake testing integration
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${GAMEBOY_UNIT_TEST_EXECUTABLE})

install(
	TARGETS ${GAMEBOY_UNIT_TEST_EXECUTABLE}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/unit_tests/gameboy/
)
