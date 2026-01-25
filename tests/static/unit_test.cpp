// SPDX-License-Identifier: MIT

// Static assertion tests for units, references, and conversions
// All tests execute at compile time via static_assert

#include <units/core/ratio.hpp>
#include <units/core/reference.hpp>
#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/prefixes.hpp>

#include <concepts>

namespace units::test {

  // =============================================================================
  // fixed_string tests
  // =============================================================================

  // Basic construction and comparison
  static_assert(fixed_string { "m" } == fixed_string { "m" });
  static_assert(fixed_string { "kg" } == fixed_string { "kg" });
  static_assert(!(fixed_string { "m" } == fixed_string { "kg" }));
  static_assert(!(fixed_string { "mm" } == fixed_string { "m" }));

  // Size
  static_assert(fixed_string { "m" }.size() == 1);
  static_assert(fixed_string { "kg" }.size() == 2);
  static_assert(fixed_string { "mol" }.size() == 3);

  // =============================================================================
  // Magnitude tests
  // =============================================================================

  // Basic magnitude construction
  static_assert(mag<1>::num == 1);
  static_assert(mag<1>::den == 1);
  static_assert(mag<1000>::num == 1000);
  static_assert(mag<1000>::den == 1);

  // Magnitude from ratio
  static_assert(mag_ratio<1, 1000>::num == 1);
  static_assert(mag_ratio<1, 1000>::den == 1000);

  // mag_one
  static_assert(mag_one::num == 1);
  static_assert(mag_one::den == 1);

  // Magnitude equality
  static_assert(mag_equal_v<mag_one, mag<1>>);
  static_assert(mag_equal_v<mag<1000>, magnitude<kilo>>);
  static_assert(!mag_equal_v<mag<1>, mag<1000>>);

  // Magnitude arithmetic
  static_assert(mag_equal_v<mag_multiply<mag<10>, mag<100>>, mag<1000>>);
  static_assert(mag_equal_v<mag_divide<mag<1000>, mag<10>>, mag<100>>);
  static_assert(mag_equal_v<mag_power<mag<10>, 3>, mag<1000>>);
  static_assert(mag_equal_v<mag_power<mag<10>, 0>, mag<1>>);

  // Magnitude value
  static_assert(mag<1000>::value() == 1000.0);
  static_assert(mag_ratio<1, 1000>::value() == 0.001);

  // =============================================================================
  // Unit concept tests
  // =============================================================================

  // SI base units satisfy Unit concept
  static_assert(Unit<si::metre_t>);
  static_assert(Unit<si::kilogram_t>);
  static_assert(Unit<si::second_t>);
  static_assert(Unit<si::ampere_t>);
  static_assert(Unit<si::kelvin_t>);
  static_assert(Unit<si::mole_t>);
  static_assert(Unit<si::candela_t>);
  static_assert(Unit<si::gram_t>);

  // SI base units satisfy NamedUnit concept
  static_assert(NamedUnit<si::metre_t>);
  static_assert(NamedUnit<si::kilogram_t>);
  static_assert(NamedUnit<si::second_t>);

  // SI derived units satisfy Unit concept
  static_assert(Unit<si::hertz_t>);
  static_assert(Unit<si::newton_t>);
  static_assert(Unit<si::pascal_t>);
  static_assert(Unit<si::joule_t>);
  static_assert(Unit<si::watt_t>);

  // Prefixed units satisfy Unit concept
  static_assert(Unit<si::kilometre_t>);
  static_assert(Unit<si::centimetre_t>);
  static_assert(Unit<si::millimetre_t>);
  static_assert(Unit<si::millisecond_t>);
  static_assert(Unit<si::milligram_t>);

  // =============================================================================
  // Unit symbol tests
  // =============================================================================

  // Base unit symbols
  static_assert(si::metre_t::symbol == fixed_string { "m" });
  static_assert(si::kilogram_t::symbol == fixed_string { "kg" });
  static_assert(si::second_t::symbol == fixed_string { "s" });
  static_assert(si::ampere_t::symbol == fixed_string { "A" });
  static_assert(si::kelvin_t::symbol == fixed_string { "K" });
  static_assert(si::mole_t::symbol == fixed_string { "mol" });
  static_assert(si::candela_t::symbol == fixed_string { "cd" });
  static_assert(si::gram_t::symbol == fixed_string { "g" });

  // Derived unit symbols
  static_assert(si::hertz_t::symbol == fixed_string { "Hz" });
  static_assert(si::newton_t::symbol == fixed_string { "N" });
  static_assert(si::joule_t::symbol == fixed_string { "J" });
  static_assert(si::watt_t::symbol == fixed_string { "W" });
  static_assert(si::pascal_t::symbol == fixed_string { "Pa" });

  // Prefixed unit symbols
  static_assert(si::kilometre_t::symbol == fixed_string { "km" });
  static_assert(si::centimetre_t::symbol == fixed_string { "cm" });
  static_assert(si::millimetre_t::symbol == fixed_string { "mm" });
  static_assert(si::millisecond_t::symbol == fixed_string { "ms" });

  // =============================================================================
  // Unit magnitude tests
  // =============================================================================

  // Base units have magnitude 1
  static_assert(mag_equal_v<get_magnitude<si::metre_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<si::kilogram_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<si::second_t>, mag_one>);

  // Derived units have magnitude 1 (coherent)
  static_assert(mag_equal_v<get_magnitude<si::newton_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<si::joule_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<si::watt_t>, mag_one>);

  // Gram has magnitude 1/1000 (relative to kilogram)
  static_assert(si::gram_t::magnitude_type::num == 1);
  static_assert(si::gram_t::magnitude_type::den == 1000);

  // Prefixed length units
  static_assert(si::kilometre_t::magnitude_type::num == 1000);
  static_assert(si::kilometre_t::magnitude_type::den == 1);

  static_assert(si::centimetre_t::magnitude_type::num == 1);
  static_assert(si::centimetre_t::magnitude_type::den == 100);

  static_assert(si::millimetre_t::magnitude_type::num == 1);
  static_assert(si::millimetre_t::magnitude_type::den == 1000);

  // Prefixed mass units (applied to gram, not kilogram)
  static_assert(si::milligram_t::magnitude_type::num == 1);
  static_assert(si::milligram_t::magnitude_type::den == 100'0000); // 10^-3 * 10^-3

  // =============================================================================
  // Conversion factor tests
  // =============================================================================

  // Same unit has conversion factor 1
  static_assert(conversion_factor<si::metre_t, si::metre_t>() == 1.0);
  static_assert(conversion_factor<si::kilogram_t, si::kilogram_t>() == 1.0);
  static_assert(conversion_factor<si::second_t, si::second_t>() == 1.0);

  // Length conversions
  static_assert(conversion_factor<si::kilometre_t, si::metre_t>() == 1000.0);
  static_assert(conversion_factor<si::metre_t, si::kilometre_t>() == 0.001);
  static_assert(conversion_factor<si::centimetre_t, si::metre_t>() == 0.01);
  static_assert(conversion_factor<si::metre_t, si::centimetre_t>() == 100.0);
  static_assert(conversion_factor<si::millimetre_t, si::metre_t>() == 0.001);
  static_assert(conversion_factor<si::metre_t, si::millimetre_t>() == 1000.0);

  // Cross-prefix conversions
  static_assert(conversion_factor<si::kilometre_t, si::centimetre_t>() == 100000.0);
  static_assert(conversion_factor<si::centimetre_t, si::millimetre_t>() == 10.0);
  static_assert(conversion_factor<si::millimetre_t, si::centimetre_t>() == 0.1);

  // Time conversions
  static_assert(conversion_factor<si::millisecond_t, si::second_t>() == 0.001);
  static_assert(conversion_factor<si::second_t, si::millisecond_t>() == 1000.0);
  static_assert(conversion_factor<si::microsecond_t, si::second_t>() == 0.000001);
  static_assert(conversion_factor<si::nanosecond_t, si::second_t>() == 0.000000001);

  // Mass conversions (gram-based)
  static_assert(conversion_factor<si::gram_t, si::kilogram_t>() == 0.001);
  static_assert(conversion_factor<si::kilogram_t, si::gram_t>() == 1000.0);
  static_assert(conversion_factor<si::milligram_t, si::gram_t>() == 0.001);
  static_assert(conversion_factor<si::milligram_t, si::kilogram_t>() == 0.000001);

  // =============================================================================
  // Reference tests
  // =============================================================================

  // Reference construction
  using length_in_metres = reference<isq::length_t, si::metre_t>;
  using length_in_km     = reference<isq::length_t, si::kilometre_t>;
  using time_in_seconds  = reference<isq::time_t, si::second_t>;
  using mass_in_kg       = reference<isq::mass_t, si::kilogram_t>;

  // Reference satisfies concept
  static_assert(Reference<length_in_metres>);
  static_assert(Reference<length_in_km>);
  static_assert(Reference<time_in_seconds>);
  static_assert(Reference<mass_in_kg>);

  // Reference accessors
  static_assert(std::same_as<get_quantity_spec<length_in_metres>, isq::length_t>);
  static_assert(std::same_as<get_unit<length_in_metres>, si::metre_t>);
  static_assert(std::same_as<get_quantity_spec<time_in_seconds>, isq::time_t>);
  static_assert(std::same_as<get_unit<time_in_seconds>, si::second_t>);

  // make_reference factory
  static_assert(Reference<decltype(make_reference(isq::length, si::metre))>);
  static_assert(Reference<decltype(make_reference(isq::time, si::second))>);

  // Reference from unit only (uses unit's quantity_kind)
  static_assert(Reference<reference_from_unit<si::metre_t>>);
  static_assert(Reference<reference_from_unit<si::second_t>>);

  // Reference conversion factor
  static_assert(reference_conversion_factor<length_in_km, length_in_metres>() == 1000.0);
  static_assert(reference_conversion_factor<length_in_metres, length_in_km>() == 0.001);

  // =============================================================================
  // SI prefix ratio tests
  // =============================================================================

  // Large prefixes
  static_assert(ratio_equal_v<si::kilo, ratio<1000>>);
  static_assert(ratio_equal_v<si::mega, ratio<100'0000>>);
  static_assert(ratio_equal_v<si::giga, ratio<10'0000'0000>>);
  static_assert(ratio_equal_v<si::tera, ratio<1'0000'0000'0000>>);
  static_assert(ratio_equal_v<si::peta, ratio<1000'0000'0000'0000>>);

  // Small prefixes
  static_assert(ratio_equal_v<si::milli, ratio<1, 1000>>);
  static_assert(ratio_equal_v<si::micro, ratio<1, 100'0000>>);
  static_assert(ratio_equal_v<si::nano, ratio<1, 10'0000'0000>>);
  static_assert(ratio_equal_v<si::pico, ratio<1, 1'0000'0000'0000>>);
  static_assert(ratio_equal_v<si::femto, ratio<1, 1000'0000'0000'0000>>);

  // Prefix relationships
  static_assert(ratio_equal_v<ratio_multiply<si::kilo, si::milli>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_multiply<si::mega, si::micro>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_multiply<si::giga, si::nano>, ratio<1>>);

  // Prefix powers
  static_assert(ratio_equal_v<ratio_power<si::kilo, 2>, si::mega>);
  static_assert(ratio_equal_v<ratio_power<si::kilo, 3>, si::giga>);
  static_assert(ratio_equal_v<ratio_power<si::milli, 2>, si::micro>);

  // =============================================================================
  // Unit arithmetic tests (basic - more complex tests when quantity is ready)
  // =============================================================================

  // Unit multiplication creates derived_unit
  using m_times_m = decltype(si::metre * si::metre);
  static_assert(Unit<m_times_m>);
  static_assert(DerivedUnit<m_times_m>);

  // Unit division creates derived_unit
  using m_per_s = decltype(si::metre / si::second);
  static_assert(Unit<m_per_s>);
  static_assert(DerivedUnit<m_per_s>);

  // Complex derived unit
  using m_per_s2 = decltype(si::metre / si::second / si::second);
  static_assert(Unit<m_per_s2>);
  static_assert(DerivedUnit<m_per_s2>);

  // kg*m/s² (force unit expression)
  using kg_m_per_s2 = decltype(si::kilogram * si::metre / si::second / si::second);
  static_assert(Unit<kg_m_per_s2>);
  static_assert(DerivedUnit<kg_m_per_s2>);

} // namespace units::test

// Main function - tests are all compile-time, so just need to compile successfully
auto main() -> int {
  return 0;
}
