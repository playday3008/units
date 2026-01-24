// SPDX-License-Identifier: MIT

// Runtime tests for affine quantities (quantity_point) - temperature conversions

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <units/units.hpp>

using namespace units;
using namespace units::si;

TEST_CASE("Celsius to Kelvin conversions", "[temperature][affine]") {
  SECTION("Freezing point of water") {
    auto celsius = celsius_pt(0.0);
    auto kelvin  = to_kelvin(celsius);
    REQUIRE_THAT(kelvin.value(), Catch::Matchers::WithinAbs(273.15, 1e-10));
  }

  SECTION("Boiling point of water") {
    auto celsius = celsius_pt(100.0);
    auto kelvin  = to_kelvin(celsius);
    REQUIRE_THAT(kelvin.value(), Catch::Matchers::WithinAbs(373.15, 1e-10));
  }

  SECTION("Absolute zero") {
    auto celsius = celsius_pt(-273.15);
    auto kelvin  = to_kelvin(celsius);
    REQUIRE_THAT(kelvin.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Room temperature") {
    auto celsius = celsius_pt(20.0);
    auto kelvin  = to_kelvin(celsius);
    REQUIRE_THAT(kelvin.value(), Catch::Matchers::WithinAbs(293.15, 1e-10));
  }
}

TEST_CASE("Kelvin to Celsius conversions", "[temperature][affine]") {
  SECTION("Freezing point of water") {
    auto kelvin  = kelvin_pt(273.15);
    auto celsius = to_celsius(kelvin);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Boiling point of water") {
    auto kelvin  = kelvin_pt(373.15);
    auto celsius = to_celsius(kelvin);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }

  SECTION("Absolute zero") {
    auto kelvin  = kelvin_pt(0.0);
    auto celsius = to_celsius(kelvin);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(-273.15, 1e-10));
  }
}

TEST_CASE("Fahrenheit to Celsius conversions", "[temperature][affine]") {
  SECTION("Freezing point of water") {
    auto fahrenheit = fahrenheit_pt(32.0);
    auto celsius    = to_celsius(fahrenheit);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Boiling point of water") {
    auto fahrenheit = fahrenheit_pt(212.0);
    auto celsius    = to_celsius(fahrenheit);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }

  SECTION("Body temperature (98.6 F)") {
    auto fahrenheit = fahrenheit_pt(98.6);
    auto celsius    = to_celsius(fahrenheit);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(37.0, 0.01));
  }

  SECTION("-40 degrees (same in both scales)") {
    auto fahrenheit = fahrenheit_pt(-40.0);
    auto celsius    = to_celsius(fahrenheit);
    REQUIRE_THAT(celsius.value(), Catch::Matchers::WithinAbs(-40.0, 1e-10));
  }
}

TEST_CASE("Celsius to Fahrenheit conversions", "[temperature][affine]") {
  SECTION("Freezing point of water") {
    auto celsius    = celsius_pt(0.0);
    auto fahrenheit = to_fahrenheit(celsius);
    REQUIRE_THAT(fahrenheit.value(), Catch::Matchers::WithinAbs(32.0, 1e-10));
  }

  SECTION("Boiling point of water") {
    auto celsius    = celsius_pt(100.0);
    auto fahrenheit = to_fahrenheit(celsius);
    REQUIRE_THAT(fahrenheit.value(), Catch::Matchers::WithinAbs(212.0, 1e-10));
  }

  SECTION("Room temperature (20 C)") {
    auto celsius    = celsius_pt(20.0);
    auto fahrenheit = to_fahrenheit(celsius);
    REQUIRE_THAT(fahrenheit.value(), Catch::Matchers::WithinAbs(68.0, 1e-10));
  }
}

TEST_CASE("Fahrenheit to Kelvin conversions", "[temperature][affine]") {
  SECTION("Freezing point of water") {
    auto fahrenheit = fahrenheit_pt(32.0);
    auto kelvin     = to_kelvin(fahrenheit);
    REQUIRE_THAT(kelvin.value(), Catch::Matchers::WithinAbs(273.15, 1e-10));
  }

  SECTION("Absolute zero (approx -459.67 F)") {
    auto fahrenheit = fahrenheit_pt(-459.67);
    auto kelvin     = to_kelvin(fahrenheit);
    REQUIRE_THAT(kelvin.value(), Catch::Matchers::WithinAbs(0.0, 0.01));
  }
}

TEST_CASE("Temperature round-trip conversions", "[temperature][affine][precision]") {
  SECTION("Celsius -> Kelvin -> Celsius") {
    auto original = celsius_pt(25.0);
    auto kelvin   = to_kelvin(original);
    auto back     = to_celsius(kelvin);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }

  SECTION("Fahrenheit -> Celsius -> Fahrenheit") {
    auto original = fahrenheit_pt(72.0);
    auto celsius  = to_celsius(original);
    auto back     = to_fahrenheit(celsius);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }

  SECTION("Fahrenheit -> Kelvin -> Fahrenheit") {
    auto original = fahrenheit_pt(451.0);
    auto kelvin   = to_kelvin(original);
    auto back     = to_fahrenheit(kelvin);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }

  SECTION("Full cycle: C -> F -> K -> C") {
    auto celsius    = celsius_pt(100.0);
    auto fahrenheit = to_fahrenheit(celsius);
    auto kelvin     = to_kelvin(fahrenheit);
    auto back       = to_celsius(kelvin);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinRel(celsius.value(), 1e-12));
  }
}

TEST_CASE("Temperature differences", "[temperature][affine]") {
  SECTION("Temperature difference in Celsius") {
    auto t1   = celsius_pt(100.0);
    auto t2   = celsius_pt(0.0);
    auto diff = t1 - t2;
    REQUIRE_THAT(diff.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }

  SECTION("Temperature difference in Kelvin") {
    auto t1   = kelvin_pt(373.15);
    auto t2   = kelvin_pt(273.15);
    auto diff = t1 - t2;
    REQUIRE_THAT(diff.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }
}
