// SPDX-License-Identifier: MIT

// Additional runtime tests to improve code coverage
// Focuses on: temperature conversions, quantity converting constructor,
// format edge cases, and remaining math functions

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <format>
#include <string>
#include <units/units.hpp>

#include <cmath>

using namespace units;
using namespace units::si;
using namespace units::si::literals;

// NOLINTBEGIN(readability-function-cognitive-complexity)

// =============================================================================
// Temperature conversion tests - covering all paths in temperature.hpp
// =============================================================================

TEST_CASE("Temperature: Kelvin to Celsius conversions", "[temperature][conversion]") {
  SECTION("Boiling point of water - 373.15 K = 100 C") {
    auto k      = kelvin_pt(373.15);
    auto result = to_celsius(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }

  SECTION("Freezing point of water - 273.15 K = 0 C") {
    auto k      = kelvin_pt(273.15);
    auto result = to_celsius(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Absolute zero - 0 K = -273.15 C") {
    auto k      = kelvin_pt(0.0);
    auto result = to_celsius(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-273.15, 1e-10));
  }

  SECTION("Room temperature - 293.15 K = 20 C") {
    auto k      = kelvin_pt(293.15);
    auto result = to_celsius(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(20.0, 1e-10));
  }
}

TEST_CASE("Temperature: Celsius to Kelvin conversions", "[temperature][conversion]") {
  SECTION("Boiling point of water - 100 C = 373.15 K") {
    auto c      = celsius_pt(100.0);
    auto result = to_kelvin(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(373.15, 1e-10));
  }

  SECTION("Freezing point of water - 0 C = 273.15 K") {
    auto c      = celsius_pt(0.0);
    auto result = to_kelvin(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(273.15, 1e-10));
  }

  SECTION("Absolute zero in Celsius - -273.15 C = 0 K") {
    auto c      = celsius_pt(-273.15);
    auto result = to_kelvin(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Room temperature - 20 C = 293.15 K") {
    auto c      = celsius_pt(20.0);
    auto result = to_kelvin(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(293.15, 1e-10));
  }
}

TEST_CASE("Temperature: Kelvin to Fahrenheit conversions", "[temperature][conversion]") {
  SECTION("Boiling point of water - 373.15 K = 212 F") {
    auto k      = kelvin_pt(373.15);
    auto result = to_fahrenheit(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(212.0, 1e-9));
  }

  SECTION("Freezing point of water - 273.15 K = 32 F") {
    auto k      = kelvin_pt(273.15);
    auto result = to_fahrenheit(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(32.0, 1e-9));
  }

  SECTION("Absolute zero - 0 K = -459.67 F") {
    auto k      = kelvin_pt(0.0);
    auto result = to_fahrenheit(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-459.67, 1e-9));
  }

  SECTION("Room temperature - 293.15 K = 68 F") {
    auto k      = kelvin_pt(293.15);
    auto result = to_fahrenheit(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(68.0, 1e-9));
  }
}

TEST_CASE("Temperature: Fahrenheit to Kelvin conversions", "[temperature][conversion]") {
  SECTION("Boiling point of water - 212 F = 373.15 K") {
    auto f      = fahrenheit_pt(212.0);
    auto result = to_kelvin(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(373.15, 1e-9));
  }

  SECTION("Freezing point of water - 32 F = 273.15 K") {
    auto f      = fahrenheit_pt(32.0);
    auto result = to_kelvin(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(273.15, 1e-9));
  }

  SECTION("Absolute zero - -459.67 F = 0 K") {
    auto f      = fahrenheit_pt(-459.67);
    auto result = to_kelvin(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-9));
  }

  SECTION("0 F = 255.372 K (approximate)") {
    auto f      = fahrenheit_pt(0.0);
    auto result = to_kelvin(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(255.372222222, 1e-6));
  }
}

TEST_CASE("Temperature: Celsius to Fahrenheit conversions", "[temperature][conversion]") {
  SECTION("Boiling point of water - 100 C = 212 F") {
    auto c      = celsius_pt(100.0);
    auto result = to_fahrenheit(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(212.0, 1e-10));
  }

  SECTION("Freezing point of water - 0 C = 32 F") {
    auto c      = celsius_pt(0.0);
    auto result = to_fahrenheit(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(32.0, 1e-10));
  }

  SECTION("Absolute zero in Celsius - -273.15 C = -459.67 F") {
    auto c      = celsius_pt(-273.15);
    auto result = to_fahrenheit(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-459.67, 1e-9));
  }

  SECTION("Body temperature - 37 C = 98.6 F") {
    auto c      = celsius_pt(37.0);
    auto result = to_fahrenheit(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(98.6, 1e-10));
  }

  SECTION("-40 C = -40 F (same value)") {
    auto c      = celsius_pt(-40.0);
    auto result = to_fahrenheit(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-40.0, 1e-10));
  }
}

TEST_CASE("Temperature: Fahrenheit to Celsius conversions", "[temperature][conversion]") {
  SECTION("Boiling point of water - 212 F = 100 C") {
    auto f      = fahrenheit_pt(212.0);
    auto result = to_celsius(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }

  SECTION("Freezing point of water - 32 F = 0 C") {
    auto f      = fahrenheit_pt(32.0);
    auto result = to_celsius(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Absolute zero in Fahrenheit - -459.67 F = -273.15 C") {
    auto f      = fahrenheit_pt(-459.67);
    auto result = to_celsius(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-273.15, 1e-9));
  }

  SECTION("Body temperature - 98.6 F = 37 C") {
    auto f      = fahrenheit_pt(98.6);
    auto result = to_celsius(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(37.0, 1e-10));
  }

  SECTION("-40 F = -40 C (same value)") {
    auto f      = fahrenheit_pt(-40.0);
    auto result = to_celsius(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-40.0, 1e-10));
  }
}

TEST_CASE("Temperature: Round-trip conversions", "[temperature][conversion]") {
  SECTION("K -> C -> K") {
    auto original = kelvin_pt(300.0);
    auto in_c     = to_celsius(original);
    auto back     = to_kelvin(in_c);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinAbs(original.value(), 1e-10));
  }

  SECTION("K -> F -> K") {
    auto original = kelvin_pt(300.0);
    auto in_f     = to_fahrenheit(original);
    auto back     = to_kelvin(in_f);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinAbs(original.value(), 1e-9));
  }

  SECTION("C -> K -> C") {
    auto original = celsius_pt(25.0);
    auto in_k     = to_kelvin(original);
    auto back     = to_celsius(in_k);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinAbs(original.value(), 1e-10));
  }

  SECTION("C -> F -> C") {
    auto original = celsius_pt(25.0);
    auto in_f     = to_fahrenheit(original);
    auto back     = to_celsius(in_f);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinAbs(original.value(), 1e-10));
  }

  SECTION("F -> K -> F") {
    auto original = fahrenheit_pt(77.0);
    auto in_k     = to_kelvin(original);
    auto back     = to_fahrenheit(in_k);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinAbs(original.value(), 1e-9));
  }

  SECTION("F -> C -> F") {
    auto original = fahrenheit_pt(77.0);
    auto in_c     = to_celsius(original);
    auto back     = to_fahrenheit(in_c);
    REQUIRE_THAT(back.value(), Catch::Matchers::WithinAbs(original.value(), 1e-10));
  }
}

// =============================================================================
// Quantity converting constructor tests
// =============================================================================

TEST_CASE("Quantity implicit conversion between compatible units", "[quantity][conversion]") {
  SECTION("km implicitly converts to m") {
    auto                                                      km_val = 1.0_km;
    const quantity<reference<isq::length_t, metre_t>, double> m_val = km_val; // implicit conversion
    REQUIRE_THAT(m_val.value(), Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("mm implicitly converts to m") {
    auto                                                      mm_val = 1000.0_mm;
    const quantity<reference<isq::length_t, metre_t>, double> m_val = mm_val; // implicit conversion
    REQUIRE_THAT(m_val.value(), Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("cm implicitly converts to m") {
    auto                                                      cm_val = 100.0_cm;
    const quantity<reference<isq::length_t, metre_t>, double> m_val = cm_val; // implicit conversion
    REQUIRE_THAT(m_val.value(), Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("g implicitly converts to kg") {
    auto                                                       g_val = 1000.0_g;
    const quantity<reference<isq::mass_t, kilogram_t>, double> kg_val
        = g_val; // implicit conversion
    REQUIRE_THAT(kg_val.value(), Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("ms implicitly converts to s") {
    auto                                                     ms_val = 1000.0_ms;
    const quantity<reference<isq::time_t, second_t>, double> s_val  = ms_val; // implicit conversion
    REQUIRE_THAT(s_val.value(), Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("Multiple conversions in chain") {
    auto                                                      km_val = 2.5_km;
    const quantity<reference<isq::length_t, metre_t>, double> m_val  = km_val;
    REQUIRE_THAT(m_val.value(), Catch::Matchers::WithinAbs(2500.0, 1e-10));

    // Convert back via explicit conversion
    auto back_km = m_val.in(kilometre);
    REQUIRE_THAT(back_km.value(), Catch::Matchers::WithinAbs(2.5, 1e-10));
  }
}

// =============================================================================
// Format tests for edge cases and derived units
// =============================================================================

TEST_CASE("Format: precision specification", "[format]") {
  // NOLINTBEGIN(modernize-use-std-numbers) - using 3.14159 as test value, not pi

  SECTION("0 decimal places") {
    auto q   = make_quantity(3.14159, metre);
    auto str = std::format("{:.0f}", q);
    REQUIRE(str.find("3 m") != std::string::npos);
  }

  SECTION("1 decimal place") {
    auto q   = make_quantity(3.14159, metre);
    auto str = std::format("{:.1f}", q);
    REQUIRE(str.find("3.1 m") != std::string::npos);
  }

  SECTION("2 decimal places") {
    auto q   = make_quantity(3.14159, metre);
    auto str = std::format("{:.2f}", q);
    REQUIRE(str.find("3.14 m") != std::string::npos);
  }

  SECTION("5 decimal places") {
    auto q   = make_quantity(3.14159, metre);
    auto str = std::format("{:.5f}", q);
    REQUIRE(str.find("3.14159 m") != std::string::npos);
  }

  // NOLINTEND(modernize-use-std-numbers)

  SECTION("scientific notation") {
    auto q   = make_quantity(1.23e6, metre);
    auto str = std::format("{:.2e}", q);
    REQUIRE(str.find("1.23e") != std::string::npos);
    REQUIRE(str.find('m') != std::string::npos);
  }
}

TEST_CASE("Format: width specification", "[format]") {
  SECTION("Right-aligned with width") {
    auto q   = 5.0_m;
    auto str = std::format("{:>10}", q);
    // Should have padding
    REQUIRE(str.length() >= 10);
  }

  SECTION("Left-aligned with width") {
    auto q   = 5.0_m;
    auto str = std::format("{:<10}", q);
    REQUIRE(str.length() >= 10);
  }
}

TEST_CASE("Format: derived unit symbols", "[format][derived]") {
  SECTION("Velocity m/s") {
    auto distance = 10.0_m;
    auto time     = 2.0_s;
    auto speed    = distance / time;
    auto str      = std::format("{}", speed);
    REQUIRE(str.find('5') != std::string::npos);
    REQUIRE(str.find('m') != std::string::npos);
    REQUIRE(str.find('s') != std::string::npos);
  }

  SECTION("Acceleration m/s^2") {
    auto distance = 10.0_m;
    auto time     = 2.0_s;
    auto accel    = distance / (time * time);
    auto str      = std::format("{}", accel);
    REQUIRE(str.find("2.5") != std::string::npos);
  }

  SECTION("Area m^2") {
    auto l    = 3.0_m;
    auto w    = 4.0_m;
    auto area = l * w;
    auto str  = std::format("{}", area);
    REQUIRE(str.find("12") != std::string::npos);
  }

  SECTION("Volume m^3") {
    auto l      = 2.0_m;
    auto w      = 2.0_m;
    auto h      = 2.0_m;
    auto volume = l * w * h;
    auto str    = std::format("{}", volume);
    REQUIRE(str.find('8') != std::string::npos);
  }

  SECTION("Force-like kg*m/s^2") {
    auto mass  = 5.0_kg;
    auto accel = 10.0_m / (1.0_s * 1.0_s);
    auto force = mass * accel;
    auto str   = std::format("{}", force);
    REQUIRE(str.find("50") != std::string::npos);
  }
}

TEST_CASE("Format: to_string variations", "[format]") {
  SECTION("to_string default") {
    auto q   = 42.0_m;
    auto str = to_string(q);
    REQUIRE(str.find("42") != std::string::npos);
    REQUIRE(str.find('m') != std::string::npos);
  }

  SECTION("to_string with precision") {
    // NOLINTNEXTLINE(modernize-use-std-numbers) - using 3.14159 as test value, not pi
    auto q   = make_quantity(3.14159, metre);
    auto str = to_string(q, 2);
    REQUIRE(str.find("3.14") != std::string::npos);
    REQUIRE(str.find('m') != std::string::npos);
  }

  SECTION("value_to_string (no unit)") {
    auto q   = 42.0_m;
    auto str = value_to_string(q);
    REQUIRE(str.find("42") != std::string::npos);
    REQUIRE(str.find('m') == std::string::npos);
  }
}

TEST_CASE("Format: unit_symbol function", "[format]") {
  SECTION("metre symbol") {
    auto symbol = unit_symbol<metre_t>();
    REQUIRE(symbol == "m");
  }

  SECTION("kilogram symbol") {
    auto symbol = unit_symbol<kilogram_t>();
    REQUIRE(symbol == "kg");
  }

  SECTION("second symbol") {
    auto symbol = unit_symbol<second_t>();
    REQUIRE(symbol == "s");
  }

  SECTION("kilometre symbol") {
    auto symbol = unit_symbol<kilometre_t>();
    REQUIRE(symbol == "km");
  }

  SECTION("millisecond symbol") {
    auto symbol = unit_symbol<millisecond_t>();
    REQUIRE(symbol == "ms");
  }
}

TEST_CASE("Format: reference_symbol function", "[format]") {
  SECTION("length reference symbol") {
    using length_ref = reference<isq::length_t, metre_t>;
    auto symbol      = reference_symbol<length_ref>();
    REQUIRE(symbol == "m");
  }

  SECTION("mass reference symbol") {
    using mass_ref = reference<isq::mass_t, kilogram_t>;
    auto symbol    = reference_symbol<mass_ref>();
    REQUIRE(symbol == "kg");
  }
}

// =============================================================================
// Additional math function tests
// =============================================================================

TEST_CASE("Math: trunc edge cases", "[math][trunc]") {
  SECTION("Positive with small fractional part") {
    auto q      = make_quantity(5.001, metre);
    auto result = trunc(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("Positive with large fractional part") {
    auto q      = make_quantity(5.999, metre);
    auto result = trunc(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("Negative with small fractional part") {
    auto q      = make_quantity(-5.001, metre);
    auto result = trunc(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-5.0, 1e-10));
  }

  SECTION("Negative with large fractional part") {
    auto q      = make_quantity(-5.999, metre);
    auto result = trunc(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-5.0, 1e-10));
  }

  SECTION("Exactly integer value") {
    auto q      = make_quantity(5.0, metre);
    auto result = trunc(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }
}

TEST_CASE("Math: round edge cases", "[math][round]") {
  SECTION("Exactly half - positive") {
    auto q      = make_quantity(2.5, metre);
    auto result = round(q);
    // Round half away from zero
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }

  SECTION("Exactly half - negative") {
    auto q      = make_quantity(-2.5, metre);
    auto result = round(q);
    // Round half away from zero
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-3.0, 1e-10));
  }

  SECTION("Just below half") {
    auto q      = make_quantity(2.49, metre);
    auto result = round(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(2.0, 1e-10));
  }

  SECTION("Just above half") {
    auto q      = make_quantity(2.51, metre);
    auto result = round(q);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }
}

TEST_CASE("Math: fmod edge cases", "[math][fmod]") {
  SECTION("Exact multiple") {
    auto dividend = make_quantity(10.0, metre);
    auto divisor  = make_quantity(5.0, metre);
    auto result   = fmod(dividend, divisor);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Larger divisor than dividend") {
    auto dividend = make_quantity(3.0, metre);
    auto divisor  = make_quantity(5.0, metre);
    auto result   = fmod(dividend, divisor);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }

  SECTION("Negative divisor") {
    auto dividend = make_quantity(10.5, metre);
    auto divisor  = make_quantity(-3.0, metre);
    auto result   = fmod(dividend, divisor);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(1.5, 1e-10));
  }
}

// NOTE: sqrt and cbrt with derived units require compile-time unit type simplification
// which isn't currently implemented. These are tested in static tests with proper types.
// See tests/static/math_test.cpp for sqrt/cbrt tests with typed units.

TEST_CASE("Math: pow with various exponents", "[math][pow]") {
  SECTION("pow<5>") {
    auto value  = 2.0_m;
    auto result = pow<5>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(32.0, 1e-10));
  }

  SECTION("pow<-3>") {
    auto value  = 2.0_m;
    auto result = pow<-3>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.125, 1e-10));
  }

  SECTION("pow<6>") {
    auto value  = 2.0_m;
    auto result = pow<6>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(64.0, 1e-10));
  }

  SECTION("pow<-4>") {
    auto value  = 2.0_m;
    auto result = pow<-4>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0625, 1e-10));
  }
}

// =============================================================================
// Hyperbolic function edge cases
// =============================================================================

TEST_CASE("Math: hyperbolic functions with larger values", "[math][hyperbolic]") {
  SECTION("sinh of 1") {
    auto value  = make_quantity(1.0, radian);
    auto result = sinh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::sinh(1.0), 1e-10));
  }

  SECTION("cosh of 1") {
    auto value  = make_quantity(1.0, radian);
    auto result = cosh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::cosh(1.0), 1e-10));
  }

  SECTION("tanh of 1") {
    auto value  = make_quantity(1.0, radian);
    auto result = tanh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::tanh(1.0), 1e-10));
  }

  SECTION("sinh of negative value") {
    auto value  = make_quantity(-1.0, radian);
    auto result = sinh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::sinh(-1.0), 1e-10));
  }

  SECTION("cosh of negative value (cosh is even)") {
    auto value  = make_quantity(-1.0, radian);
    auto result = cosh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::cosh(-1.0), 1e-10));
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::cosh(1.0), 1e-10)); // cosh(-x) = cosh(x)
  }

  SECTION("tanh approaches 1 for large positive") {
    auto value  = make_quantity(5.0, radian);
    auto result = tanh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::tanh(5.0), 1e-10));
    REQUIRE(result > 0.99);
  }

  SECTION("tanh approaches -1 for large negative") {
    auto value  = make_quantity(-5.0, radian);
    auto result = tanh(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::tanh(-5.0), 1e-10));
    REQUIRE(result < -0.99);
  }
}

// =============================================================================
// Quantity arithmetic with mixed types
// =============================================================================

TEST_CASE("Quantity: mixed representation arithmetic", "[quantity][mixed]") {
  SECTION("double + float (via implicit conversion)") {
    auto                                                     a = 5.0_m;
    const quantity<reference<isq::length_t, metre_t>, float> b { 3.0F };
    // Result should have common_type (double)
    auto result
        = a + quantity<reference<isq::length_t, metre_t>, double>(static_cast<double>(b.value()));
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(8.0, 1e-6));
  }

  SECTION("int quantity with double scalar") {
    const quantity<reference<isq::length_t, metre_t>, int> q { 5 };
    auto                                                   result = q * 2.0;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }
}

// =============================================================================
// QuantityPoint additional tests
// =============================================================================

TEST_CASE("QuantityPoint: in() method", "[quantity_point][conversion]") {
  SECTION("kelvin_pt in different unit") {
    auto temp      = kelvin_pt(300.0);
    auto converted = temp.in(kelvin);
    REQUIRE_THAT(converted.value(), Catch::Matchers::WithinAbs(300.0, 1e-10));
  }

  SECTION("celsius_pt in degree_celsius") {
    auto temp      = celsius_pt(25.0);
    auto converted = temp.in(degree_celsius);
    REQUIRE_THAT(converted.value(), Catch::Matchers::WithinAbs(25.0, 1e-10));
  }
}

// NOLINTEND(readability-function-cognitive-complexity)
