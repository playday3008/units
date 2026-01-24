// SPDX-License-Identifier: MIT

// Static tests for quantity_point (affine quantities)
// If this file compiles, all tests pass.

#include <units/systems/si/temperature.hpp>

namespace {

  using namespace units;
  using namespace units::si;

  // =============================================================================
  // Basic quantity_point tests
  // =============================================================================

  // Type aliases work correctly
  static_assert(sizeof(kelvin_point<double>) == sizeof(double));
  static_assert(sizeof(celsius_point<double>) == sizeof(double));
  static_assert(sizeof(fahrenheit_point<double>) == sizeof(double));

  // Factory functions work
  constexpr auto temp_k = kelvin_pt(300.0);
  constexpr auto temp_c = celsius_pt(25.0);
  constexpr auto temp_f = fahrenheit_pt(77.0);

  // Value access
  static_assert(temp_k.value() == 300.0);
  static_assert(temp_c.value() == 25.0);
  static_assert(temp_f.value() == 77.0);

  // =============================================================================
  // Point arithmetic tests
  // =============================================================================

  // point - point = quantity (displacement)
  constexpr auto k1      = kelvin_pt(350.0);
  constexpr auto k2      = kelvin_pt(300.0);
  constexpr auto delta_k = k1 - k2;
  static_assert(delta_k.value() == 50.0);

  // point + quantity = point
  constexpr auto k_delta = quantity<kelvin_ref, double> { 20.0 };
  constexpr auto k_plus  = k2 + k_delta;
  static_assert(k_plus.value() == 320.0);

  // point - quantity = point
  constexpr auto k_minus = k2 - k_delta;
  static_assert(k_minus.value() == 280.0);

  // =============================================================================
  // Celsius tests
  // =============================================================================

  constexpr auto c1 = celsius_pt(0.0);   // 0°C = freezing point
  constexpr auto c2 = celsius_pt(100.0); // 100°C = boiling point

  // Celsius differences
  constexpr auto c_diff = c2 - c1;
  static_assert(c_diff.value() == 100.0); // 100 degC difference

  // Celsius to Kelvin conversion
  // 0°C == 273.15 K
  constexpr auto freezing_k = to_kelvin(celsius_pt(0.0));
  static_assert(freezing_k.value() == 273.15);

  // 100°C == 373.15 K
  constexpr auto boiling_k = to_kelvin(celsius_pt(100.0));
  static_assert(boiling_k.value() == 373.15);

  // Kelvin to Celsius conversion
  constexpr auto abs_zero_c = to_celsius(kelvin_pt(0.0));
  static_assert(abs_zero_c.value() == -273.15);

  constexpr auto room_c = to_celsius(kelvin_pt(293.15));
  static_assert(room_c.value() == 20.0);

  // =============================================================================
  // Fahrenheit tests
  // =============================================================================

  // 32°F == 0°C (freezing point of water)
  constexpr auto freezing_f      = fahrenheit_pt(32.0);
  constexpr auto freezing_f_to_c = to_celsius(freezing_f);

  // Due to floating point precision, use approximate comparison
  constexpr auto approx_equal(double lhs, double rhs, double epsilon = 1e-10) -> bool {
    return (lhs - rhs) < epsilon && (rhs - lhs) < epsilon;
  }

  static_assert(approx_equal(freezing_f_to_c.value(), 0.0));

  // 212°F == 100°C (boiling point of water)
  constexpr auto boiling_f      = fahrenheit_pt(212.0);
  constexpr auto boiling_f_to_c = to_celsius(boiling_f);
  static_assert(approx_equal(boiling_f_to_c.value(), 100.0));

  // Celsius to Fahrenheit
  // 0°C == 32°F
  constexpr auto freezing_c_to_f = to_fahrenheit(celsius_pt(0.0));
  static_assert(approx_equal(freezing_c_to_f.value(), 32.0));

  // 100°C == 212°F
  constexpr auto boiling_c_to_f = to_fahrenheit(celsius_pt(100.0));
  static_assert(approx_equal(boiling_c_to_f.value(), 212.0));

  // =============================================================================
  // Round-trip conversion tests
  // =============================================================================

  // Kelvin -> Celsius -> Kelvin
  constexpr auto k_original  = kelvin_pt(350.0);
  constexpr auto k_to_c      = to_celsius(k_original);
  constexpr auto k_roundtrip = to_kelvin(k_to_c);
  static_assert(approx_equal(k_roundtrip.value(), k_original.value()));

  // Celsius -> Fahrenheit -> Celsius
  constexpr auto c_original  = celsius_pt(25.0);
  constexpr auto c_to_f      = to_fahrenheit(c_original);
  constexpr auto c_roundtrip = to_celsius(c_to_f);
  static_assert(approx_equal(c_roundtrip.value(), c_original.value()));

  // Kelvin -> Fahrenheit -> Kelvin
  constexpr auto k2_original  = kelvin_pt(300.0);
  constexpr auto k2_to_f      = to_fahrenheit(k2_original);
  constexpr auto k2_roundtrip = to_kelvin(k2_to_f);
  static_assert(approx_equal(k2_roundtrip.value(), k2_original.value()));

  // =============================================================================
  // Special temperature values
  // =============================================================================

  // Absolute zero in Celsius
  constexpr auto abs_zero_kelvin  = kelvin_pt(0.0);
  constexpr auto abs_zero_celsius = to_celsius(abs_zero_kelvin);
  static_assert(approx_equal(abs_zero_celsius.value(), -273.15));

  // Absolute zero in Fahrenheit
  constexpr auto abs_zero_fahrenheit = to_fahrenheit(abs_zero_kelvin);
  static_assert(approx_equal(abs_zero_fahrenheit.value(), -459.67));

  // Body temperature: 37°C = 98.6°F
  constexpr auto body_temp_c = celsius_pt(37.0);
  constexpr auto body_temp_f = to_fahrenheit(body_temp_c);
  static_assert(approx_equal(body_temp_f.value(), 98.6, 0.01)); // Slightly larger epsilon

  // =============================================================================
  // Comparison operators
  // =============================================================================

  // Same origin comparisons
  constexpr auto t1 = kelvin_pt(300.0);
  constexpr auto t2 = kelvin_pt(300.0);
  constexpr auto t3 = kelvin_pt(350.0);

  static_assert(t1 == t2);
  static_assert(!(t1 == t3));
  static_assert(t1 < t3);
  static_assert(t3 > t1);
  static_assert(t1 <= t2);
  static_assert(t1 >= t2);
  static_assert(t1 <= t3);
  static_assert(t3 >= t1);

  // Celsius comparisons
  constexpr auto tc1 = celsius_pt(20.0);
  constexpr auto tc2 = celsius_pt(25.0);
  static_assert(tc1 < tc2);
  static_assert(tc2 > tc1);

  // =============================================================================
  // Origin type traits
  // =============================================================================

  static_assert(PointOrigin<absolute_zero_t>);
  static_assert(PointOrigin<ice_point_t>);
  static_assert(PointOrigin<fahrenheit_zero_t>);

  static_assert(is_absolute_origin_v<absolute_zero_t>);
  static_assert(is_relative_origin_v<ice_point_t>);
  static_assert(is_relative_origin_v<fahrenheit_zero_t>);

  // =============================================================================
  // QuantityPoint concept
  // =============================================================================

  static_assert(QuantityPoint<kelvin_point<double>>);
  static_assert(QuantityPoint<celsius_point<double>>);
  static_assert(QuantityPoint<fahrenheit_point<double>>);

  static_assert(!QuantityPoint<int>);
  static_assert(!QuantityPoint<double>);
  static_assert(!QuantityPoint<quantity<kelvin_ref, double>>);

  // =============================================================================
  // Zero-overhead verification
  // =============================================================================

  // quantity_point should be trivially copyable
  static_assert(std::is_trivially_copyable_v<kelvin_point<double>>);
  static_assert(std::is_trivially_copyable_v<celsius_point<double>>);
  static_assert(std::is_trivially_copyable_v<fahrenheit_point<double>>);

  // quantity_point should have standard layout
  static_assert(std::is_standard_layout_v<kelvin_point<double>>);
  static_assert(std::is_standard_layout_v<celsius_point<double>>);
  static_assert(std::is_standard_layout_v<fahrenheit_point<double>>);

  // =============================================================================
  // Different representation types
  // =============================================================================

  // Float representation
  constexpr auto temp_f32 = kelvin_point<float> { quantity<kelvin_ref, float> { 300.0F } };
  static_assert(sizeof(temp_f32) == sizeof(float));
  static_assert(temp_f32.value() == 300.0F);

  // Long double representation
  constexpr auto temp_ld
      = kelvin_point<long double> { quantity<kelvin_ref, long double> { 300.0L } };
  static_assert(sizeof(temp_ld) == sizeof(long double));

} // anonymous namespace

// Main function (required for executable, but all tests are static)
auto main() -> int {
  // All tests are compile-time static_asserts
  // If we got here, everything passed
  return 0;
}
