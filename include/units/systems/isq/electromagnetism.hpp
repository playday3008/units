// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include <units/core/quantity_spec.hpp>

namespace units::isq {

  // =============================================================================
  // ISQ Electromagnetism Quantities (ISO 80000-6)
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Basic electrical quantities
  // -----------------------------------------------------------------------------

  /// Electric charge - fundamental property of matter (scalar)
  /// Equation: Q = I * t
  /// Dimension: I*T
  struct electric_charge_t final : quantity_spec<> {};
  inline constexpr electric_charge_t electric_charge {};

  /// Electric potential (voltage) - work per unit charge (scalar)
  /// Equation: V = W/Q
  /// Dimension: M*L^2*T^-3*I^-1
  struct voltage_t final : quantity_spec<> {};
  inline constexpr voltage_t voltage {};

  /// Electric potential difference - alias for voltage
  using electric_potential_t = voltage_t;
  inline constexpr electric_potential_t electric_potential {};

  /// Electromotive force (EMF) - energy per unit charge from non-electrostatic source
  /// Same dimension as voltage but conceptually distinct
  struct electromotive_force_t final : quantity_spec<voltage_t> {};
  inline constexpr electromotive_force_t electromotive_force {};
  inline constexpr electromotive_force_t emf = electromotive_force;

  // -----------------------------------------------------------------------------
  // Circuit elements
  // -----------------------------------------------------------------------------

  /// Capacitance - ability to store charge (scalar)
  /// Equation: C = Q/V
  /// Dimension: M^-1*L^-2*T^4*I^2
  struct capacitance_t final : quantity_spec<> {};
  inline constexpr capacitance_t capacitance {};

  /// Inductance - ability to store magnetic energy (scalar)
  /// Equation: L = Φ/I
  /// Dimension: M*L^2*T^-2*I^-2
  struct inductance_t final : quantity_spec<> {};
  inline constexpr inductance_t inductance {};

  /// Resistance - opposition to current flow (scalar)
  /// Equation: R = V/I (Ohm's law)
  /// Dimension: M*L^2*T^-3*I^-2
  struct resistance_t final : quantity_spec<> {};
  inline constexpr resistance_t resistance {};

  /// Impedance - frequency-dependent opposition to current (scalar)
  /// Same dimension as resistance
  struct impedance_t final : quantity_spec<resistance_t> {};
  inline constexpr impedance_t impedance {};

  /// Reactance - imaginary part of impedance (scalar)
  /// Same dimension as resistance
  struct reactance_t final : quantity_spec<resistance_t> {};
  inline constexpr reactance_t reactance {};

  /// Conductance - inverse of resistance (scalar)
  /// Equation: G = 1/R = I/V
  /// Dimension: M^-1*L^-2*T^3*I^2
  struct conductance_t final : quantity_spec<> {};
  inline constexpr conductance_t conductance {};

  /// Admittance - inverse of impedance (scalar)
  /// Same dimension as conductance
  struct admittance_t final : quantity_spec<conductance_t> {};
  inline constexpr admittance_t admittance {};

  // -----------------------------------------------------------------------------
  // Magnetic quantities
  // -----------------------------------------------------------------------------

  /// Magnetic flux - total magnetic field through a surface (scalar)
  /// Equation: Φ = ∫B·dA
  /// Dimension: M*L^2*T^-2*I^-1
  struct magnetic_flux_t final : quantity_spec<> {};
  inline constexpr magnetic_flux_t magnetic_flux {};

  /// Magnetic flux density (B-field) - magnetic field strength per unit area (vector)
  /// Equation: B = Φ/A
  /// Dimension: M*T^-2*I^-1
  struct magnetic_flux_density_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr magnetic_flux_density_t magnetic_flux_density {};

  /// Magnetic field strength (H-field) - magnetizing field (vector)
  /// Equation: ∇×H = J + ∂D/∂t
  /// Dimension: I*L^-1
  struct magnetic_field_strength_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr magnetic_field_strength_t magnetic_field_strength {};

  /// Magnetic vector potential - vector potential for B field (vector)
  /// Equation: B = ∇×A
  /// Dimension: M*L*T^-2*I^-1
  struct magnetic_vector_potential_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr magnetic_vector_potential_t magnetic_vector_potential {};

  /// Magnetomotive force - magnetic analogue of voltage (scalar)
  /// Equation: F = N*I
  /// Dimension: I
  struct magnetomotive_force_t final : quantity_spec<> {};
  inline constexpr magnetomotive_force_t magnetomotive_force {};

  /// Magnetic reluctance - opposition to magnetic flux (scalar)
  /// Equation: R_m = F/Φ
  /// Dimension: M^-1*L^-2*T^2*I^2
  struct magnetic_reluctance_t final : quantity_spec<> {};
  inline constexpr magnetic_reluctance_t magnetic_reluctance {};

  // -----------------------------------------------------------------------------
  // Electric field quantities
  // -----------------------------------------------------------------------------

  /// Electric field strength (E-field) - force per unit charge (vector)
  /// Equation: E = F/q
  /// Dimension: M*L*T^-3*I^-1
  struct electric_field_strength_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr electric_field_strength_t electric_field_strength {};

  /// Electric displacement field (D-field) - accounts for free charges (vector)
  /// Equation: ∇·D = ρ_free
  /// Dimension: I*T*L^-2
  struct electric_displacement_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr electric_displacement_t electric_displacement {};

  /// Electric flux - total electric field through a surface (scalar)
  /// Equation: Ψ = ∫D·dA
  /// Dimension: I*T
  struct electric_flux_t final : quantity_spec<> {};
  inline constexpr electric_flux_t electric_flux {};

  /// Current density - current per unit area (vector)
  /// Equation: J = I/A
  /// Dimension: I*L^-2
  struct current_density_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr current_density_t current_density {};

  /// Surface current density - current per unit width (vector)
  /// Dimension: I*L^-1
  struct surface_current_density_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr surface_current_density_t surface_current_density {};

  /// Electric polarization - dipole moment per unit volume (vector)
  /// Dimension: I*T*L^-2
  struct electric_polarization_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr electric_polarization_t electric_polarization {};

  /// Magnetization - magnetic moment per unit volume (vector)
  /// Dimension: I*L^-1
  struct magnetization_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr magnetization_t magnetization {};

  // -----------------------------------------------------------------------------
  // Material properties
  // -----------------------------------------------------------------------------

  /// Resistivity - resistance per unit length and area (scalar)
  /// Equation: ρ = R*A/L
  /// Dimension: M*L^3*T^-3*I^-2
  struct resistivity_t final : quantity_spec<> {};
  inline constexpr resistivity_t resistivity {};

  /// Conductivity - inverse of resistivity (scalar)
  /// Equation: σ = 1/ρ
  /// Dimension: M^-1*L^-3*T^3*I^2
  struct conductivity_t final : quantity_spec<> {};
  inline constexpr conductivity_t conductivity {};

  /// Permittivity - electric polarizability of a medium (scalar)
  /// Equation: D = εE
  /// Dimension: M^-1*L^-3*T^4*I^2
  struct permittivity_t final : quantity_spec<> {};
  inline constexpr permittivity_t permittivity {};

  /// Permeability - magnetic polarizability of a medium (scalar)
  /// Equation: B = μH
  /// Dimension: M*L*T^-2*I^-2
  struct permeability_t final : quantity_spec<> {};
  inline constexpr permeability_t permeability {};

  /// Electric susceptibility - polarization response to E field (dimensionless)
  /// Equation: P = ε₀χ_eE
  struct electric_susceptibility_t final : quantity_spec<> {};
  inline constexpr electric_susceptibility_t electric_susceptibility {};

  /// Magnetic susceptibility - magnetization response to H field (dimensionless)
  /// Equation: M = χ_mH
  struct magnetic_susceptibility_t final : quantity_spec<> {};
  inline constexpr magnetic_susceptibility_t magnetic_susceptibility {};

  // -----------------------------------------------------------------------------
  // Power and energy in EM
  // -----------------------------------------------------------------------------

  /// Electric power - rate of energy transfer (scalar)
  /// Equation: P = V*I
  /// Same dimension as mechanical power
  struct electric_power_t final : quantity_spec<> {};
  inline constexpr electric_power_t electric_power {};

  /// Poynting vector - electromagnetic power density (vector)
  /// Equation: S = E × H
  /// Dimension: M*T^-3
  struct poynting_vector_t final : quantity_spec<void, quantity_character::vector> {};
  inline constexpr poynting_vector_t poynting_vector {};

  // =============================================================================
  // Static assertions to verify hierarchy and properties
  // =============================================================================

  // EMF is a child of voltage
  static_assert(is_child_of_v<electromotive_force_t, voltage_t>);

  // Impedance and reactance are children of resistance
  static_assert(is_child_of_v<impedance_t, resistance_t>);
  static_assert(is_child_of_v<reactance_t, resistance_t>);

  // Admittance is a child of conductance
  static_assert(is_child_of_v<admittance_t, conductance_t>);

  // Conversion rules
  static_assert(implicitly_convertible_v<electromotive_force_t, voltage_t>);
  static_assert(implicitly_convertible_v<impedance_t, resistance_t>);
  static_assert(implicitly_convertible_v<admittance_t, conductance_t>);

  // Vector quantities
  static_assert(magnetic_flux_density_t::character == quantity_character::vector);
  static_assert(magnetic_field_strength_t::character == quantity_character::vector);
  static_assert(electric_field_strength_t::character == quantity_character::vector);
  static_assert(current_density_t::character == quantity_character::vector);
  static_assert(electric_polarization_t::character == quantity_character::vector);
  static_assert(magnetization_t::character == quantity_character::vector);
  static_assert(poynting_vector_t::character == quantity_character::vector);

  // Scalar quantities
  static_assert(electric_charge_t::character == quantity_character::scalar);
  static_assert(voltage_t::character == quantity_character::scalar);
  static_assert(capacitance_t::character == quantity_character::scalar);
  static_assert(inductance_t::character == quantity_character::scalar);
  static_assert(resistance_t::character == quantity_character::scalar);
  static_assert(conductance_t::character == quantity_character::scalar);
  static_assert(magnetic_flux_t::character == quantity_character::scalar);
  static_assert(resistivity_t::character == quantity_character::scalar);
  static_assert(conductivity_t::character == quantity_character::scalar);
  static_assert(permittivity_t::character == quantity_character::scalar);
  static_assert(permeability_t::character == quantity_character::scalar);

  // Common quantity spec tests
  static_assert(std::same_as<common_quantity_spec<impedance_t, reactance_t>, resistance_t>);

} // namespace units::isq
