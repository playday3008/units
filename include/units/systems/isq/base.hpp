// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/quantity_spec.hpp>

namespace units::isq {

  // =============================================================================
  // ISQ Base Quantities (ISO 80000)
  // =============================================================================
  // The seven base quantities of the International System of Quantities (ISQ)

  /// Length - extent of something along its greatest dimension
  struct length_t final : quantity_spec<> {};
  inline constexpr length_t length {};

  /// Mass - measure of resistance to acceleration
  struct mass_t final : quantity_spec<> {};
  inline constexpr mass_t mass {};

  /// Time - duration of events
  struct time_t final : quantity_spec<> {};
  inline constexpr time_t time {};

  /// Electric current - flow of electric charge
  struct electric_current_t final : quantity_spec<> {};
  inline constexpr electric_current_t electric_current {};

  /// Thermodynamic temperature - measure of thermal energy
  struct thermodynamic_temperature_t final : quantity_spec<> {};
  inline constexpr thermodynamic_temperature_t thermodynamic_temperature {};

  /// Amount of substance - number of elementary entities
  struct amount_of_substance_t final : quantity_spec<> {};
  inline constexpr amount_of_substance_t amount_of_substance {};

  /// Luminous intensity - luminous power per solid angle
  struct luminous_intensity_t final : quantity_spec<> {};
  inline constexpr luminous_intensity_t luminous_intensity {};

  // =============================================================================
  // Length subtypes (is-a relationships)
  // =============================================================================
  // These are all specific kinds of length, forming a hierarchy

  /// Width - horizontal extent
  struct width_t final : quantity_spec<length_t> {};
  inline constexpr width_t width {};

  /// Height - vertical extent
  struct height_t final : quantity_spec<length_t> {};
  inline constexpr height_t height {};

  /// Depth - extent into a surface or medium
  struct depth_t final : quantity_spec<length_t> {};
  inline constexpr depth_t depth {};

  /// Thickness - extent through a material
  struct thickness_t final : quantity_spec<length_t> {};
  inline constexpr thickness_t thickness {};

  /// Radius - distance from center to circumference
  struct radius_t final : quantity_spec<length_t> {};
  inline constexpr radius_t radius {};

  /// Diameter - distance across a circle through its center
  struct diameter_t final : quantity_spec<length_t> {};
  inline constexpr diameter_t diameter {};

  /// Distance - extent of separation between two points
  struct distance_t final : quantity_spec<length_t> {};
  inline constexpr distance_t distance {};

  /// Path length - length along a path
  struct path_length_t final : quantity_spec<length_t> {};
  inline constexpr path_length_t path_length {};

  /// Wavelength - spatial period of a wave
  struct wavelength_t final : quantity_spec<length_t> {};
  inline constexpr wavelength_t wavelength {};

  /// Position vector - vector quantity (unlike length which is scalar)
  struct position_vector_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr position_vector_t position_vector {};

  /// Displacement - change in position (vector)
  struct displacement_t final : quantity_spec<position_vector_t, quantity_character::vector> {};
  inline constexpr displacement_t displacement {};

  // =============================================================================
  // Time subtypes
  // =============================================================================

  /// Duration - length of a time interval
  struct duration_t final : quantity_spec<time_t> {};
  inline constexpr duration_t duration {};

  /// Period - time for one complete cycle
  struct period_t final : quantity_spec<duration_t> {};
  inline constexpr period_t period {};

  // =============================================================================
  // Area and Volume (derived from length)
  // =============================================================================

  /// Area - extent of a two-dimensional surface
  struct area_t final : quantity_spec<> {};
  inline constexpr area_t area {};

  /// Volume - extent of a three-dimensional region
  struct volume_t final : quantity_spec<> {};
  inline constexpr volume_t volume {};

  // =============================================================================
  // Dimensionless quantities
  // =============================================================================

  /// Dimensionless - quantity with no physical dimension
  struct dimensionless_t final : quantity_spec<> {};
  inline constexpr dimensionless_t dimensionless {};

  /// Angle (plane angle) - ratio of arc length to radius
  struct angle_t final : quantity_spec<dimensionless_t> {};
  inline constexpr angle_t angle {};

  /// Solid angle - ratio of area to square of radius
  struct solid_angle_t final : quantity_spec<dimensionless_t> {};
  inline constexpr solid_angle_t solid_angle {};

  // =============================================================================
  // Static assertions to verify hierarchy
  // =============================================================================

  // Width, height, radius, etc. are all lengths
  static_assert(is_child_of_v<width_t, length_t>);
  static_assert(is_child_of_v<height_t, length_t>);
  static_assert(is_child_of_v<radius_t, length_t>);
  static_assert(is_child_of_v<diameter_t, length_t>);
  static_assert(is_child_of_v<depth_t, length_t>);
  static_assert(is_child_of_v<thickness_t, length_t>);
  static_assert(is_child_of_v<distance_t, length_t>);
  static_assert(is_child_of_v<wavelength_t, length_t>);

  // But they are not each other
  static_assert(!is_child_of_v<width_t, height_t>);
  static_assert(!is_child_of_v<height_t, width_t>);

  // Base quantities are not children of each other
  static_assert(!is_child_of_v<length_t, mass_t>);
  static_assert(!is_child_of_v<mass_t, time_t>);

  // Period is duration is time
  static_assert(is_child_of_v<period_t, duration_t>);
  static_assert(is_child_of_v<period_t, time_t>);
  static_assert(is_child_of_v<duration_t, time_t>);

  // Angle and solid_angle are dimensionless
  static_assert(is_child_of_v<angle_t, dimensionless_t>);
  static_assert(is_child_of_v<solid_angle_t, dimensionless_t>);

  // Displacement is position_vector (both vectors)
  static_assert(is_child_of_v<displacement_t, position_vector_t>);

  // Characters are correct
  static_assert(length_t::character == quantity_character::scalar);
  static_assert(position_vector_t::character == quantity_character::vector);
  static_assert(displacement_t::character == quantity_character::vector);

} // namespace units::isq
