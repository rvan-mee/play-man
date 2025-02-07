
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.0
)

FetchContent_MakeAvailable(Catch2)

# Create test executables

set(GAMEBOY_UNIT_TEST_EXECUTABLE gameboy-unit-tests)
add_executable(${GAMEBOY_UNIT_TEST_EXECUTABLE} gameboy/Test.cpp)
target_link_libraries(${GAMEBOY_UNIT_TEST_EXECUTABLE} PRIVATE ${LIBRARY_NAME})
target_link_libraries(${GAMEBOY_UNIT_TEST_EXECUTABLE} PRIVATE Catch2::Catch2WithMain)

set(UTILITY_UNIT_TEST_EXECUTABLE utility_unit_tests)
add_executable(${UTILITY_UNIT_TEST_EXECUTABLE}
	utility/EnumMacro.cpp 
	utility/MetaUtilityTests.cpp
	utility/UtilFuncTests.cpp
	logger/LoggerTests.cpp
	signal/SignalTests.cpp
	issueHandler/IssueHandlerTests.cpp
)
target_link_libraries(${UTILITY_UNIT_TEST_EXECUTABLE} PRIVATE ${LIBRARY_NAME})
target_link_libraries(${UTILITY_UNIT_TEST_EXECUTABLE} PRIVATE Catch2::Catch2WithMain)

# Enable Catch2's CMake testing integration
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${GAMEBOY_UNIT_TEST_EXECUTABLE} ${UTILITY_UNIT_TEST_EXECUTABLE})

install(
	TARGETS ${GAMEBOY_UNIT_TEST_EXECUTABLE}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/unit_tests/gameboy/
)

install(
	TARGETS ${UTILITY_UNIT_TEST_EXECUTABLE}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/unit_tests/Utility/
)
