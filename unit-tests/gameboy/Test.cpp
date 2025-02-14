
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/test.hpp"

// Define a test case
TEST_CASE("Unit tests work correctly")
{
    REQUIRE(SomeTest::GetValue() == 42);
}
