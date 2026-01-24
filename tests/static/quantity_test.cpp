// SPDX-License-Identifier: MIT

// Comprehensive compile-time tests for the quantity class.
// If this file compiles, all tests pass.

#include <type_traits>

#include <units/core/quantity.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/literals.hpp>
#include <units/systems/si/prefixes.hpp>

namespace units::test {

  using namespace units::si;
  using namespace units::si::literals;

  // =============================================================================
  // Zero-overhead guarantees
  // =============================================================================

  // Verify quantity has the same size as its representation
  static_assert(sizeof(quantity<reference<isq::length_t, metre_t>, double>) == sizeof(double));
  static_assert(sizeof(quantity<reference<isq::length_t, metre_t>, float>) == sizeof(float));
  static_assert(sizeof(quantity<reference<isq::length_t, metre_t>, int>) == sizeof(int));

  // Verify trivially copyable
  static_assert(std::is_trivially_copyable_v<quantity<reference<isq::length_t, metre_t>, double>>);

  // Verify standard layout
  static_assert(std::is_standard_layout_v<quantity<reference<isq::length_t, metre_t>, double>>);

  // Verify noexcept operations
  static_assert(
      std::is_nothrow_copy_constructible_v<quantity<reference<isq::length_t, metre_t>, double>>);
  static_assert(
      std::is_nothrow_move_constructible_v<quantity<reference<isq::length_t, metre_t>, double>>);
  static_assert(
      std::is_nothrow_copy_assignable_v<quantity<reference<isq::length_t, metre_t>, double>>);
  static_assert(
      std::is_nothrow_move_assignable_v<quantity<reference<isq::length_t, metre_t>, double>>);

  // =============================================================================
  // Quantity concept verification
  // =============================================================================

  static_assert(Quantity<quantity<reference<isq::length_t, metre_t>, double>>);
  static_assert(!Quantity<double>);
  static_assert(!Quantity<int>);
  static_assert(!Quantity<metre_t>);

  // =============================================================================
  // Basic construction and value access
  // =============================================================================

  namespace construction_tests {

    // Can construct with explicit value
    constexpr auto len1 = quantity<reference<isq::length_t, metre_t>, double> { 5.0 };
    static_assert(len1.value() == 5.0);

    // Using literals
    constexpr auto len2 = 10_m;
    static_assert(len2.value() == 10.0);

    constexpr auto len3 = 5.5_m;
    static_assert(len3.value() == 5.5);

    // Different units
    constexpr auto len_km = 2_km;
    static_assert(len_km.value() == 2.0);

    constexpr auto time_s = 3_s;
    static_assert(time_s.value() == 3.0);

    constexpr auto mass_kg = 10_kg;
    static_assert(mass_kg.value() == 10.0);

  } // namespace construction_tests

  // =============================================================================
  // Unit conversions
  // =============================================================================

  namespace conversion_tests {

    // kilometre to metre conversion factor
    static_assert(conversion_factor<kilometre_t, metre_t>() == 1000.0);

    // centimetre to metre conversion factor
    static_assert(conversion_factor<centimetre_t, metre_t>() == 0.01);

    // millimetre to metre conversion factor
    static_assert(conversion_factor<millimetre_t, metre_t>() == 0.001);

    // Convert 1 km to metres
    constexpr auto km_val    = 1_km;
    constexpr auto m_from_km = km_val.in(metre);
    static_assert(m_from_km.value() == 1000.0);

    // Convert 100 cm to metres
    constexpr auto cm_val    = 100_cm;
    constexpr auto m_from_cm = cm_val.in(metre);
    static_assert(m_from_cm.value() == 1.0);

    // numerical_value_in test
    constexpr auto km_val2 = 2.5_km;
    static_assert(km_val2.numerical_value_in(metre) == 2500.0);
    static_assert(km_val2.numerical_value_in(kilometre) == 2.5);

  } // namespace conversion_tests

  // =============================================================================
  // Arithmetic operations - same unit
  // =============================================================================

  namespace same_unit_arithmetic {

    // Addition
    constexpr auto sum = 5_m + 3_m;
    static_assert(sum.value() == 8.0);

    // Subtraction
    constexpr auto diff = 10_m - 4_m;
    static_assert(diff.value() == 6.0);

    // Unary minus
    constexpr auto neg = -(5_m);
    static_assert(neg.value() == -5.0);

    // Unary plus
    constexpr auto pos = +(5_m);
    static_assert(pos.value() == 5.0);

    // Multiply by scalar (quantity * scalar)
    constexpr auto scaled1 = 5_m * 3.0;
    static_assert(scaled1.value() == 15.0);

    // Multiply by scalar (scalar * quantity)
    constexpr auto scaled2 = 2.0 * 10_m;
    static_assert(scaled2.value() == 20.0);

    // Divide by scalar
    constexpr auto divided = 12_m / 4.0;
    static_assert(divided.value() == 3.0);

  } // namespace same_unit_arithmetic

  // =============================================================================
  // Arithmetic operations - different compatible units
  // =============================================================================

  namespace cross_unit_arithmetic {

    // 1 km + 500 m should work and give result in first unit's base
    // Note: The result uses the first operand's unit
    constexpr auto sum1 = 1_km + make_quantity(500.0, metre);
    // Sum should be 1500 when viewed in metres
    static_assert(sum1.in(metre).value() == 1500.0);

    // 2 km - 500 m
    constexpr auto diff1 = make_quantity(2.0, kilometre) - make_quantity(500.0, metre);
    static_assert(diff1.in(metre).value() == 1500.0);

  } // namespace cross_unit_arithmetic

  // =============================================================================
  // Comparison operations
  // =============================================================================

  namespace comparison_tests {

    // Same unit comparisons
    static_assert(5_m == 5_m);
    static_assert(5_m != 6_m);
    static_assert(5_m < 6_m);
    static_assert(6_m > 5_m);
    static_assert(5_m <= 5_m);
    static_assert(5_m <= 6_m);
    static_assert(6_m >= 6_m);
    static_assert(6_m >= 5_m);

    // Cross-unit comparisons (with conversion)
    constexpr auto one_km     = 1_km;
    constexpr auto thousand_m = make_quantity(1000.0, metre);
    static_assert(one_km.in(metre).value() == thousand_m.value());

    // Verify 1 km == 1000 m using in() for explicit conversion
    static_assert(one_km.in(metre) == thousand_m);

    // Three-way comparison
    static_assert((5_m <=> 6_m) < 0);
    static_assert((6_m <=> 5_m) > 0);
    static_assert((5_m <=> 5_m) == 0);

  } // namespace comparison_tests

  // =============================================================================
  // Multiplication and division creating derived quantities
  // =============================================================================

  namespace derived_quantity_tests {

    // Distance / time = speed
    constexpr auto distance = 100_m;
    constexpr auto time     = 10_s;
    constexpr auto speed    = distance / time;
    static_assert(speed.value() == 10.0);

    // Length * length = area
    constexpr auto len1 = 2_m;
    constexpr auto len2 = 3_m;
    constexpr auto area = len1 * len2;
    static_assert(area.value() == 6.0);

    // More complex: (mass * length) / (time * time) proportional to force
    constexpr auto mass       = 2_kg;
    constexpr auto accel_len  = 3_m;
    constexpr auto time1      = 1_s;
    constexpr auto time2      = 1_s;
    constexpr auto force_like = (mass * accel_len) / (time1 * time2);
    static_assert(force_like.value() == 6.0);

  } // namespace derived_quantity_tests

  // =============================================================================
  // Type safety verification
  // =============================================================================

  namespace type_safety_tests {

    // These should NOT compile (commented out to verify they fail)
    // Uncomment individually to verify compile errors:

    // Cannot add length + time
    // constexpr auto bad1 = 1_m + 1_s;  // Should fail

    // Cannot compare length with time
    // constexpr bool bad2 = (1_m == 1_s);  // Should fail

    // Cannot add length + mass
    // constexpr auto bad3 = 1_m + 1_kg;  // Should fail

  } // namespace type_safety_tests

  // =============================================================================
  // Reference type verification
  // =============================================================================

  namespace reference_tests {

    // Check reference from unit
    using metre_ref = reference_from_unit<metre_t>;
    static_assert(Reference<metre_ref>);
    static_assert(std::same_as<get_unit<metre_ref>, metre_t>);
    static_assert(std::same_as<get_quantity_spec<metre_ref>, isq::length_t>);

    // Check reference compatibility
    static_assert(references_compatible_v<reference<isq::length_t, metre_t>,
                                          reference<isq::length_t, kilometre_t>>);

  } // namespace reference_tests

  // =============================================================================
  // Make quantity factory function
  // =============================================================================

  namespace factory_tests {

    constexpr auto len = make_quantity(5.0, metre);
    static_assert(len.value() == 5.0);

    constexpr auto time = make_quantity(10, second);
    static_assert(time.value() == 10);

    constexpr auto mass = make_quantity(2.5F, kilogram);
    static_assert(mass.value() == 2.5F);

  } // namespace factory_tests

  // =============================================================================
  // Compound assignment
  // =============================================================================

  namespace compound_assignment_tests {

    static constexpr auto test_add_assign() {
      auto val  = 5_m;
      val      += 3_m;
      return val.value();
    }
    static_assert(test_add_assign() == 8.0);

    static constexpr auto test_sub_assign() {
      auto val  = 10_m;
      val      -= 3_m;
      return val.value();
    }
    static_assert(test_sub_assign() == 7.0);

    static constexpr auto test_mul_assign() {
      auto val  = 5_m;
      val      *= 2.0;
      return val.value();
    }
    static_assert(test_mul_assign() == 10.0);

    static constexpr auto test_div_assign() {
      auto val  = 10_m;
      val      /= 2.0;
      return val.value();
    }
    static_assert(test_div_assign() == 5.0);

  } // namespace compound_assignment_tests

  // =============================================================================
  // Integration test: realistic calculations
  // =============================================================================

  namespace integration_tests {

    // Calculate kinetic energy: KE = 0.5 * m * v^2
    // Using v = distance / time
    static constexpr auto calc_kinetic_energy() {
      auto distance = 100_m;
      auto time     = 10_s;
      auto velocity = distance / time; // 10 m/s
      auto mass     = 2_kg;

      // v^2 needs velocity * velocity
      auto v_squared = velocity * velocity; // 100 m^2/s^2

      // KE = 0.5 * m * v^2
      auto ke = 0.5 * mass * v_squared; // 100 J (in kg*m^2/s^2)

      return ke.value();
    }
    static_assert(calc_kinetic_energy() == 100.0);

    // Calculate average speed
    static constexpr auto calc_avg_speed() {
      auto total_distance = 1_km;
      auto total_time     = make_quantity(100.0, second);
      auto speed          = total_distance.in(metre) / total_time;
      return speed.value();
    }
    static_assert(calc_avg_speed() == 10.0); // 10 m/s

  } // namespace integration_tests

} // namespace units::test

// Main function - if this compiles, all static tests pass
auto main() -> int {
  return 0;
}
