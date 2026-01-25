// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/quantity_spec.hpp>
#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/isq/mechanics.hpp>

namespace units::si {

  // =============================================================================
  // SI Derived Units with Special Names and Symbols (BIPM)
  // =============================================================================
  // The 22 SI derived units that have been given special names.
  // Each is defined in terms of the base units with appropriate quantity kinds.

  // -----------------------------------------------------------------------------
  // Dimensionless derived units
  // -----------------------------------------------------------------------------

  /// radian - unit of plane angle
  /// Symbol: rad
  /// Definition: m/m = 1 (dimensionless, but kept distinct for clarity)
  /// Represents the angle subtended at the center of a circle by an arc
  /// equal in length to the radius.
  struct radian_t final : named_unit<"rad", kind_of<isq::angle_t>> {};
  inline constexpr radian_t radian {};

  /// steradian - unit of solid angle
  /// Symbol: sr
  /// Definition: m²/m² = 1 (dimensionless, but kept distinct for clarity)
  /// Represents the solid angle subtended at the center of a sphere by an area
  /// on its surface equal to the square of the radius.
  struct steradian_t final : named_unit<"sr", kind_of<isq::solid_angle_t>> {};
  inline constexpr steradian_t steradian {};

  // -----------------------------------------------------------------------------
  // Frequency
  // -----------------------------------------------------------------------------

  /// hertz - unit of frequency
  /// Symbol: Hz
  /// Definition: 1/s = s⁻¹
  /// Named after Heinrich Hertz
  struct hertz_t final : named_unit<"Hz", kind_of<isq::frequency_t>> {};
  inline constexpr hertz_t hertz {};

  // -----------------------------------------------------------------------------
  // Mechanics - Force, Energy, Power, Pressure
  // -----------------------------------------------------------------------------

  /// newton - unit of force
  /// Symbol: N
  /// Definition: kg⋅m⋅s⁻² (mass × acceleration)
  /// Named after Isaac Newton
  struct newton_t final : named_unit<"N", kind_of<isq::force_t>> {};
  inline constexpr newton_t newton {};

  /// pascal - unit of pressure
  /// Symbol: Pa
  /// Definition: N/m² = kg⋅m⁻¹⋅s⁻² (force per unit area)
  /// Named after Blaise Pascal
  struct pascal_t final : named_unit<"Pa", kind_of<isq::pressure_t>> {};
  inline constexpr pascal_t pascal {};

  /// joule - unit of energy, work, heat
  /// Symbol: J
  /// Definition: N⋅m = kg⋅m²⋅s⁻² (force × distance)
  /// Named after James Prescott Joule
  struct joule_t final : named_unit<"J", kind_of<isq::energy_t>> {};
  inline constexpr joule_t joule {};

  /// watt - unit of power
  /// Symbol: W
  /// Definition: J/s = kg⋅m²⋅s⁻³ (energy per unit time)
  /// Named after James Watt
  struct watt_t final : named_unit<"W", kind_of<isq::power_t>> {};
  inline constexpr watt_t watt {};

  // -----------------------------------------------------------------------------
  // Electromagnetism
  // -----------------------------------------------------------------------------

  /// coulomb - unit of electric charge
  /// Symbol: C
  /// Definition: A⋅s (current × time)
  /// Named after Charles-Augustin de Coulomb
  struct coulomb_t final : named_unit<"C", kind_of<isq::electric_current_t>> {};
  inline constexpr coulomb_t coulomb {};

  /// volt - unit of electric potential difference (voltage)
  /// Symbol: V
  /// Definition: W/A = kg⋅m²⋅s⁻³⋅A⁻¹ (power per unit current)
  /// Named after Alessandro Volta
  struct volt_t final : named_unit<"V", kind_of<isq::energy_t>> {};
  inline constexpr volt_t volt {};

  /// farad - unit of electric capacitance
  /// Symbol: F
  /// Definition: C/V = s⁴⋅A²⋅kg⁻¹⋅m⁻² (charge per unit voltage)
  /// Named after Michael Faraday
  struct farad_t final : named_unit<"F", kind_of<isq::dimensionless_t>> {};
  inline constexpr farad_t farad {};

  /// ohm - unit of electric resistance
  /// Symbol: Ω (or Ohm in ASCII)
  /// Definition: V/A = kg⋅m²⋅s⁻³⋅A⁻² (voltage per unit current)
  /// Named after Georg Ohm
  struct ohm_t final : named_unit<"Ohm", kind_of<isq::dimensionless_t>> {};
  inline constexpr ohm_t ohm {};

  /// siemens - unit of electric conductance
  /// Symbol: S
  /// Definition: A/V = 1/Ω = s³⋅A²⋅kg⁻¹⋅m⁻² (inverse of resistance)
  /// Named after Werner von Siemens
  struct siemens_t final : named_unit<"S", kind_of<isq::dimensionless_t>> {};
  inline constexpr siemens_t siemens {};

  /// weber - unit of magnetic flux
  /// Symbol: Wb
  /// Definition: V⋅s = kg⋅m²⋅s⁻²⋅A⁻¹ (voltage × time)
  /// Named after Wilhelm Eduard Weber
  struct weber_t final : named_unit<"Wb", kind_of<isq::dimensionless_t>> {};
  inline constexpr weber_t weber {};

  /// tesla - unit of magnetic flux density
  /// Symbol: T
  /// Definition: Wb/m² = kg⋅s⁻²⋅A⁻¹ (magnetic flux per unit area)
  /// Named after Nikola Tesla
  struct tesla_t final : named_unit<"T", kind_of<isq::dimensionless_t>> {};
  inline constexpr tesla_t tesla {};

  /// henry - unit of electric inductance
  /// Symbol: H
  /// Definition: Wb/A = kg⋅m²⋅s⁻²⋅A⁻² (magnetic flux per unit current)
  /// Named after Joseph Henry
  struct henry_t final : named_unit<"H", kind_of<isq::dimensionless_t>> {};
  inline constexpr henry_t henry {};

  // -----------------------------------------------------------------------------
  // Temperature
  // -----------------------------------------------------------------------------

  /// degree Celsius - unit of Celsius temperature
  /// Symbol: °C (or degC in ASCII)
  /// Definition: Same magnitude as kelvin, offset by 273.15
  /// Named after Anders Celsius
  /// Note: For temperature intervals, 1°C = 1K
  struct degree_celsius_t final : named_unit<"degC", kind_of<isq::thermodynamic_temperature_t>> {};
  inline constexpr degree_celsius_t degree_celsius {};

  // -----------------------------------------------------------------------------
  // Photometry
  // -----------------------------------------------------------------------------

  /// lumen - unit of luminous flux
  /// Symbol: lm
  /// Definition: cd⋅sr (candela × steradian)
  /// Measures the total amount of visible light emitted
  struct lumen_t final : named_unit<"lm", kind_of<isq::luminous_intensity_t>> {};
  inline constexpr lumen_t lumen {};

  /// lux - unit of illuminance
  /// Symbol: lx
  /// Definition: lm/m² = cd⋅sr⋅m⁻² (luminous flux per unit area)
  /// Measures luminous flux incident on a surface
  struct lux_t final : named_unit<"lx", kind_of<isq::luminous_intensity_t>> {};
  inline constexpr lux_t lux {};

  // -----------------------------------------------------------------------------
  // Radioactivity and Radiation
  // -----------------------------------------------------------------------------

  /// becquerel - unit of radioactivity (activity)
  /// Symbol: Bq
  /// Definition: s⁻¹ (decays per second, same dimension as hertz)
  /// Named after Henri Becquerel
  struct becquerel_t final : named_unit<"Bq", kind_of<isq::frequency_t>> {};
  inline constexpr becquerel_t becquerel {};

  /// gray - unit of absorbed dose (ionizing radiation)
  /// Symbol: Gy
  /// Definition: J/kg = m²⋅s⁻² (energy absorbed per unit mass)
  /// Named after Louis Harold Gray
  struct gray_t final : named_unit<"Gy", kind_of<isq::energy_t>> {};
  inline constexpr gray_t gray {};

  /// sievert - unit of equivalent dose (ionizing radiation)
  /// Symbol: Sv
  /// Definition: J/kg = m²⋅s⁻² (same dimension as gray, but different quantity)
  /// Named after Rolf Sievert
  struct sievert_t final : named_unit<"Sv", kind_of<isq::energy_t>> {};
  inline constexpr sievert_t sievert {};

  // -----------------------------------------------------------------------------
  // Chemistry
  // -----------------------------------------------------------------------------

  /// katal - unit of catalytic activity
  /// Symbol: kat
  /// Definition: mol⋅s⁻¹ (amount of substance transformed per unit time)
  struct katal_t final : named_unit<"kat", kind_of<isq::amount_of_substance_t>> {};
  inline constexpr katal_t katal {};

  // =============================================================================
  // Static assertions to verify derived unit properties
  // =============================================================================

  // All derived units satisfy Unit and NamedUnit concepts
  static_assert(Unit<radian_t>);
  static_assert(Unit<steradian_t>);
  static_assert(Unit<hertz_t>);
  static_assert(Unit<newton_t>);
  static_assert(Unit<pascal_t>);
  static_assert(Unit<joule_t>);
  static_assert(Unit<watt_t>);
  static_assert(Unit<coulomb_t>);
  static_assert(Unit<volt_t>);
  static_assert(Unit<farad_t>);
  static_assert(Unit<ohm_t>);
  static_assert(Unit<siemens_t>);
  static_assert(Unit<weber_t>);
  static_assert(Unit<tesla_t>);
  static_assert(Unit<henry_t>);
  static_assert(Unit<degree_celsius_t>);
  static_assert(Unit<lumen_t>);
  static_assert(Unit<lux_t>);
  static_assert(Unit<becquerel_t>);
  static_assert(Unit<gray_t>);
  static_assert(Unit<sievert_t>);
  static_assert(Unit<katal_t>);

  static_assert(NamedUnit<newton_t>);
  static_assert(NamedUnit<joule_t>);
  static_assert(NamedUnit<watt_t>);

  // All named derived units have magnitude 1 (coherent derived units)
  static_assert(mag_equal_v<get_magnitude<radian_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<hertz_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<newton_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<joule_t>, mag_one>);
  static_assert(mag_equal_v<get_magnitude<watt_t>, mag_one>);

  // Verify symbols
  static_assert(hertz_t::symbol == fixed_string { "Hz" });
  static_assert(newton_t::symbol == fixed_string { "N" });
  static_assert(joule_t::symbol == fixed_string { "J" });
  static_assert(watt_t::symbol == fixed_string { "W" });
  static_assert(pascal_t::symbol == fixed_string { "Pa" });

} // namespace units::si
