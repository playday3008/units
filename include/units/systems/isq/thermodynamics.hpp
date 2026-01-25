// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include <units/core/quantity_spec.hpp>
#include <units/systems/isq/mechanics.hpp>

namespace units::isq {

  // =============================================================================
  // ISQ Thermodynamics Quantities (ISO 80000-5)
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Heat capacity
  // -----------------------------------------------------------------------------

  /// Heat capacity - energy required to raise temperature by one unit (scalar)
  /// Equation: C = dQ/dT
  /// Dimension: M*L^2*T^-2*Θ^-1
  struct heat_capacity_t final : quantity_spec<> {};
  inline constexpr heat_capacity_t heat_capacity {};

  /// Specific heat capacity - heat capacity per unit mass (scalar)
  /// Equation: c = C/m = dQ/(m*dT)
  /// Dimension: L^2*T^-2*Θ^-1
  struct specific_heat_capacity_t final : quantity_spec<> {};
  inline constexpr specific_heat_capacity_t specific_heat_capacity {};

  /// Molar heat capacity - heat capacity per mole (scalar)
  /// Equation: C_m = C/n
  /// Dimension: M*L^2*T^-2*Θ^-1*N^-1
  struct molar_heat_capacity_t final : quantity_spec<> {};
  inline constexpr molar_heat_capacity_t molar_heat_capacity {};

  // -----------------------------------------------------------------------------
  // Entropy
  // -----------------------------------------------------------------------------

  /// Entropy - measure of disorder/unavailable energy (scalar)
  /// Equation: dS = dQ_rev/T
  /// Dimension: M*L^2*T^-2*Θ^-1
  struct entropy_t final : quantity_spec<> {};
  inline constexpr entropy_t entropy {};

  /// Specific entropy - entropy per unit mass (scalar)
  /// Equation: s = S/m
  /// Dimension: L^2*T^-2*Θ^-1
  struct specific_entropy_t final : quantity_spec<> {};
  inline constexpr specific_entropy_t specific_entropy {};

  /// Molar entropy - entropy per mole (scalar)
  /// Equation: S_m = S/n
  /// Dimension: M*L^2*T^-2*Θ^-1*N^-1
  struct molar_entropy_t final : quantity_spec<> {};
  inline constexpr molar_entropy_t molar_entropy {};

  // -----------------------------------------------------------------------------
  // Thermal transport
  // -----------------------------------------------------------------------------

  /// Thermal conductivity - rate of heat transfer through a material (scalar)
  /// Equation: q = -k * grad(T)
  /// Dimension: M*L*T^-3*Θ^-1
  struct thermal_conductivity_t final : quantity_spec<> {};
  inline constexpr thermal_conductivity_t thermal_conductivity {};

  /// Thermal resistance - opposition to heat flow (scalar)
  /// Equation: R_th = ΔT/P = L/(k*A)
  /// Dimension: M^-1*L^-2*T^3*Θ
  struct thermal_resistance_t final : quantity_spec<> {};
  inline constexpr thermal_resistance_t thermal_resistance {};

  /// Heat flux density - rate of heat transfer per unit area (scalar)
  /// Equation: q = Q/(A*t)
  /// Dimension: M*T^-3
  struct heat_flux_density_t final : quantity_spec<> {};
  inline constexpr heat_flux_density_t heat_flux_density {};

  /// Heat transfer coefficient - convective heat transfer rate (scalar)
  /// Equation: h = q/ΔT
  /// Dimension: M*T^-3*Θ^-1
  struct heat_transfer_coefficient_t final : quantity_spec<> {};
  inline constexpr heat_transfer_coefficient_t heat_transfer_coefficient {};

  /// Thermal diffusivity - rate of heat diffusion through a material (scalar)
  /// Equation: α = k/(ρ*c_p)
  /// Dimension: L^2*T^-1
  struct thermal_diffusivity_t final : quantity_spec<> {};
  inline constexpr thermal_diffusivity_t thermal_diffusivity {};

  // -----------------------------------------------------------------------------
  // Thermodynamic potentials (children of energy_t)
  // -----------------------------------------------------------------------------

  /// Internal energy - total energy within a thermodynamic system (is-a energy)
  /// Equation: U = Q - W (first law)
  struct internal_energy_t final : quantity_spec<energy_t> {};
  inline constexpr internal_energy_t internal_energy {};

  /// Enthalpy - internal energy plus pressure-volume work (is-a energy)
  /// Equation: H = U + PV
  struct enthalpy_t final : quantity_spec<energy_t> {};
  inline constexpr enthalpy_t enthalpy {};

  /// Helmholtz energy (free energy) - available work at constant T and V (is-a energy)
  /// Equation: F = U - TS
  struct helmholtz_energy_t final : quantity_spec<energy_t> {};
  inline constexpr helmholtz_energy_t helmholtz_energy {};

  /// Gibbs energy (free energy) - available work at constant T and P (is-a energy)
  /// Equation: G = H - TS = U + PV - TS
  struct gibbs_energy_t final : quantity_spec<energy_t> {};
  inline constexpr gibbs_energy_t gibbs_energy {};

  // -----------------------------------------------------------------------------
  // Specific thermodynamic potentials
  // -----------------------------------------------------------------------------

  /// Specific internal energy - internal energy per unit mass (is-a energy)
  /// Equation: u = U/m
  struct specific_internal_energy_t final : quantity_spec<energy_t> {};
  inline constexpr specific_internal_energy_t specific_internal_energy {};

  /// Specific enthalpy - enthalpy per unit mass (is-a energy)
  /// Equation: h = H/m
  struct specific_enthalpy_t final : quantity_spec<energy_t> {};
  inline constexpr specific_enthalpy_t specific_enthalpy {};

  /// Specific Helmholtz energy - Helmholtz energy per unit mass (is-a energy)
  /// Equation: f = F/m
  struct specific_helmholtz_energy_t final : quantity_spec<energy_t> {};
  inline constexpr specific_helmholtz_energy_t specific_helmholtz_energy {};

  /// Specific Gibbs energy - Gibbs energy per unit mass (is-a energy)
  /// Equation: g = G/m
  struct specific_gibbs_energy_t final : quantity_spec<energy_t> {};
  inline constexpr specific_gibbs_energy_t specific_gibbs_energy {};

  // -----------------------------------------------------------------------------
  // Molar thermodynamic potentials
  // -----------------------------------------------------------------------------

  /// Molar internal energy - internal energy per mole
  struct molar_internal_energy_t final : quantity_spec<> {};
  inline constexpr molar_internal_energy_t molar_internal_energy {};

  /// Molar enthalpy - enthalpy per mole
  struct molar_enthalpy_t final : quantity_spec<> {};
  inline constexpr molar_enthalpy_t molar_enthalpy {};

  /// Molar Gibbs energy (chemical potential) - Gibbs energy per mole
  struct molar_gibbs_energy_t final : quantity_spec<> {};
  inline constexpr molar_gibbs_energy_t molar_gibbs_energy {};

  /// Chemical potential - change in free energy with particle number
  /// Equation: μ = (∂G/∂n)_{T,P}
  using chemical_potential_t = molar_gibbs_energy_t;
  inline constexpr chemical_potential_t chemical_potential {};

  // =============================================================================
  // Static assertions to verify hierarchy and properties
  // =============================================================================

  // Thermodynamic potentials are children of energy
  static_assert(is_child_of_v<internal_energy_t, energy_t>);
  static_assert(is_child_of_v<enthalpy_t, energy_t>);
  static_assert(is_child_of_v<helmholtz_energy_t, energy_t>);
  static_assert(is_child_of_v<gibbs_energy_t, energy_t>);

  // Specific potentials are children of energy
  static_assert(is_child_of_v<specific_internal_energy_t, energy_t>);
  static_assert(is_child_of_v<specific_enthalpy_t, energy_t>);
  static_assert(is_child_of_v<specific_helmholtz_energy_t, energy_t>);
  static_assert(is_child_of_v<specific_gibbs_energy_t, energy_t>);

  // Conversion rules - thermodynamic potentials can convert to energy
  static_assert(implicitly_convertible_v<internal_energy_t, energy_t>);
  static_assert(implicitly_convertible_v<enthalpy_t, energy_t>);
  static_assert(implicitly_convertible_v<helmholtz_energy_t, energy_t>);
  static_assert(implicitly_convertible_v<gibbs_energy_t, energy_t>);

  // But not the reverse (parent to child requires explicit conversion)
  static_assert(!implicitly_convertible_v<energy_t, internal_energy_t>);
  static_assert(!implicitly_convertible_v<energy_t, enthalpy_t>);

  // All thermodynamic quantities are scalar
  static_assert(heat_capacity_t::character == quantity_character::scalar);
  static_assert(specific_heat_capacity_t::character == quantity_character::scalar);
  static_assert(entropy_t::character == quantity_character::scalar);
  static_assert(thermal_conductivity_t::character == quantity_character::scalar);
  static_assert(internal_energy_t::character == quantity_character::scalar);
  static_assert(enthalpy_t::character == quantity_character::scalar);
  static_assert(gibbs_energy_t::character == quantity_character::scalar);

  // Common quantity spec - different thermodynamic potentials share energy as common
  static_assert(std::same_as<common_quantity_spec<internal_energy_t, enthalpy_t>, energy_t>);
  static_assert(std::same_as<common_quantity_spec<helmholtz_energy_t, gibbs_energy_t>, energy_t>);

} // namespace units::isq
