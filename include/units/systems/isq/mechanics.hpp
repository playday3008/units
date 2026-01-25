// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include <units/core/quantity_spec.hpp>
#include <units/systems/isq/base.hpp>

namespace units::isq {

  // =============================================================================
  // ISQ Mechanics Quantities (ISO 80000-4)
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Speed and velocity
  // -----------------------------------------------------------------------------

  /// Speed - rate of change of distance with respect to time (scalar)
  /// Equation: speed = length / time
  struct speed_t final : quantity_spec<> {};
  inline constexpr speed_t speed {};

  /// Velocity - rate of change of position with respect to time (vector)
  /// Equation: velocity = displacement / time
  struct velocity_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr velocity_t velocity {};

  // -----------------------------------------------------------------------------
  // Acceleration
  // -----------------------------------------------------------------------------

  /// Acceleration - rate of change of velocity with respect to time (vector)
  /// Equation: acceleration = velocity / time
  struct acceleration_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr acceleration_t acceleration {};

  // -----------------------------------------------------------------------------
  // Force and related quantities
  // -----------------------------------------------------------------------------

  /// Force - interaction that changes the motion of an object (vector)
  /// Equation: force = mass * acceleration
  struct force_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr force_t force {};

  /// Weight - gravitational force on an object (is-a force)
  struct weight_t final : quantity_spec<force_t, quantity_character::vector> {};
  inline constexpr weight_t weight {};

  // -----------------------------------------------------------------------------
  // Momentum
  // -----------------------------------------------------------------------------

  /// Momentum - product of mass and velocity (vector)
  /// Equation: momentum = mass * velocity
  struct momentum_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr momentum_t momentum {};

  /// Impulse - change in momentum (is-a momentum)
  struct impulse_t final : quantity_spec<momentum_t, quantity_character::vector> {};
  inline constexpr impulse_t impulse {};

  // -----------------------------------------------------------------------------
  // Energy and Work
  // -----------------------------------------------------------------------------

  /// Energy - capacity to do work (scalar)
  /// Equation: energy = force * length (same dimension as work)
  struct energy_t final : quantity_spec<> {};
  inline constexpr energy_t energy {};

  /// Kinetic energy - energy of motion (is-a energy)
  struct kinetic_energy_t final : quantity_spec<energy_t> {};
  inline constexpr kinetic_energy_t kinetic_energy {};

  /// Potential energy - energy of position (is-a energy)
  struct potential_energy_t final : quantity_spec<energy_t> {};
  inline constexpr potential_energy_t potential_energy {};

  /// Mechanical energy - sum of kinetic and potential (is-a energy)
  struct mechanical_energy_t final : quantity_spec<energy_t> {};
  inline constexpr mechanical_energy_t mechanical_energy {};

  /// Work - energy transfer by force over distance (is-a energy)
  /// Note: Work has the same dimension as energy but is conceptually distinct
  struct work_t final : quantity_spec<energy_t> {};
  inline constexpr work_t work {};

  /// Heat - thermal energy transfer (is-a energy)
  struct heat_t final : quantity_spec<energy_t> {};
  inline constexpr heat_t heat {};

  // -----------------------------------------------------------------------------
  // Torque (Moment of force)
  // -----------------------------------------------------------------------------

  /// Torque - rotational equivalent of force (scalar despite being r x F)
  /// CRITICAL: Same dimension as energy (M*L^2*T^-2) but DIFFERENT KIND!
  /// Torque should NOT be implicitly convertible to/from energy.
  struct torque_t final : quantity_spec<> {};
  inline constexpr torque_t torque {};

  /// Moment of force - alias for torque
  using moment_of_force_t = torque_t;
  inline constexpr moment_of_force_t moment_of_force {};

  // -----------------------------------------------------------------------------
  // Power
  // -----------------------------------------------------------------------------

  /// Power - rate of energy transfer or work done (scalar)
  /// Equation: power = energy / time
  struct power_t final : quantity_spec<> {};
  inline constexpr power_t power {};

  // -----------------------------------------------------------------------------
  // Pressure and Stress
  // -----------------------------------------------------------------------------

  /// Pressure - force per unit area (scalar)
  /// Equation: pressure = force / area
  struct pressure_t final : quantity_spec<> {};
  inline constexpr pressure_t pressure {};

  /// Stress - internal forces in a material (tensor, but often treated as scalar)
  struct stress_t final : quantity_spec<pressure_t> {};
  inline constexpr stress_t stress {};

  // -----------------------------------------------------------------------------
  // Frequency
  // -----------------------------------------------------------------------------

  /// Frequency - number of cycles per unit time (scalar)
  /// Equation: frequency = 1 / time
  struct frequency_t final : quantity_spec<> {};
  inline constexpr frequency_t frequency {};

  /// Angular frequency - frequency in radians per second
  /// Note: related to frequency by factor of 2*pi
  struct angular_frequency_t final : quantity_spec<> {};
  inline constexpr angular_frequency_t angular_frequency {};

  // -----------------------------------------------------------------------------
  // Angular quantities
  // -----------------------------------------------------------------------------

  /// Angular velocity - rate of change of angle
  struct angular_velocity_t final : quantity_spec<> {};
  inline constexpr angular_velocity_t angular_velocity {};

  /// Angular acceleration - rate of change of angular velocity
  struct angular_acceleration_t final : quantity_spec<> {};
  inline constexpr angular_acceleration_t angular_acceleration {};

  /// Angular momentum - rotational equivalent of linear momentum
  struct angular_momentum_t final : quantity_spec<> {};
  inline constexpr angular_momentum_t angular_momentum {};

  /// Moment of inertia - rotational equivalent of mass
  struct moment_of_inertia_t final : quantity_spec<> {};
  inline constexpr moment_of_inertia_t moment_of_inertia {};

  // =============================================================================
  // Static assertions to verify hierarchy and properties
  // =============================================================================

  // Energy subtypes hierarchy
  static_assert(is_child_of_v<kinetic_energy_t, energy_t>);
  static_assert(is_child_of_v<potential_energy_t, energy_t>);
  static_assert(is_child_of_v<mechanical_energy_t, energy_t>);
  static_assert(is_child_of_v<work_t, energy_t>);
  static_assert(is_child_of_v<heat_t, energy_t>);

  // Weight is-a force
  static_assert(is_child_of_v<weight_t, force_t>);

  // Impulse is-a momentum
  static_assert(is_child_of_v<impulse_t, momentum_t>);

  // Stress is-a pressure
  static_assert(is_child_of_v<stress_t, pressure_t>);

  // CRITICAL: Torque is NOT energy (different kind despite same dimension)
  static_assert(!is_child_of_v<torque_t, energy_t>);
  static_assert(!is_child_of_v<energy_t, torque_t>);

  // Conversion rules
  static_assert(implicitly_convertible_v<work_t, energy_t>);           // work -> energy: OK
  static_assert(implicitly_convertible_v<kinetic_energy_t, energy_t>); // kinetic -> energy: OK
  static_assert(
      !implicitly_convertible_v<energy_t, work_t>); // energy -> work: NO (parent to child)
  static_assert(!implicitly_convertible_v<torque_t, energy_t>); // torque -> energy: NO!
  static_assert(!implicitly_convertible_v<energy_t, torque_t>); // energy -> torque: NO!

  // Explicit conversion is allowed within hierarchy
  static_assert(explicitly_convertible_v<work_t, energy_t>);
  static_assert(explicitly_convertible_v<energy_t, work_t>);

  // But NOT between unrelated types
  static_assert(!explicitly_convertible_v<torque_t, energy_t>);
  static_assert(!explicitly_convertible_v<energy_t, torque_t>);

  // Common quantity spec tests
  static_assert(std::same_as<common_quantity_spec<width_t, height_t>, length_t>);
  static_assert(std::same_as<common_quantity_spec<work_t, kinetic_energy_t>, energy_t>);
  static_assert(std::same_as<common_quantity_spec<width_t, width_t>, width_t>);

  // Vector character is preserved
  static_assert(velocity_t::character == quantity_character::vector);
  static_assert(acceleration_t::character == quantity_character::vector);
  static_assert(force_t::character == quantity_character::vector);
  static_assert(weight_t::character == quantity_character::vector);
  static_assert(momentum_t::character == quantity_character::vector);
  static_assert(impulse_t::character == quantity_character::vector);

  // Scalar quantities
  static_assert(speed_t::character == quantity_character::scalar);
  static_assert(energy_t::character == quantity_character::scalar);
  static_assert(torque_t::character == quantity_character::scalar);
  static_assert(power_t::character == quantity_character::scalar);
  static_assert(pressure_t::character == quantity_character::scalar);

} // namespace units::isq
