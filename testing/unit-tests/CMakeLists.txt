
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.0
)

FetchContent_MakeAvailable(Catch2)

# Create test executables

set(GENERAL_PLAY_MAN_UNIT_TEST_EXECUTABLE general-play-man-unit-tests)
add_executable(${GENERAL_PLAY_MAN_UNIT_TEST_EXECUTABLE}
	settings/PlayManSettingsTests.cpp
)
target_link_libraries(${GENERAL_PLAY_MAN_UNIT_TEST_EXECUTABLE} PRIVATE ${LIBRARY_NAME})
target_link_libraries(${GENERAL_PLAY_MAN_UNIT_TEST_EXECUTABLE} PRIVATE Catch2::Catch2WithMain)

set(GAMEBOY_UNIT_TEST_EXECUTABLE gameboy-unit-tests)
add_executable(${GAMEBOY_UNIT_TEST_EXECUTABLE}
	gameboy/InstructionTests.cpp
)
target_link_libraries(${GAMEBOY_UNIT_TEST_EXECUTABLE} PRIVATE ${LIBRARY_NAME})
target_link_libraries(${GAMEBOY_UNIT_TEST_EXECUTABLE} PRIVATE Catch2::Catch2WithMain)

set(UTILITY_UNIT_TEST_EXECUTABLE utility-unit-tests)
add_executable(${UTILITY_UNIT_TEST_EXECUTABLE}
	utility/EnumMacro.cpp 
	utility/MetaUtilityTests.cpp
	utility/UtilFuncTests.cpp
	logger/LoggerTests.cpp
	signal/SignalTests.cpp
	issueHandler/IssueHandlerTests.cpp
	containers/EnumIndexableArrayTests.cpp
)
target_link_libraries(${UTILITY_UNIT_TEST_EXECUTABLE} PRIVATE ${LIBRARY_NAME})
target_link_libraries(${UTILITY_UNIT_TEST_EXECUTABLE} PRIVATE Catch2::Catch2WithMain)

set(UNIT_TEST_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin/unit-tests)

# Copy unit test test data
add_custom_target(
	copy_test_data ALL

	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_LIST_DIR}/../test-data/
		${UNIT_TEST_INSTALL_DIR}/test-data/

	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_LIST_DIR}/../test-data/
		${PROJECT_BINARY_DIR}/testing/test-data/
)

# Enable Catch2's CMake testing integration
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(
	${GENERAL_PLAY_MAN_UNIT_TEST_EXECUTABLE}
	${GAMEBOY_UNIT_TEST_EXECUTABLE}
	${UTILITY_UNIT_TEST_EXECUTABLE}
)

install(
	TARGETS ${GENERAL_PLAY_MAN_UNIT_TEST_EXECUTABLE}
	RUNTIME DESTINATION ${UNIT_TEST_INSTALL_DIR}/play-man/
)

install(
	TARGETS ${GAMEBOY_UNIT_TEST_EXECUTABLE}
    RUNTIME DESTINATION ${UNIT_TEST_INSTALL_DIR}/gameboy/
)

install(
	TARGETS ${UTILITY_UNIT_TEST_EXECUTABLE}
    RUNTIME DESTINATION ${UNIT_TEST_INSTALL_DIR}/utility/
)
