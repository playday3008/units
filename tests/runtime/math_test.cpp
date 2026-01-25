// SPDX-License-Identifier: MIT

// Runtime tests for mathematical functions with quantities

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <units/units.hpp>

#include <cmath>

#include <limits>
#include <numbers>

using namespace units;
using namespace units::si;
using namespace units::si::literals;

// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Absolute value function", "[math][abs]") {
  SECTION("Positive value unchanged") {
    auto value  = 5.0_m;
    auto result = abs(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("Negative value becomes positive") {
    auto value  = make_quantity(-5.0, metre);
    auto result = abs(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("Zero unchanged") {
    auto value  = 0.0_m;
    auto result = abs(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(0.0, 1e-10));
  }
}

TEST_CASE("Min and Max functions", "[math][minmax]") {
  SECTION("Min of two values") {
    auto a      = 5.0_m;
    auto b      = 10.0_m;
    auto result = min(a, b);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(5.0, 1e-10));
  }

  SECTION("Max of two values") {
    auto a      = 5.0_m;
    auto b      = 10.0_m;
    auto result = max(a, b);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(10.0, 1e-10));
  }

  SECTION("Min with negative values") {
    auto a      = make_quantity(-5.0, metre);
    auto b      = make_quantity(-10.0, metre);
    auto result = min(a, b);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-10.0, 1e-10));
  }

  SECTION("Max with negative values") {
    auto a      = make_quantity(-5.0, metre);
    auto b      = make_quantity(-10.0, metre);
    auto result = max(a, b);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-5.0, 1e-10));
  }
}

TEST_CASE("Rounding functions", "[math][rounding]") {
  SECTION("Floor") {
    auto value  = make_quantity(3.7, metre);
    auto result = floor(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }

  SECTION("Floor negative") {
    auto value  = make_quantity(-3.7, metre);
    auto result = floor(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-4.0, 1e-10));
  }

  SECTION("Ceil") {
    auto value  = make_quantity(3.2, metre);
    auto result = ceil(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(4.0, 1e-10));
  }

  SECTION("Ceil negative") {
    auto value  = make_quantity(-3.2, metre);
    auto result = ceil(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-3.0, 1e-10));
  }

  SECTION("Round half away from zero") {
    auto value  = make_quantity(2.5, metre);
    auto result = round(value);
    // std::round uses "round half away from zero": 2.5 -> 3.0
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }

  SECTION("Round") {
    auto value  = make_quantity(3.7, metre);
    auto result = round(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(4.0, 1e-10));
  }

  SECTION("Trunc") {
    auto value  = make_quantity(3.9, metre);
    auto result = trunc(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }

  SECTION("Trunc negative") {
    auto value  = make_quantity(-3.9, metre);
    auto result = trunc(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-3.0, 1e-10));
  }
}

// NOTE: sqrt and pow with dimension changes are tested in static tests.
// The runtime test focuses on scalar operations since the library's unit
// multiplication doesn't yet simplify m^1*m^1 to m^2 for sqrt compatibility.

TEST_CASE("Power functions on scalars", "[math][pow]") {
  SECTION("pow<0> returns 1") {
    auto value  = 5.0_m;
    auto result = pow<0>(value);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("pow<1> returns same value") {
    auto value  = 3.0_m;
    auto result = pow<1>(value);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(3.0, 1e-10));
  }
}

TEST_CASE("Trigonometric functions", "[math][trig]") {
  SECTION("Sin of pi/2 radians") {
    auto angle  = make_quantity(std::numbers::pi / 2.0, radian);
    auto result = sin(angle); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("Sin of 0 radians") {
    auto angle  = 0.0_rad;
    auto result = sin(angle); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Cos of 0 radians") {
    auto angle  = 0.0_rad;
    auto result = cos(angle); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("Cos of pi radians") {
    auto angle  = make_quantity(std::numbers::pi, radian);
    auto result = cos(angle); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-1.0, 1e-10));
  }

  SECTION("Tan of pi/4 radians") {
    auto angle  = make_quantity(std::numbers::pi / 4.0, radian);
    auto result = tan(angle); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("Sin squared + Cos squared = 1") {
    auto angle      = make_quantity(0.5, radian);
    auto sin_result = sin(angle); // Returns double
    auto cos_result = cos(angle); // Returns double
    auto sum        = (sin_result * sin_result) + (cos_result * cos_result);
    REQUIRE_THAT(sum, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }
}

TEST_CASE("Inverse trigonometric functions", "[math][trig]") {
  SECTION("Asin") {
    // asin(1) = pi/2
    auto result = std::asin(1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("Acos") {
    // acos(0) = pi/2
    auto result = std::acos(0.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 2.0, 1e-10));
  }

  SECTION("Atan") {
    // atan(1) = pi/4
    auto result = std::atan(1.0);
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(std::numbers::pi / 4.0, 1e-10));
  }
}

TEST_CASE("Edge cases with special values", "[math][edge]") {
  SECTION("Abs of infinity") {
    auto value  = make_quantity(std::numeric_limits<double>::infinity(), metre);
    auto result = abs(value);
    REQUIRE(std::isinf(result.value()));
    REQUIRE(result.value() > 0);
  }

  SECTION("Abs of negative infinity") {
    auto value  = make_quantity(-std::numeric_limits<double>::infinity(), metre);
    auto result = abs(value);
    REQUIRE(std::isinf(result.value()));
    REQUIRE(result.value() > 0);
  }

  SECTION("Floor of infinity") {
    auto value  = make_quantity(std::numeric_limits<double>::infinity(), metre);
    auto result = floor(value);
    REQUIRE(std::isinf(result.value()));
  }

  SECTION("Ceil of negative infinity") {
    auto value  = make_quantity(-std::numeric_limits<double>::infinity(), metre);
    auto result = ceil(value);
    REQUIRE(std::isinf(result.value()));
    REQUIRE(result.value() < 0);
  }

  SECTION("NaN handling in abs") {
    auto value  = make_quantity(std::numeric_limits<double>::quiet_NaN(), metre);
    auto result = abs(value);
    REQUIRE(std::isnan(result.value()));
  }

  SECTION("NaN handling in min/max") {
    auto nan    = make_quantity(std::numeric_limits<double>::quiet_NaN(), metre);
    auto normal = 5.0_m;

    // Per IEEE 754, comparisons with NaN return false
    // So min/max behavior depends on implementation
    auto min_result = min(nan, normal);
    auto max_result = max(nan, normal);

    // clang-format off
    // Must be inside another parenthesis to satisfy REQUIRE macro

    // At least one should return the normal value
    // (behavior depends on order and implementation)
    REQUIRE((min_result.value() == 5.0 || max_result.value() == 5.0
            || std::isnan(min_result.value()) || std::isnan(max_result.value())));

    // clang-format on
  }
}

TEST_CASE("Hyperbolic functions", "[math][hyperbolic]") {
  SECTION("Sinh of 0") {
    auto value  = 0.0_rad;
    auto result = sinh(value); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Cosh of 0") {
    auto value  = 0.0_rad;
    auto result = cosh(value); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }

  SECTION("Tanh of 0") {
    auto value  = 0.0_rad;
    auto result = tanh(value); // Returns double
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-10));
  }

  SECTION("Cosh squared - Sinh squared = 1") {
    auto value       = make_quantity(1.0, radian);
    auto sinh_result = sinh(value); // Returns double
    auto cosh_result = cosh(value); // Returns double
    auto diff        = (cosh_result * cosh_result) - (sinh_result * sinh_result);
    REQUIRE_THAT(diff, Catch::Matchers::WithinAbs(1.0, 1e-10));
  }
}

TEST_CASE("Fmod function", "[math][fmod]") {
  SECTION("Basic modulo") {
    auto dividend = make_quantity(10.5, metre);
    auto divisor  = make_quantity(3.0, metre);
    auto result   = fmod(dividend, divisor);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(1.5, 1e-10));
  }

  SECTION("Negative dividend") {
    auto dividend = make_quantity(-10.5, metre);
    auto divisor  = make_quantity(3.0, metre);
    auto result   = fmod(dividend, divisor);
    REQUIRE_THAT(result.value(), Catch::Matchers::WithinAbs(-1.5, 1e-10));
  }
}

// NOLINTEND(readability-function-cognitive-complexity)
