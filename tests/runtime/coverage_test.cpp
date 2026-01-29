// SPDX-License-Identifier: MIT

// Comprehensive runtime tests to improve code coverage

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <format>
#include <string>
#include <units/units.hpp>

#include <cmath>

#include <numbers>

using namespace units;
using namespace units::si;
using namespace units::si::literals;

// NOLINTBEGIN(readability-function-cognitive-complexity)

// =============================================================================
// Quantity class tests - covering compound assignment and arithmetic
// =============================================================================

TEST_CASE("Quantity compound assignment operators", "[quantity][compound]") {
  SECTION("operator+= with same reference") {
    auto q  = 5.0_m;
    q      += 3.0_m;
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(8.0, 1e-10));
  }

  SECTION("operator-= with same reference") {
    auto q  = 10.0_m;
    q      -= 3.0_m;
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(7.0, 1e-10));
  }

  SECTION("operator*= with scalar") {
    auto q  = 5.0_m;
    q      *= 3.0;
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(15.0, 1e-10));
  }

  SECTION("operator*= with int scalar") {
    auto q  = 5.0_m;
    q      *= 2;
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }

  SECTION("operator/= with scalar") {
    auto q  = 15.0_m;
    q      /= 3.0;
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("operator/= with int scalar") {
    auto q  = 10.0_m;
    q      /= 2;
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }
}

TEST_CASE("Quantity unary operators", "[quantity][unary]") {
  SECTION("unary plus") {
    auto q      = 5.0_m;
    auto result = +q;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("unary minus") {
    auto q      = 5.0_m;
    auto result = -q;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-5.0, 1e-10));
  }

  SECTION("unary minus on negative") {
    auto q      = make_quantity(-5.0, metre);
    auto result = -q;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }
}

TEST_CASE("Quantity unit conversion methods", "[quantity][conversion]") {
  SECTION("numerical_value_in() method") {
    auto distance = 1.0_km;
    auto in_m     = distance.numerical_value_in(metre);
    REQUIRE_THAT(in_m, Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("numerical_value_in() with millimetre") {
    auto distance = 1.0_m;
    auto in_mm    = distance.numerical_value_in(millimetre);
    REQUIRE_THAT(in_mm, Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("in() method converts unit") {
    auto distance = 1.0_km;
    auto in_m     = distance.in(metre);
    REQUIRE_THAT(in_m.value(), Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("in() method with millimetre") {
    auto distance = 1.0_m;
    auto in_mm    = distance.in(millimetre);
    REQUIRE_THAT(in_mm.value(), Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("force_in() method") {
    auto distance = 1.5_km;
    auto in_m     = distance.force_in(metre);
    REQUIRE_THAT(in_m.value(), Catch::Matchers::WithinAbs(1500.0, 1e-10));
  }
}

TEST_CASE("Quantity scalar operations", "[quantity][scalar]") {
  SECTION("quantity * scalar") {
    auto q      = 5.0_m;
    auto result = q * 3.0;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(15.0, 1e-10));
  }

  SECTION("scalar * quantity") {
    auto q      = 5.0_m;
    auto result = 3.0 * q;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(15.0, 1e-10));
  }

  SECTION("quantity / scalar") {
    auto q      = 15.0_m;
    auto result = q / 3.0;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("int scalar * quantity") {
    auto q      = 5.0_m;
    auto result = 2 * q;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }
}

TEST_CASE("Quantity comparison operators", "[quantity][comparison]") {
  SECTION("equality - same reference") {
    auto a = 5.0_m;
    auto b = 5.0_m;
    REQUIRE(a == b);
  }

  SECTION("inequality - same reference") {
    auto a = 5.0_m;
    auto b = 6.0_m;
    REQUIRE(a != b);
  }

  SECTION("three-way comparison - less than") {
    auto       a        = 5.0_m;
    auto       b        = 10.0_m;
    auto       cmp      = a <=> b;
    const bool is_less  = cmp < 0;
    const bool a_less_b = a < b;
    REQUIRE(is_less);
    REQUIRE(a_less_b);
  }

  SECTION("three-way comparison - greater than") {
    auto       a           = 10.0_m;
    auto       b           = 5.0_m;
    auto       cmp         = a <=> b;
    const bool is_greater  = cmp > 0;
    const bool a_greater_b = a > b;
    REQUIRE(is_greater);
    REQUIRE(a_greater_b);
  }

  SECTION("three-way comparison - equal") {
    auto       a        = 5.0_m;
    auto       b        = 5.0_m;
    auto       cmp      = a <=> b;
    const bool is_equal = cmp == 0;
    REQUIRE(is_equal);
  }

  SECTION("less than or equal") {
    auto a = 5.0_m;
    auto b = 5.0_m;
    REQUIRE(a <= b);
  }

  SECTION("greater than or equal") {
    auto a = 5.0_m;
    auto b = 5.0_m;
    REQUIRE(a >= b);
  }
}

TEST_CASE("Quantity arithmetic with same reference", "[quantity][arithmetic]") {
  SECTION("addition with same reference") {
    auto a      = 5.0_m;
    auto b      = 3.0_m;
    auto result = a + b;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(8.0, 1e-10));
  }

  SECTION("subtraction with same reference") {
    auto a      = 10.0_m;
    auto b      = 3.0_m;
    auto result = a - b;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(7.0, 1e-10));
  }
}

TEST_CASE("Quantity multiplication and division", "[quantity][derived]") {
  SECTION("quantity * quantity creates derived") {
    auto length = 5.0_m;
    auto width  = 3.0_m;
    auto area   = length * width;
    REQUIRE_THAT(area.value(), Catch::Matchers::WithinAbs(15.0, 1e-10));
  }

  SECTION("quantity / quantity creates derived") {
    auto distance = 100.0_m;
    auto time     = 10.0_s;
    auto speed    = distance / time;
    REQUIRE_THAT(speed.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }
}

// =============================================================================
// Unit conversion factor tests
// =============================================================================

TEST_CASE("Unit conversion factors", "[unit][conversion]") {
  SECTION("kilometre to metre") {
    constexpr auto factor = conversion_factor<kilometre_t, metre_t>();
    REQUIRE_THAT(factor, Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("metre to kilometre") {
    constexpr auto factor = conversion_factor<metre_t, kilometre_t>();
    REQUIRE_THAT(factor, Catch::Matchers::WithinAbs(0.001, 1e-10));
  }

  SECTION("metre to millimetre") {
    constexpr auto factor = conversion_factor<metre_t, millimetre_t>();
    REQUIRE_THAT(factor, Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("millimetre to metre") {
    constexpr auto factor = conversion_factor<millimetre_t, metre_t>();
    REQUIRE_THAT(factor, Catch::Matchers::WithinAbs(0.001, 1e-10));
  }
}

// =============================================================================
// Ratio tests - runtime accessible operations
// =============================================================================

TEST_CASE("Ratio value conversion", "[ratio]") {
  SECTION("ratio value as double") {
    constexpr auto val = ratio<1, 2>::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 1e-10));
  }

  SECTION("ratio value as float") {
    constexpr auto val = ratio<1, 4>::value<float>();
    REQUIRE_THAT(static_cast<double>(val), Catch::Matchers::WithinAbs(0.25, 1e-6));
  }

  SECTION("ratio equals comparison") {
    constexpr auto r1 = ratio<2, 4> {};
    constexpr auto r2 = ratio<1, 2> {};
    static_assert(r1.equals(r2)); // NOLINT(readability-static-accessed-through-instance)
  }

  SECTION("ratio with negative numerator") {
    constexpr auto val = ratio<-3, 4>::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(-0.75, 1e-10));
  }

  SECTION("kilo ratio value") {
    constexpr auto val = kilo::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("milli ratio value") {
    constexpr auto val = milli::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.001, 1e-10));
  }
}

// =============================================================================
// Math functions - covering missing functions
// =============================================================================

TEST_CASE("Clamp function", "[math][clamp]") {
  SECTION("value within range") {
    auto val    = 5.0_m;
    auto lo     = 0.0_m;
    auto hi     = 10.0_m;
    auto result = clamp(val, lo, hi);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("value below range") {
    auto val    = make_quantity(-5.0, metre);
    auto lo     = 0.0_m;
    auto hi     = 10.0_m;
    auto result = clamp(val, lo, hi);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("value above range") {
    auto val    = 15.0_m;
    auto lo     = 0.0_m;
    auto hi     = 10.0_m;
    auto result = clamp(val, lo, hi);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }

  SECTION("value at lower bound") {
    auto val    = 0.0_m;
    auto lo     = 0.0_m;
    auto hi     = 10.0_m;
    auto result = clamp(val, lo, hi);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("value at upper bound") {
    auto val    = 10.0_m;
    auto lo     = 0.0_m;
    auto hi     = 10.0_m;
    auto result = clamp(val, lo, hi);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }
}

TEST_CASE("is_zero helper function", "[math][helpers]") {
  SECTION("zero value") {
    auto val = 0.0_m;
    REQUIRE(is_zero(val));
  }

  SECTION("non-zero positive value") {
    auto val = 1.0_m;
    REQUIRE_FALSE(is_zero(val));
  }

  SECTION("non-zero negative value") {
    auto val = make_quantity(-1.0, metre);
    REQUIRE_FALSE(is_zero(val));
  }

  SECTION("very small positive value - within default epsilon") {
    auto val = make_quantity(1e-12, metre);
    REQUIRE(is_zero(val));
  }

  SECTION("small value with custom epsilon") {
    auto val = make_quantity(1e-5, metre);
    REQUIRE(is_zero(val, 1e-4));       // custom larger epsilon
    REQUIRE_FALSE(is_zero(val, 1e-6)); // custom smaller epsilon
  }
}

TEST_CASE("approx_equal helper function", "[math][helpers]") {
  SECTION("equal values") {
    auto a = 5.0_m;
    auto b = 5.0_m;
    REQUIRE(approx_equal(a, b));
  }

  SECTION("approximately equal values") {
    auto a = make_quantity(5.0, metre);
    auto b = make_quantity(5.0 + 1e-15, metre);
    REQUIRE(approx_equal(a, b));
  }

  SECTION("different values") {
    auto a = 5.0_m;
    auto b = 6.0_m;
    REQUIRE_FALSE(approx_equal(a, b));
  }

  SECTION("relative tolerance check") {
    auto a = make_quantity(1000.0, metre);
    auto b = make_quantity(1000.0 + 1e-7, metre);
    REQUIRE(approx_equal(a, b));
  }

  SECTION("custom tolerance") {
    auto a = make_quantity(5.0, metre);
    auto b = make_quantity(5.1, metre);
    REQUIRE_FALSE(approx_equal(a, b)); // default tolerance fails
    REQUIRE(approx_equal(a, b, 0.05)); // 5% relative tolerance passes
  }
}

TEST_CASE("Exponential and logarithmic functions", "[math][exp_log]") {
  SECTION("exp of 0") {
    auto result = units::exp(0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("exp of 1") {
    auto result = units::exp(1.0); // NOLINT(modernize-use-std-numbers)
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::e, 1e-10));
  }

  SECTION("exp of negative value") {
    auto result = units::exp(-1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0 / std::numbers::e, 1e-10));
  }

  SECTION("log of 1") {
    auto result = units::log(1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("log of e") {
    auto result = units::log(std::numbers::e);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("log of large value") {
    auto result = units::log(100.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::log(100.0), 1e-10));
  }

  SECTION("log10 of 10") {
    auto result = units::log10(10.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("log10 of 100") {
    auto result = units::log10(100.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(2.0, 1e-10));
  }

  SECTION("log2 of 2") {
    auto result = units::log2(2.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("log2 of 8") {
    auto result = units::log2(8.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(3.0, 1e-10));
  }

  SECTION("exp and log are inverses") {
    const double x      = 2.5;
    auto         result = units::log(units::exp(x));
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(x, 1e-10));
  }
}

TEST_CASE("Inverse trigonometric functions (units versions)", "[math][trig]") {
  SECTION("asin of 0") {
    auto result = units::asin(0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("asin of 1") {
    auto result = units::asin(1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("asin of -1") {
    auto result = units::asin(-1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("acos of 1") {
    auto result = units::acos(1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("acos of 0") {
    auto result = units::acos(0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("acos of -1") {
    auto result = units::acos(-1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi, 1e-10));
  }

  SECTION("atan of 0") {
    auto result = units::atan(0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("atan of 1") {
    auto result = units::atan(1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 4.0, 1e-10));
  }

  SECTION("atan of -1") {
    auto result = units::atan(-1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-std::numbers::pi / 4.0, 1e-10));
  }

  SECTION("atan of large positive value") {
    auto result = units::atan(100.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::atan(100.0), 1e-10));
  }

  SECTION("atan of large negative value") {
    auto result = units::atan(-100.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::atan(-100.0), 1e-10));
  }
}

TEST_CASE("atan2 function", "[math][trig]") {
  SECTION("atan2 - first quadrant") {
    auto result = units::atan2(1.0, 1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 4.0, 1e-10));
  }

  SECTION("atan2 - second quadrant") {
    auto result = units::atan2(1.0, -1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(3.0 * std::numbers::pi / 4.0, 1e-10));
  }

  SECTION("atan2 - third quadrant") {
    auto result = units::atan2(-1.0, -1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-3.0 * std::numbers::pi / 4.0, 1e-10));
  }

  SECTION("atan2 - fourth quadrant") {
    auto result = units::atan2(-1.0, 1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-std::numbers::pi / 4.0, 1e-10));
  }

  SECTION("atan2 - positive y axis") {
    auto result = units::atan2(1.0, 0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("atan2 - negative y axis") {
    auto result = units::atan2(-1.0, 0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("atan2 - positive x axis") {
    auto result = units::atan2(0.0, 1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("atan2 - negative x axis") {
    auto result = units::atan2(0.0, -1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi, 1e-10));
  }

  SECTION("atan2 - origin") {
    auto result = units::atan2(0.0, 0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }
}

TEST_CASE("Power function variations", "[math][pow]") {
  SECTION("pow<2> - squaring") {
    auto value  = 3.0_m;
    auto result = pow<2>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(9.0, 1e-10));
  }

  SECTION("pow<3> - cubing") {
    auto value  = 2.0_m;
    auto result = pow<3>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(8.0, 1e-10));
  }

  SECTION("pow<-2> - inverse square") {
    auto value  = 2.0_m;
    auto result = pow<-2>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.25, 1e-10));
  }

  SECTION("pow<4> - fourth power") {
    auto value  = 2.0_m;
    auto result = pow<4>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(16.0, 1e-10));
  }
}

// =============================================================================
// Quantity point tests - covering additional operations
// =============================================================================

TEST_CASE("Quantity point compound assignment", "[quantity_point][compound]") {
  SECTION("operator+= with quantity") {
    auto temp  = kelvin_pt(300.0);
    temp      += make_quantity(50.0, kelvin);
    REQUIRE_THAT(temp.value(), Catch::Matchers::WithinAbs(350.0, 1e-10));
  }

  SECTION("operator-= with quantity") {
    auto temp  = kelvin_pt(300.0);
    temp      -= make_quantity(50.0, kelvin);
    REQUIRE_THAT(temp.value(), Catch::Matchers::WithinAbs(250.0, 1e-10));
  }
}

TEST_CASE("Quantity point arithmetic", "[quantity_point][arithmetic]") {
  SECTION("point + quantity = point") {
    auto temp     = kelvin_pt(273.15);
    auto delta    = make_quantity(100.0, kelvin);
    auto new_temp = temp + delta;
    REQUIRE_THAT(new_temp.value(), Catch::Matchers::WithinAbs(373.15, 1e-10));
  }

  SECTION("point - quantity = point") {
    auto temp     = kelvin_pt(373.15);
    auto delta    = make_quantity(100.0, kelvin);
    auto new_temp = temp - delta;
    REQUIRE_THAT(new_temp.value(), Catch::Matchers::WithinAbs(273.15, 1e-10));
  }

  SECTION("point - point = quantity") {
    auto temp1 = kelvin_pt(373.15);
    auto temp2 = kelvin_pt(273.15);
    auto diff  = temp1 - temp2;
    REQUIRE_THAT(diff.value(), Catch::Matchers::WithinAbs(100.0, 1e-10));
  }
}

TEST_CASE("Quantity point comparison", "[quantity_point][comparison]") {
  SECTION("equality") {
    auto a = kelvin_pt(300.0);
    auto b = kelvin_pt(300.0);
    REQUIRE(a == b);
  }

  SECTION("inequality") {
    auto a = kelvin_pt(300.0);
    auto b = kelvin_pt(350.0);
    REQUIRE(a != b);
  }

  SECTION("three-way comparison - less than") {
    auto       a        = kelvin_pt(300.0);
    auto       b        = kelvin_pt(350.0);
    auto       cmp      = a <=> b;
    const bool is_less  = cmp < 0;
    const bool a_less_b = a < b;
    REQUIRE(is_less);
    REQUIRE(a_less_b);
  }

  SECTION("three-way comparison - greater than") {
    auto       a           = kelvin_pt(350.0);
    auto       b           = kelvin_pt(300.0);
    auto       cmp         = a <=> b;
    const bool is_greater  = cmp > 0;
    const bool a_greater_b = a > b;
    REQUIRE(is_greater);
    REQUIRE(a_greater_b);
  }
}

TEST_CASE("Quantity point accessors", "[quantity_point][accessors]") {
  SECTION("value() accessor") {
    auto temp = kelvin_pt(300.0);
    REQUIRE_THAT(temp.value(), Catch::Matchers::WithinAbs(300.0, 1e-10));
  }

  SECTION("quantity_from_origin() accessor") {
    auto temp = kelvin_pt(300.0);
    auto qty  = temp.quantity_from_origin();
    REQUIRE_THAT(qty.value(), Catch::Matchers::WithinAbs(300.0, 1e-10));
  }

  SECTION("numerical_value_in() method") {
    // numerical_value_in converts the displacement unit, not the absolute temperature
    // celsius_point(100.0) has a displacement of 100 degrees from ice_point
    // Since 1°C = 1 K for intervals, numerical_value_in(kelvin) returns 100.0
    auto temp = celsius_pt(100.0);
    auto in_k = temp.numerical_value_in(degree_celsius);
    REQUIRE_THAT(in_k, Catch::Matchers::WithinAbs(100.0, 1e-10));
  }
}

TEST_CASE("Quantity point unit conversion", "[quantity_point][conversion]") {
  SECTION("in() method for unit conversion") {
    // Create a temperature in Kelvin and convert to different representation
    auto temp      = kelvin_pt(300.0);
    auto converted = temp.in(kelvin);
    REQUIRE_THAT(converted.value(), Catch::Matchers::WithinAbs(300.0, 1e-10));
  }
}

// =============================================================================
// Fixed string tests
// =============================================================================

TEST_CASE("Fixed string operations", "[unit][fixed_string]") {
  SECTION("size() method") {
    constexpr fixed_string str { "hello" };
    static_assert(str.size() == 5);
  }

  SECTION("operator[] access") {
    constexpr fixed_string str { "test" };
    static_assert(str[0] == 't');
    static_assert(str[1] == 'e');
    static_assert(str[2] == 's');
    static_assert(str[3] == 't');
  }

  SECTION("equality comparison - same content") {
    constexpr fixed_string str1 { "hello" };
    constexpr fixed_string str2 { "hello" };
    static_assert(str1 == str2);
  }

  SECTION("equality comparison - different content") {
    constexpr fixed_string str1 { "hello" };
    constexpr fixed_string str2 { "world" };
    static_assert(!(str1 == str2));
  }

  SECTION("equality comparison - different sizes") {
    constexpr fixed_string str1 { "hi" };
    constexpr fixed_string str2 { "hello" };
    static_assert(!(str1 == str2));
  }
}

// =============================================================================
// Magnitude tests
// =============================================================================

TEST_CASE("Magnitude value", "[unit][magnitude]") {
  SECTION("mag_one value") {
    constexpr auto val = mag_one::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("kilo magnitude value") {
    using kilo_mag     = magnitude<kilo>;
    constexpr auto val = kilo_mag::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("milli magnitude value") {
    using milli_mag    = magnitude<milli>;
    constexpr auto val = milli_mag::value<double>();
    REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.001, 1e-10));
  }
}

// =============================================================================
// Reference operations tests
// =============================================================================

TEST_CASE("Reference multiplication and division", "[reference]") {
  SECTION("reference multiplication") {
    using length_ref = reference<isq::length_t, metre_t>;
    using time_ref   = reference<isq::time_t, second_t>;

    constexpr length_ref len_ref {};
    constexpr time_ref   time_ref_val {};

    auto result = len_ref * time_ref_val;
    // Result should be a valid reference type
    static_assert(Reference<decltype(result)>);
  }

  SECTION("reference division") {
    using length_ref = reference<isq::length_t, metre_t>;
    using time_ref   = reference<isq::time_t, second_t>;

    constexpr length_ref len_ref {};
    constexpr time_ref   time_ref_val {};

    auto result = len_ref / time_ref_val;
    // Result should be a valid reference type (speed)
    static_assert(Reference<decltype(result)>);
  }

  SECTION("make_reference factory") {
    auto ref = make_reference(isq::length, metre);
    static_assert(Reference<decltype(ref)>);
  }

  SECTION("make_reference from unit only") {
    auto ref = make_reference(metre);
    static_assert(Reference<decltype(ref)>);
  }
}

// =============================================================================
// Make quantity factory tests
// =============================================================================

TEST_CASE("make_quantity factory", "[quantity][factory]") {
  SECTION("make_quantity with value and unit") {
    auto q = make_quantity(5.0, metre);
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("make_quantity with int value") {
    auto q = make_quantity(5, metre);
    REQUIRE(q.value() == 5);
  }

  SECTION("make_quantity with negative value") {
    auto q = make_quantity(-5.0, metre);
    REQUIRE_THAT(q.value(), Catch::Matchers::WithinAbs(-5.0, 1e-10));
  }
}

// =============================================================================
// Different representation type tests
// =============================================================================

TEST_CASE("Quantity with different rep types", "[quantity][rep]") {
  SECTION("quantity with float rep") {
    using float_length_ref = reference<isq::length_t, metre_t>;
    auto q                 = quantity<float_length_ref, float> { 5.0F };
    REQUIRE_THAT(static_cast<double>(q.value()), Catch::Matchers::WithinAbs(5.0, 1e-6));
  }

  SECTION("quantity with int rep") {
    using int_length_ref = reference<isq::length_t, metre_t>;
    auto q               = quantity<int_length_ref, int> { 5 };
    REQUIRE(q.value() == 5);
  }

  SECTION("mixed rep arithmetic") {
    auto a = 5.0_m;
    auto b = make_quantity(3, metre);
    auto result
        = a + quantity<reference<isq::length_t, metre_t>, double>(static_cast<double>(b.value()));
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(8.0, 1e-10));
  }
}

// =============================================================================
// is_quantity trait tests
// =============================================================================

TEST_CASE("is_quantity type trait", "[quantity][traits]") {
  SECTION("quantity type returns true") {
    auto q = 5.0_m;
    static_assert(is_quantity_v<decltype(q)>);
    static_assert(Quantity<decltype(q)>);
  }

  SECTION("non-quantity type returns false") {
    static_assert(!is_quantity_v<double>);
    static_assert(!is_quantity_v<int>);
    static_assert(!Quantity<double>);
  }
}

// =============================================================================
// Operations with different compatible references
// =============================================================================

TEST_CASE("Operations with different compatible references", "[quantity][compatible_refs]") {
  SECTION("km + m uses different compatible references") {
    auto km_val = 1.0_km;
    auto m_val  = 500.0_m;
    // This triggers the operator+ overload for different but compatible references
    auto result = km_val + m_val;
    // Result should be in km (first operand's unit), so 1.0 + 0.5 = 1.5 km
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(1.5, 1e-10));
  }

  SECTION("m + km uses different compatible references") {
    auto m_val  = 500.0_m;
    auto km_val = 1.0_km;
    // Result should be in m (first operand's unit), so 500.0 + 1000.0 = 1500.0 m
    auto result = m_val + km_val;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(1500.0, 1e-10));
  }

  SECTION("km - m uses different compatible references") {
    auto km_val = 2.0_km;
    auto m_val  = 500.0_m;
    // Result should be in km, so 2.0 - 0.5 = 1.5 km
    auto result = km_val - m_val;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(1.5, 1e-10));
  }

  SECTION("m - km uses different compatible references") {
    auto m_val  = 2000.0_m;
    auto km_val = 1.0_km;
    // Result should be in m, so 2000.0 - 1000.0 = 1000.0 m
    auto result = m_val - km_val;
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(1000.0, 1e-10));
  }

  SECTION("km == m comparison with different references") {
    auto km_val = 1.0_km;
    auto m_val  = 1000.0_m;
    // Should be equal after conversion
    REQUIRE(km_val == m_val);
  }

  SECTION("km != m comparison with different references") {
    auto km_val = 1.0_km;
    auto m_val  = 500.0_m;
    // Should not be equal
    REQUIRE(km_val != m_val);
  }

  SECTION("km <=> m three-way comparison with different references") {
    auto km_val = 1.0_km;
    auto m_val  = 500.0_m;
    // 1 km > 500 m
    auto       cmp          = km_val <=> m_val;
    const bool is_greater   = cmp > 0;
    const bool km_greater_m = km_val > m_val;
    REQUIRE(is_greater);
    REQUIRE(km_greater_m);
  }

  SECTION("m < km comparison") {
    auto m_val  = 500.0_m;
    auto km_val = 1.0_km;
    // 500 m < 1 km
    const bool m_less_km = m_val < km_val;
    REQUIRE(m_less_km);
  }
}

// =============================================================================
// Derived unit formatting with multiple terms (middle dot coverage)
// =============================================================================

TEST_CASE("Derived unit formatting with multiple terms", "[format][derived]") {
  SECTION("Format m*m (area)") {
    auto length = 3.0_m;
    auto width  = 4.0_m;
    auto area   = length * width;
    auto str    = std::format("{}", area);
    // Should contain the area value
    REQUIRE(str.find("12") != std::string::npos);
  }

  SECTION("Format m*m*m (volume) - triggers middle dot") {
    auto l      = 2.0_m;
    auto w      = 3.0_m;
    auto ht     = 4.0_m;
    auto volume = l * w * ht;
    auto str    = std::format("{}", volume);
    // Should contain 24
    REQUIRE(str.find("24") != std::string::npos);
  }

  SECTION("Format m/s (speed)") {
    auto distance = 100.0_m;
    auto time     = 10.0_s;
    auto speed    = distance / time;
    auto str      = std::format("{}", speed);
    // Should contain 10
    REQUIRE(str.find("10") != std::string::npos);
  }

  SECTION("Format kg*m/s^2 (force-like)") {
    auto mass         = 2.0_kg;
    auto distance     = 5.0_m;
    auto time         = 2.0_s;
    auto acceleration = distance / (time * time);
    auto force_like   = mass * acceleration;
    auto str          = std::format("{}", force_like);
    // Should contain the value 2.5
    REQUIRE(str.find("2.5") != std::string::npos);
  }
}

// =============================================================================
// Additional quantity multiplication/division tests
// =============================================================================

TEST_CASE("Quantity multiplication creates derived quantities", "[quantity][multiplication]") {
  SECTION("m * m = m^2") {
    auto l    = 5.0_m;
    auto w    = 4.0_m;
    auto area = l * w;
    REQUIRE_THAT(area.value(), Catch::Matchers::WithinAbs(20.0, 1e-10));
  }

  SECTION("m * m * m = m^3") {
    auto l      = 2.0_m;
    auto w      = 3.0_m;
    auto ht     = 4.0_m;
    auto volume = l * w * ht;
    REQUIRE_THAT(volume.value(), Catch::Matchers::WithinAbs(24.0, 1e-10));
  }

  SECTION("m / s = speed") {
    auto dist  = 100.0_m;
    auto t     = 5.0_s;
    auto speed = dist / t;
    REQUIRE_THAT(speed.value(), Catch::Matchers::WithinAbs(20.0, 1e-10));
  }

  SECTION("m / s / s = acceleration") {
    auto dist  = 50.0_m;
    auto t     = 5.0_s;
    auto accel = dist / t / t;
    REQUIRE_THAT(accel.value(), Catch::Matchers::WithinAbs(2.0, 1e-10));
  }

  SECTION("kg * m / s^2 = force") {
    auto mass  = 10.0_kg;
    auto dist  = 20.0_m;
    auto t     = 2.0_s;
    auto force = mass * dist / (t * t);
    REQUIRE_THAT(force.value(), Catch::Matchers::WithinAbs(50.0, 1e-10));
  }
}

// =============================================================================
// Temperature factory functions direct usage
// =============================================================================

TEST_CASE("Temperature factory functions", "[temperature][factory]") {
  SECTION("kelvin_pt creates temperature point") {
    auto temp = kelvin_pt(300.0);
    REQUIRE_THAT(temp.value(), Catch::Matchers::WithinAbs(300.0, 1e-10));
  }

  SECTION("celsius_pt creates temperature point") {
    auto temp = celsius_pt(25.0);
    REQUIRE_THAT(temp.value(), Catch::Matchers::WithinAbs(25.0, 1e-10));
  }

  SECTION("fahrenheit_pt creates temperature point") {
    auto temp = fahrenheit_pt(77.0);
    REQUIRE_THAT(temp.value(), Catch::Matchers::WithinAbs(77.0, 1e-10));
  }
}

// =============================================================================
// Temperature conversion functions
// =============================================================================

TEST_CASE("Temperature conversion functions", "[temperature][conversion]") {
  SECTION("to_kelvin from kelvin (no-op)") {
    auto k      = kelvin_pt(300.0);
    auto result = to_kelvin(k);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(300.0, 1e-10));
  }

  SECTION("to_celsius from celsius (no-op)") {
    auto c      = celsius_pt(25.0);
    auto result = to_celsius(c);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(25.0, 1e-10));
  }

  SECTION("to_fahrenheit from fahrenheit (no-op)") {
    auto f      = fahrenheit_pt(77.0);
    auto result = to_fahrenheit(f);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(77.0, 1e-10));
  }
}

// NOLINTEND(readability-function-cognitive-complexity)
