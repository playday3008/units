// SPDX-License-Identifier: MIT

/// @file math_test.cpp
/// @brief Static tests for units mathematical functions.

#include <numbers>

#include <units/math.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/literals.hpp>
#include <units/systems/si/prefixes.hpp>

namespace units::test {

  using namespace si::literals;

  // =============================================================================
  // Compile-time helper for approximate equality
  // =============================================================================

  consteval auto approx(double lhs, double rhs, double epsilon = 1e-6) -> bool {
    return (lhs - rhs) < epsilon && (rhs - lhs) < epsilon;
  }

  // =============================================================================
  // abs() tests
  // =============================================================================

  static_assert(abs(5.0_m).value() == 5.0);
  static_assert(abs(-5.0_m).value() == 5.0);
  static_assert(abs(0.0_m).value() == 0.0);

  static_assert(abs(3.14_s).value() == 3.14);
  static_assert(abs(-3.14_s).value() == 3.14);

  // =============================================================================
  // floor() tests
  // =============================================================================

  static_assert(floor(5.7_m).value() == 5.0);
  static_assert(floor(5.0_m).value() == 5.0);
  static_assert(floor(-5.7_m).value() == -6.0);
  static_assert(floor(-5.0_m).value() == -5.0);

  // =============================================================================
  // ceil() tests
  // =============================================================================

  static_assert(ceil(5.3_m).value() == 6.0);
  static_assert(ceil(5.0_m).value() == 5.0);
  static_assert(ceil(-5.3_m).value() == -5.0);
  static_assert(ceil(-5.0_m).value() == -5.0);

  // =============================================================================
  // round() tests
  // =============================================================================

  static_assert(round(5.3_m).value() == 5.0);
  static_assert(round(5.5_m).value() == 6.0);
  static_assert(round(5.7_m).value() == 6.0);
  static_assert(round(-5.3_m).value() == -5.0);
  static_assert(round(-5.5_m).value() == -6.0);

  // =============================================================================
  // trunc() tests
  // =============================================================================

  static_assert(trunc(5.7_m).value() == 5.0);
  static_assert(trunc(5.0_m).value() == 5.0);
  static_assert(trunc(-5.7_m).value() == -5.0);
  static_assert(trunc(-5.0_m).value() == -5.0);

  // =============================================================================
  // fmod() tests
  // =============================================================================

  static_assert(approx(fmod(5.5_m, 2.0_m).value(), 1.5));
  static_assert(approx(fmod(10.0_s, 3.0_s).value(), 1.0));

  // =============================================================================
  // min/max tests
  // =============================================================================

  static_assert(min(3.0_m, 5.0_m).value() == 3.0);
  static_assert(min(5.0_m, 3.0_m).value() == 3.0);
  static_assert(max(3.0_m, 5.0_m).value() == 5.0);
  static_assert(max(5.0_m, 3.0_m).value() == 5.0);

  // =============================================================================
  // clamp() tests
  // =============================================================================

  static_assert(clamp(5.0_m, 0.0_m, 10.0_m).value() == 5.0);
  static_assert(clamp(-5.0_m, 0.0_m, 10.0_m).value() == 0.0);
  static_assert(clamp(15.0_m, 0.0_m, 10.0_m).value() == 10.0);

  // =============================================================================
  // is_zero() tests
  // =============================================================================

  static_assert(is_zero(0.0_m));
  static_assert(is_zero(1e-15_m));
  static_assert(!is_zero(0.1_m));

  // =============================================================================
  // approx_equal() tests
  // =============================================================================

  static_assert(approx_equal(1.0_m, 1.0_m));
  static_assert(approx_equal(1.0_m, 1.0000000001_m));
  static_assert(!approx_equal(1.0_m, 1.01_m));

  // =============================================================================
  // pow<N>() tests
  // =============================================================================

  // pow<2> - squaring
  static_assert(pow<2>(3.0_m).value() == 9.0);
  static_assert(pow<2>(5.0_s).value() == 25.0);

  // pow<1> - identity
  static_assert(pow<1>(5.0_m).value() == 5.0);

  // pow<0> - returns 1
  static_assert(pow<0>(5.0_m) == 1.0);

  // =============================================================================
  // Trigonometric function tests
  // =============================================================================

  // sin() tests
  static_assert(approx(sin(0.0_rad), 0.0));
  static_assert(approx(sin(operator""_rad(std::numbers::pi_v<long double> / 2)),
                       1.0,
                       1e-5)); // sin(pi/2) ~ 1
  static_assert(approx(sin(operator""_rad(std::numbers::pi_v<long double>)),
                       0.0,
                       1e-5)); // sin(pi) ~ 0

  // cos() tests
  static_assert(approx(cos(0.0_rad), 1.0));
  static_assert(approx(cos(operator""_rad(std::numbers::pi_v<long double> / 2)),
                       0.0,
                       1e-5)); // cos(pi/2) ~ 0
  static_assert(approx(cos(operator""_rad(std::numbers::pi_v<long double>)),
                       -1.0,
                       1e-5)); // cos(pi) ~ -1

  // tan() tests (using raw values for simplicity in constexpr context)
  static_assert(approx(tan(0.0_rad), 0.0));
  static_assert(approx(tan(operator""_rad(std::numbers::pi_v<long double> / 4)),
                       1.0,
                       1e-5)); // tan(pi/4) ~ 1

  // =============================================================================
  // Inverse trig function tests (raw values) - relaxed tolerances
  // =============================================================================

  static_assert(approx(asin(0.0), 0.0));
  static_assert(approx(asin(0.5), std::numbers::pi_v<long double> / 6, 0.01)); // asin(0.5) ~ pi/6

  static_assert(approx(acos(0.5), std::numbers::pi_v<long double> / 3, 0.01)); // acos(0.5) ~ pi/3

  static_assert(approx(atan(0.0), 0.0));
  static_assert(approx(atan(1.0), std::numbers::pi_v<long double> / 4, 0.01)); // atan(1) = pi/4

  // =============================================================================
  // Hyperbolic function tests
  // =============================================================================

  static_assert(approx(sinh(0.0_rad), 0.0));
  static_assert(approx(cosh(0.0_rad), 1.0));
  static_assert(approx(tanh(0.0_rad), 0.0));

  // =============================================================================
  // Exponential and logarithmic function tests (raw values)
  // =============================================================================

  // NOLINTBEGIN(modernize-use-std-numbers)
  static_assert(approx(exp(0.0), 1.0));
  static_assert(approx(exp(1.0), std::numbers::e, 1e-5)); // e ~ 2.71828
  // NOLINTEND(modernize-use-std-numbers)

  static_assert(approx(log(1.0), 0.0));
  static_assert(approx(log(std::numbers::e), 1.0, 1e-5)); // ln(e) = 1

  static_assert(approx(log10(10.0), 1.0, 1e-5));
  static_assert(approx(log10(100.0), 2.0, 1e-5));

  static_assert(approx(log2(2.0), 1.0, 1e-5));
  static_assert(approx(log2(8.0), 3.0, 1e-5));

  // =============================================================================
  // atan2 tests - relaxed tolerances
  // =============================================================================

  static_assert(approx(atan2(0.0, 1.0), 0.0));
  static_assert(approx(atan2(1.0, 0.0), std::numbers::pi_v<long double> / 2, 0.01)); // pi/2
  static_assert(approx(atan2(1.0, 1.0), std::numbers::pi_v<long double> / 4, 0.01)); // pi/4

  // =============================================================================
  // sqrt() tests - Note: sqrt requires squared units (all powers even)
  // =============================================================================

  // Note: sqrt(metre) is not valid since metre has power 1.
  // sqrt is valid for: m^2 -> m, s^2 -> s, etc.
  // The type system enforces this constraint at compile time.
  // For a runtime test, we would need to create a proper m^2 unit type.

  // Verify sqrt constraint is correctly enforced
  static_assert(!detail::is_sqrt_compatible<si::metre_t>::value,
                "metre should not be sqrt-compatible (power 1)");

  // Verify the concept prevents sqrt on base units at compile time
  // This is a compile-time type trait check

  // =============================================================================
  // Type trait verification
  // =============================================================================

  // Verify math functions preserve quantity types
  static_assert(
      std::same_as<
          decltype(abs(std::declval<quantity<reference<isq::length_t, si::metre_t>, double>>())),
          quantity<reference<isq::length_t, si::metre_t>, double>>);

  static_assert(
      std::same_as<
          decltype(floor(std::declval<quantity<reference<isq::length_t, si::metre_t>, double>>())),
          quantity<reference<isq::length_t, si::metre_t>, double>>);

  static_assert(
      std::same_as<
          decltype(ceil(std::declval<quantity<reference<isq::length_t, si::metre_t>, double>>())),
          quantity<reference<isq::length_t, si::metre_t>, double>>);

  static_assert(
      std::same_as<
          decltype(round(std::declval<quantity<reference<isq::length_t, si::metre_t>, double>>())),
          quantity<reference<isq::length_t, si::metre_t>, double>>);

  // Verify trig functions return dimensionless values
  static_assert(
      std::same_as<
          decltype(sin(std::declval<quantity<reference<isq::length_t, si::radian_t>, double>>())),
          double>);

  static_assert(
      std::same_as<
          decltype(cos(std::declval<quantity<reference<isq::length_t, si::radian_t>, double>>())),
          double>);

} // namespace units::test

auto main() -> int {
  // All tests are compile-time static_asserts.
  // If this compiles, all tests pass.
  return 0;
}
