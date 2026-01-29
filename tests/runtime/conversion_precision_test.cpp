// SPDX-License-Identifier: MIT

// Runtime tests for conversion precision and round-trip accuracy

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <units/units.hpp>

using namespace units;
using namespace units::si;
using namespace units::si::literals;

// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Length unit round-trip conversions", "[conversion][precision]") {
  SECTION("Metres to kilometres and back") {
    constexpr auto original  = 1234.5_m;
    auto           in_km     = original.in(kilometre);
    auto           back_to_m = in_km.in(metre);
    REQUIRE_THAT(back_to_m.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }

  SECTION("Metres to centimetres and back") {
    constexpr auto original  = 1234.5_m;
    auto           in_cm     = original.in(centimetre);
    auto           back_to_m = in_cm.in(metre);
    REQUIRE_THAT(back_to_m.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }

  SECTION("Metres to millimetres and back") {
    constexpr auto original  = 1234.5_m;
    auto           in_mm     = original.in(millimetre);
    auto           back_to_m = in_mm.in(metre);
    REQUIRE_THAT(back_to_m.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }

  SECTION("Large values") {
    constexpr auto original  = 1.0e12_m;
    auto           in_km     = original.in(kilometre);
    auto           back_to_m = in_km.in(metre);
    REQUIRE_THAT(back_to_m.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }

  SECTION("Small values") {
    constexpr auto original  = 1.0e-9_m;
    auto           in_mm     = original.in(millimetre);
    auto           back_to_m = in_mm.in(metre);
    REQUIRE_THAT(back_to_m.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }
}

TEST_CASE("Time unit round-trip conversions", "[conversion][precision]") {
  SECTION("Seconds to milliseconds and back") {
    constexpr auto original  = 123.456_s;
    auto           in_ms     = original.in(millisecond);
    auto           back_to_s = in_ms.in(second);
    REQUIRE_THAT(back_to_s.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }
}

TEST_CASE("Mass unit round-trip conversions", "[conversion][precision]") {
  SECTION("Kilograms to grams and back") {
    constexpr auto original   = 12.345_kg;
    auto           in_g       = original.in(gram);
    auto           back_to_kg = in_g.in(kilogram);
    REQUIRE_THAT(back_to_kg.value(), Catch::Matchers::WithinRel(original.value(), 1e-15));
  }
}

TEST_CASE("Imperial unit conversions", "[conversion][precision][imperial]") {
  using namespace units::imperial;

  SECTION("Feet to metres and back") {
    auto original   = make_quantity(100.0, foot);
    auto in_m       = original.in(metre);
    auto back_to_ft = in_m.in(foot);
    REQUIRE_THAT(back_to_ft.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }

  SECTION("Miles to metres and back") {
    auto original   = make_quantity(26.2, mile);
    auto in_m       = original.in(metre);
    auto back_to_mi = in_m.in(mile);
    REQUIRE_THAT(back_to_mi.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }

  SECTION("Pounds to kilograms and back") {
    auto original   = make_quantity(150.0, pound);
    auto in_kg      = original.in(kilogram);
    auto back_to_lb = in_kg.in(pound);
    REQUIRE_THAT(back_to_lb.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }
}

TEST_CASE("CGS unit conversions", "[conversion][precision][cgs]") {
  using namespace units::cgs;

  SECTION("Dynes to newtons and back") {
    auto original     = make_quantity(1000.0, dyne);
    auto in_n         = original.in(newton);
    auto back_to_dyne = in_n.in(dyne);
    REQUIRE_THAT(back_to_dyne.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }

  SECTION("Ergs to joules and back") {
    auto original    = make_quantity(1.0e7, erg);
    auto in_j        = original.in(joule);
    auto back_to_erg = in_j.in(erg);
    REQUIRE_THAT(back_to_erg.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }
}

TEST_CASE("Natural unit conversions", "[conversion][precision][natural]") {
  using namespace units::natural;

  SECTION("eV to keV to MeV to GeV and back") {
    auto original = make_quantity(1.0e9, electronvolt);
    auto in_kev   = original.in(kiloelectronvolt);
    auto in_mev   = in_kev.in(megaelectronvolt);
    auto in_gev   = in_mev.in(gigaelectronvolt);
    auto back     = in_gev.in(electronvolt);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinRel(original.value(), 1e-12));
  }
}

TEST_CASE("Derived unit conversions", "[conversion][precision]") {
  SECTION("Speed calculation") {
    constexpr auto distance = 100.0_m;
    constexpr auto time     = 9.58_s;
    auto           speed    = distance / time;

    // Verify speed value is correct
    REQUIRE_THAT(speed.value(), Catch::Matchers::WithinRel(100.0 / 9.58, 1e-12));
  }

  SECTION("Speed in different units") {
    constexpr auto distance = 1000.0_m;
    constexpr auto time     = 100.0_s;
    auto           speed    = distance / time; // 10 m/s

    REQUIRE_THAT(speed.value(), Catch::Matchers::WithinRel(10.0, 1e-12));

    // Convert to km/s by converting distance
    auto distance_km = distance.in(kilometre);
    auto speed_km_s  = distance_km / time;

    REQUIRE_THAT(speed_km_s.value(), Catch::Matchers::WithinRel(0.01, 1e-12));
  }

  SECTION("Area calculation") {
    constexpr auto length = 10.0_m;
    constexpr auto width  = 5.0_m;
    auto           area   = length * width;

    REQUIRE_THAT(area.value(), Catch::Matchers::WithinAbs(50.0, 1e-10));
  }
}

// NOLINTEND(readability-function-cognitive-complexity)
