// SPDX-License-Identifier: MIT

// Runtime tests for std::format output with quantities

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <units/units.hpp>

#include <format>
#include <numbers>

using namespace units;
using namespace units::si;
using namespace units::si::literals;

// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Basic quantity formatting", "[format]") {
  SECTION("Length quantities") {
    auto distance  = 100.0_m;
    auto formatted = std::format("{}", distance);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("100"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("m"));
  }

  SECTION("Mass quantities") {
    auto mass      = 5.5_kg;
    auto formatted = std::format("{}", mass);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("5.5"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("kg"));
  }

  SECTION("Time quantities") {
    auto time      = 9.58_s;
    auto formatted = std::format("{}", time);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("9.58"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("s"));
  }
}

TEST_CASE("Derived unit formatting", "[format]") {
  SECTION("Speed") {
    auto distance  = 100.0_m;
    auto time      = 10.0_s;
    auto speed     = distance / time;
    auto formatted = std::format("{}", speed);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("10"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("m"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("s"));
  }

  SECTION("Force (Newton)") {
    auto force     = 100.0_N;
    auto formatted = std::format("{}", force);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("100"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("N"));
  }

  SECTION("Energy (Joule)") {
    auto energy    = 1000.0_J;
    auto formatted = std::format("{}", energy);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("1000"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("J"));
  }

  SECTION("Power (Watt)") {
    auto power     = 500.0_W;
    auto formatted = std::format("{}", power);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("500"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("W"));
  }
}

TEST_CASE("Precision formatting", "[format]") {
  SECTION("Fixed precision with .2f") {
    auto value     = make_quantity(std::numbers::pi, metre);
    auto formatted = std::format("{:.2f}", value);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("3.14"));
  }

  SECTION("Fixed precision with .4f") {
    auto value     = make_quantity(std::numbers::pi, metre);
    auto formatted = std::format("{:.4f}", value);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("3.1416"));
  }

  SECTION("Scientific notation with .3e") {
    auto value     = make_quantity(2.998e8, metre);
    auto formatted = std::format("{:.3e}", value);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("2.998"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("e"));
  }
}

TEST_CASE("Width and alignment formatting", "[format]") {
  SECTION("Right-aligned with width") {
    auto value     = 5.0_m;
    auto formatted = std::format("{:>15}", value);
    REQUIRE(formatted.size() >= 15);
  }

  SECTION("Left-aligned with width") {
    auto value     = 5.0_m;
    auto formatted = std::format("{:<15}", value);
    REQUIRE(formatted.size() >= 15);
  }
}

TEST_CASE("Prefixed unit formatting", "[format]") {
  SECTION("Kilometre") {
    auto distance  = 5.0_km;
    auto formatted = std::format("{}", distance);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("5"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("km"));
  }

  SECTION("Millisecond") {
    auto time      = 500.0_ms;
    auto formatted = std::format("{}", time);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("500"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("ms"));
  }

  SECTION("Centimetre") {
    auto length    = 25.0_cm;
    auto formatted = std::format("{}", length);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("25"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("cm"));
  }
}

TEST_CASE("Natural unit formatting", "[format][natural]") {
  using namespace units::natural::literals;

  SECTION("Electronvolt") {
    auto energy    = 125.0_GeV;
    auto formatted = std::format("{}", energy);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("125"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("GeV"));
  }

  SECTION("Fermi") {
    auto length    = 1.0_fm;
    auto formatted = std::format("{}", length);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("1"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("fm"));
  }

  SECTION("Femtobarn") {
    auto cross_section = 100.0_fb;
    auto formatted     = std::format("{}", cross_section);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("100"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("fb"));
  }
}

TEST_CASE("Imperial unit formatting", "[format][imperial]") {
  using namespace units::imperial;

  SECTION("Foot") {
    auto length    = make_quantity(6.0, foot);
    auto formatted = std::format("{}", length);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("6"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("ft"));
  }

  SECTION("Mile per hour") {
    auto speed     = make_quantity(60.0, mile_per_hour);
    auto formatted = std::format("{}", speed);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("60"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("mph"));
  }

  SECTION("Pound") {
    auto mass      = make_quantity(150.0, pound);
    auto formatted = std::format("{}", mass);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("150"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("lb"));
  }
}

TEST_CASE("CGS unit formatting", "[format][cgs]") {
  using namespace units::cgs;

  SECTION("Dyne") {
    auto force     = make_quantity(1000.0, dyne);
    auto formatted = std::format("{}", force);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("1000"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("dyn"));
  }

  SECTION("Erg") {
    auto energy    = make_quantity(1.0e7, erg);
    auto formatted = std::format("{}", energy);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("erg"));
  }

  SECTION("Gauss") {
    auto field     = make_quantity(50.0, gauss);
    auto formatted = std::format("{}", field);
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("50"));
    REQUIRE_THAT(formatted, Catch::Matchers::ContainsSubstring("G"));
  }
}

// NOLINTEND(readability-function-cognitive-complexity)
