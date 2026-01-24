// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>

namespace units::si {

  // =============================================================================
  // SI Base Units (BIPM)
  // =============================================================================
  // The seven SI base units as defined by the International Bureau of Weights
  // and Measures (BIPM). Each unit is associated with its ISQ quantity kind.

  /// metre - SI unit of length
  /// Symbol: m
  /// Definition: The distance light travels in vacuum in 1/299,792,458 second
  struct metre_t final : named_unit<"m", kind_of<isq::length_t>> {};
  inline constexpr metre_t metre {};

  // American spelling alias
  inline constexpr metre_t meter = metre;

  /// kilogram - SI unit of mass
  /// Symbol: kg
  /// Definition: Defined by fixing Planck constant h = 6.62607015×10⁻³⁴ J⋅s
  /// Note: kilogram is the base unit (not gram), but has 'kilo' prefix historically
  struct kilogram_t final : named_unit<"kg", kind_of<isq::mass_t>> {};
  inline constexpr kilogram_t kilogram {};

  /// second - SI unit of time
  /// Symbol: s
  /// Definition: Duration of 9,192,631,770 periods of Cs-133 radiation
  struct second_t final : named_unit<"s", kind_of<isq::time_t>> {};
  inline constexpr second_t second {};

  /// ampere - SI unit of electric current
  /// Symbol: A
  /// Definition: Flow of 1/(1.602176634×10⁻¹⁹) elementary charges per second
  struct ampere_t final : named_unit<"A", kind_of<isq::electric_current_t>> {};
  inline constexpr ampere_t ampere {};

  /// kelvin - SI unit of thermodynamic temperature
  /// Symbol: K
  /// Definition: Defined by fixing Boltzmann constant k = 1.380649×10⁻²³ J/K
  struct kelvin_t final : named_unit<"K", kind_of<isq::thermodynamic_temperature_t>> {};
  inline constexpr kelvin_t kelvin {};

  /// mole - SI unit of amount of substance
  /// Symbol: mol
  /// Definition: Exactly 6.02214076×10²³ elementary entities (Avogadro number)
  struct mole_t final : named_unit<"mol", kind_of<isq::amount_of_substance_t>> {};
  inline constexpr mole_t mole {};

  /// candela - SI unit of luminous intensity
  /// Symbol: cd
  /// Definition: Luminous efficacy of 540 THz radiation is 683 lm/W
  struct candela_t final : named_unit<"cd", kind_of<isq::luminous_intensity_t>> {};
  inline constexpr candela_t candela {};

  // =============================================================================
  // Gram - the unit from which kilogram is derived
  // =============================================================================

  /// gram - one thousandth of a kilogram
  /// Symbol: g
  /// Note: Although kilogram is the SI base unit, gram is useful for prefixes
  struct gram_t final : named_unit<"g", kind_of<isq::mass_t>, mag_ratio<1, 1000>> {};
  inline constexpr gram_t gram {};

  // =============================================================================
  // Static assertions to verify unit properties
  // =============================================================================

  // All base units satisfy Unit concept
  static_assert(Unit<metre_t>);
  static_assert(Unit<kilogram_t>);
  static_assert(Unit<second_t>);
  static_assert(Unit<ampere_t>);
  static_assert(Unit<kelvin_t>);
  static_assert(Unit<mole_t>);
  static_assert(Unit<candela_t>);
  static_assert(Unit<gram_t>);

  // All base units satisfy NamedUnit concept
  static_assert(NamedUnit<metre_t>);
  static_assert(NamedUnit<kilogram_t>);
  static_assert(NamedUnit<second_t>);
  static_assert(NamedUnit<ampere_t>);
  static_assert(NamedUnit<kelvin_t>);
  static_assert(NamedUnit<mole_t>);
  static_assert(NamedUnit<candela_t>);

  // Base units have magnitude 1
  static_assert(mag_equal_v<get_magnitude<metre_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<kilogram_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<second_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<ampere_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<kelvin_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<mole_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<candela_t>, mag_one>);

  // Gram has magnitude 1/1000 relative to kilogram
  static_assert(gram_t::magnitude_type::num == 1);
  static_assert(gram_t::magnitude_type::den == 1000);

  // Verify symbols are correct
  static_assert(metre_t::symbol == fixed_string { "m" });
  static_assert(kilogram_t::symbol == fixed_string { "kg" });
  static_assert(second_t::symbol == fixed_string { "s" });
  static_assert(ampere_t::symbol == fixed_string { "A" });
  static_assert(kelvin_t::symbol == fixed_string { "K" });
  static_assert(mole_t::symbol == fixed_string { "mol" });
  static_assert(candela_t::symbol == fixed_string { "cd" });
  static_assert(gram_t::symbol == fixed_string { "g" });

} // namespace units::si
