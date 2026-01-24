// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include <units/core/quantity.hpp>
#include <units/core/reference.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/isq/mechanics.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/prefixes.hpp>

namespace units::si {

  // =============================================================================
  // SI Defining Constants (2019 SI redefinition)
  // =============================================================================
  // The SI is now defined by fixing the numerical values of seven defining
  // constants. These values are exact by definition.
  //
  // Reference: BIPM SI Brochure, 9th edition (2019)
  // https://www.bipm.org/en/publications/si-brochure

  // =============================================================================
  // Exact defining constant values
  // =============================================================================

  namespace constants {

    // -----------------------------------------------------------------------------
    // The hyperfine transition frequency of caesium-133
    // -----------------------------------------------------------------------------

    /// Hyperfine transition frequency of Cs-133 (exact)
    /// Symbol: ΔνCs
    /// Value: 9,192,631,770 Hz (exactly)
    /// This defines the SI second.
    inline constexpr std::int64_t caesium_frequency_value = 91'9263'1770LL;

    // -----------------------------------------------------------------------------
    // Speed of light in vacuum
    // -----------------------------------------------------------------------------

    /// Speed of light in vacuum (exact)
    /// Symbol: c
    /// Value: 299,792,458 m/s (exactly)
    /// This defines the SI metre in terms of the second.
    inline constexpr std::int64_t speed_of_light_value = 2'9979'2458LL;

    // -----------------------------------------------------------------------------
    // Planck constant
    // -----------------------------------------------------------------------------

    /// Planck constant (exact)
    /// Symbol: h
    /// Value: 6.62607015×10⁻³⁴ J⋅s (exactly)
    /// This defines the SI kilogram in terms of the second and metre.
    inline constexpr long double planck_constant_value = 6.62607015e-34L;

    /// Reduced Planck constant (derived)
    /// Symbol: ℏ (h-bar)
    /// Value: h/(2π) = 1.054571817...×10⁻³⁴ J⋅s
    inline constexpr long double reduced_planck_constant_value
        = 1.054571817646156391262428003302280744e-34L;

    // -----------------------------------------------------------------------------
    // Elementary charge
    // -----------------------------------------------------------------------------

    /// Elementary charge (exact)
    /// Symbol: e
    /// Value: 1.602176634×10⁻¹⁹ C (exactly)
    /// This defines the SI ampere in terms of the second.
    inline constexpr long double elementary_charge_value = 1.602176634e-19L;

    // -----------------------------------------------------------------------------
    // Boltzmann constant
    // -----------------------------------------------------------------------------

    /// Boltzmann constant (exact)
    /// Symbol: k or kB
    /// Value: 1.380649×10⁻²³ J/K (exactly)
    /// This defines the SI kelvin in terms of the kilogram, metre, and second.
    inline constexpr long double boltzmann_constant_value = 1.380649e-23L;

    // -----------------------------------------------------------------------------
    // Avogadro constant
    // -----------------------------------------------------------------------------

    /// Avogadro constant (exact)
    /// Symbol: NA
    /// Value: 6.02214076×10²³ mol⁻¹ (exactly)
    /// This defines the SI mole.
    inline constexpr long double avogadro_constant_value = 6.02214076e23L;

    // -----------------------------------------------------------------------------
    // Luminous efficacy of 540 THz radiation
    // -----------------------------------------------------------------------------

    /// Luminous efficacy (exact)
    /// Symbol: Kcd
    /// Value: 683 lm/W (exactly)
    /// This defines the SI candela in terms of the other units.
    inline constexpr int luminous_efficacy_value = 683;

  } // namespace constants

  // =============================================================================
  // Physical constant quantity types
  // =============================================================================
  // These represent the actual physical constants as quantity values with
  // their correct dimensions. They can be used in dimensional analysis
  // and unit conversions.

  namespace detail {

    // Reference types for physical constants
    using speed_ref       = reference<isq::speed_t, decltype(metre / second)>;
    using energy_time_ref = reference<decltype(isq::energy * isq::time), decltype(joule * second)>;
    using charge_ref      = reference<isq::electric_current_t, coulomb_t>;
    using frequency_ref   = reference<isq::frequency_t, hertz_t>;

  } // namespace detail

  // =============================================================================
  // Speed of light (c)
  // =============================================================================

  /// The speed of light in vacuum
  /// c = 299,792,458 m/s (exactly)
  ///
  /// Usage:
  ///   auto distance = speed_of_light * 1.0_s;  // 1 light-second in metres
  ///   auto time = 1.0_m / speed_of_light;       // time for light to travel 1 m
  inline constexpr auto speed_of_light
      = quantity<reference<isq::speed_t, decltype(metre / second)>, std::int64_t> {
          constants::speed_of_light_value
        };

  /// Commonly used alias
  inline constexpr auto c = speed_of_light;

  // =============================================================================
  // Planck constant (h) and reduced Planck constant (ℏ)
  // =============================================================================

  /// The Planck constant
  /// h = 6.62607015×10⁻³⁴ J⋅s (exactly)
  ///
  /// Usage:
  ///   auto E = planck_constant * frequency;  // E = hν
  inline constexpr auto planck_constant
      = quantity<reference<decltype(isq::energy * isq::time), decltype(joule * second)>,
                 long double> { constants::planck_constant_value };

  /// Commonly used alias
  inline constexpr auto h = planck_constant;

  /// The reduced Planck constant (Dirac constant)
  /// ℏ = h/(2π) = 1.054571817...×10⁻³⁴ J⋅s
  ///
  /// Usage in quantum mechanics:
  ///   auto L = hbar * quantum_number;  // angular momentum quantization
  inline constexpr auto reduced_planck_constant
      = quantity<reference<decltype(isq::energy * isq::time), decltype(joule * second)>,
                 long double> { constants::reduced_planck_constant_value };

  /// Commonly used aliases
  inline constexpr auto hbar  = reduced_planck_constant;
  inline constexpr auto h_bar = reduced_planck_constant;

  // =============================================================================
  // Elementary charge (e)
  // =============================================================================

  /// The elementary charge (charge of a proton, negation of electron charge)
  /// e = 1.602176634×10⁻¹⁹ C (exactly)
  ///
  /// Usage:
  ///   auto charge = 2.0 * elementary_charge;  // charge of alpha particle
  inline constexpr auto elementary_charge
      = quantity<reference<isq::electric_current_t, coulomb_t>, long double> {
          constants::elementary_charge_value
        };

  /// Commonly used alias
  inline constexpr auto e_charge = elementary_charge;

  // =============================================================================
  // Boltzmann constant (k)
  // =============================================================================

  /// The Boltzmann constant
  /// k = 1.380649×10⁻²³ J/K (exactly)
  ///
  /// Usage:
  ///   auto thermal_energy = boltzmann_constant * temperature;  // E = kT
  inline constexpr auto boltzmann_constant = quantity<
      reference<decltype(isq::energy / isq::thermodynamic_temperature), decltype(joule / kelvin)>,
      long double> { constants::boltzmann_constant_value };

  /// Commonly used aliases
  inline constexpr auto k_B         = boltzmann_constant;
  inline constexpr auto k_boltzmann = boltzmann_constant;

  // =============================================================================
  // Avogadro constant (NA)
  // =============================================================================

  /// The Avogadro constant
  /// NA = 6.02214076×10²³ mol⁻¹ (exactly)
  ///
  /// Usage:
  ///   auto num_particles = avogadro_constant * amount_of_substance;
  inline constexpr auto avogadro_constant
      = quantity<reference<decltype(isq::dimensionless / isq::amount_of_substance),
                           decltype(second / (second * mole))>,
                 long double> { constants::avogadro_constant_value };

  /// Commonly used alias
  inline constexpr auto N_A = avogadro_constant;

  // =============================================================================
  // Derived physical constants
  // =============================================================================

  namespace constants {

    // -----------------------------------------------------------------------------
    // Vacuum permittivity (ε₀)
    // -----------------------------------------------------------------------------

    /// Vacuum permittivity (derived from c and μ₀)
    /// ε₀ = 1/(μ₀c²) = 8.8541878128...×10⁻¹² F/m
    /// Note: No longer exact after 2019 SI redefinition
    inline constexpr long double vacuum_permittivity_value = 8.8541878128e-12L;

    // -----------------------------------------------------------------------------
    // Vacuum permeability (μ₀)
    // -----------------------------------------------------------------------------

    /// Vacuum permeability (derived)
    /// μ₀ = 4π×10⁻⁷ H/m ≈ 1.25663706212...×10⁻⁶ H/m
    /// Note: No longer exactly 4π×10⁻⁷ after 2019 SI redefinition
    inline constexpr long double vacuum_permeability_value = 1.25663706212e-6L;

    // -----------------------------------------------------------------------------
    // Fine-structure constant (α)
    // -----------------------------------------------------------------------------

    /// Fine-structure constant (dimensionless)
    /// α = e²/(4πε₀ℏc) ≈ 1/137.035999084
    /// Fundamental constant characterizing electromagnetic interaction strength
    inline constexpr long double fine_structure_constant_value = 7.2973525693e-3L;
    inline constexpr long double inverse_fine_structure_value  = 137.035999084L;

    // -----------------------------------------------------------------------------
    // Electron mass
    // -----------------------------------------------------------------------------

    /// Electron mass
    /// me = 9.1093837015×10⁻³¹ kg
    inline constexpr long double electron_mass_value = 9.1093837015e-31L;

    // -----------------------------------------------------------------------------
    // Proton mass
    // -----------------------------------------------------------------------------

    /// Proton mass
    /// mp = 1.67262192369×10⁻²⁷ kg
    inline constexpr long double proton_mass_value = 1.67262192369e-27L;

    // -----------------------------------------------------------------------------
    // Neutron mass
    // -----------------------------------------------------------------------------

    /// Neutron mass
    /// mn = 1.67492749804×10⁻²⁷ kg
    inline constexpr long double neutron_mass_value = 1.67492749804e-27L;

    // -----------------------------------------------------------------------------
    // Atomic mass constant
    // -----------------------------------------------------------------------------

    /// Atomic mass constant (unified atomic mass unit)
    /// mu = 1 Da = 1.66053906660×10⁻²⁷ kg
    inline constexpr long double atomic_mass_constant_value = 1.66053906660e-27L;

    // -----------------------------------------------------------------------------
    // Molar gas constant
    // -----------------------------------------------------------------------------

    /// Molar gas constant
    /// R = NA × k = 8.314462618... J/(mol⋅K)
    inline constexpr long double molar_gas_constant_value = 8.314462618L;

    // -----------------------------------------------------------------------------
    // Stefan-Boltzmann constant
    // -----------------------------------------------------------------------------

    /// Stefan-Boltzmann constant
    /// σ = (π²/60) × (k⁴/(ℏ³c²)) = 5.670374419...×10⁻⁸ W/(m²⋅K⁴)
    inline constexpr long double stefan_boltzmann_constant_value = 5.670374419e-8L;

    // -----------------------------------------------------------------------------
    // Standard gravity
    // -----------------------------------------------------------------------------

    /// Standard acceleration due to gravity
    /// g = 9.80665 m/s² (exactly, by definition)
    inline constexpr long double standard_gravity_value = 9.80665L;

    // -----------------------------------------------------------------------------
    // Standard atmosphere
    // -----------------------------------------------------------------------------

    /// Standard atmospheric pressure
    /// atm = 101325 Pa (exactly, by definition)
    inline constexpr std::int64_t standard_atmosphere_value = 10'1325LL;

  } // namespace constants

  // =============================================================================
  // Additional constant quantities
  // =============================================================================

  /// Standard acceleration due to gravity
  /// g = 9.80665 m/s² (exactly)
  inline constexpr auto standard_gravity
      = quantity<reference<isq::acceleration_t, decltype(metre / (second * second))>, long double> {
          constants::standard_gravity_value
        };

  /// Commonly used alias
  inline constexpr auto g_0 = standard_gravity;
  inline constexpr auto g_n = standard_gravity;

  /// Electron mass
  /// me = 9.1093837015×10⁻³¹ kg
  inline constexpr auto electron_mass = quantity<reference<isq::mass_t, kilogram_t>, long double> {
    constants::electron_mass_value
  };

  /// Commonly used alias
  inline constexpr auto m_e = electron_mass;

  /// Proton mass
  /// mp = 1.67262192369×10⁻²⁷ kg
  inline constexpr auto proton_mass
      = quantity<reference<isq::mass_t, kilogram_t>, long double> { constants::proton_mass_value };

  /// Commonly used alias
  inline constexpr auto m_p = proton_mass;

  /// Atomic mass constant (unified atomic mass unit)
  /// mu = 1 Da = 1.66053906660×10⁻²⁷ kg
  inline constexpr auto atomic_mass_constant
      = quantity<reference<isq::mass_t, kilogram_t>, long double> {
          constants::atomic_mass_constant_value
        };

  /// Commonly used alias (dalton)
  inline constexpr auto u      = atomic_mass_constant;
  inline constexpr auto dalton = atomic_mass_constant;

  /// Molar gas constant
  /// R = 8.314462618... J/(mol⋅K)
  inline constexpr auto molar_gas_constant = quantity<
      reference<decltype(isq::energy / (isq::amount_of_substance * isq::thermodynamic_temperature)),
                decltype(joule / (mole * kelvin))>,
      long double> { constants::molar_gas_constant_value };

  /// Commonly used alias
  inline constexpr auto R = molar_gas_constant;

  /// Standard atmospheric pressure
  /// atm = 101325 Pa (exactly)
  inline constexpr auto standard_atmosphere
      = quantity<reference<isq::pressure_t, pascal_t>, std::int64_t> {
          constants::standard_atmosphere_value
        };

  /// Commonly used alias
  inline constexpr auto atm = standard_atmosphere;

  // =============================================================================
  // Static assertions
  // =============================================================================

  // Verify constant types
  static_assert(Quantity<decltype(speed_of_light)>);
  static_assert(Quantity<decltype(planck_constant)>);
  static_assert(Quantity<decltype(elementary_charge)>);
  static_assert(Quantity<decltype(boltzmann_constant)>);
  static_assert(Quantity<decltype(avogadro_constant)>);
  static_assert(Quantity<decltype(standard_gravity)>);
  static_assert(Quantity<decltype(electron_mass)>);
  static_assert(Quantity<decltype(standard_atmosphere)>);

  // Verify values
  static_assert(speed_of_light.value() == 2'9979'2458LL);
  static_assert(standard_atmosphere.value() == 10'1325LL);

} // namespace units::si
