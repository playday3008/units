#include <catch2/catch_test_macros.hpp>
#include <example/example.hpp>

TEST_CASE("Example", "[example]") {
  REQUIRE(example::who == "world");
}
