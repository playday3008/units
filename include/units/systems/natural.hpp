// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/quantity.hpp>
#include <units/core/quantity_spec.hpp>
#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/isq/mechanics.hpp>

namespace units::natural {

  // =============================================================================
  // Natural Units
  // =============================================================================
  // Natural units are systems of measurement where certain fundamental physical
  // constants are set to unity (1). The most common system sets:
  //
  //   c = ℏ = 1
  //
  // This is widely used in particle physics and theoretical physics because:
  // - Energy, mass, and momentum all have the same dimension
  // - Length and time have the dimension of inverse energy
  // - The fundamental scale is typically 1 GeV
  //
  // In natural units:
  // - [Energy] = [Mass] = [Momentum] = [Temperature] = GeV
  // - [Length] = [Time] = GeV⁻¹
  // - [Action] = [Angular momentum] = dimensionless
  //
  // Note: Due to integer overflow limitations in compile-time ratios,
  // we define these units with magnitude = 1 and document the actual
  // conversion factors. Runtime conversions can use the constants below.

  // =============================================================================
  // Natural unit system constants (for runtime conversions)
  // =============================================================================

  namespace constants {

    /// Conversion factor: 1 eV in joules (exact)
    inline constexpr long double eV_in_joules = 1.602176634e-19L;

    /// Conversion factor: 1 GeV in joules
    inline constexpr long double GeV_in_joules = 1.602176634e-10L;

    /// Conversion factor: 1 GeV⁻¹ in metres (ℏc/GeV)
    inline constexpr long double inverse_GeV_in_metres = 1.97326980459e-16L;

    /// Conversion factor: 1 GeV⁻¹ in seconds (ℏ/GeV)
    inline constexpr long double inverse_GeV_in_seconds = 6.582119569e-25L;

    /// Conversion factor: 1 GeV/c² in kilograms
    inline constexpr long double GeV_c2_in_kilograms = 1.78266192162e-27L;

    /// Speed of light in natural units
    inline constexpr long double c_natural = 1.0L;

    /// Reduced Planck constant in natural units
    inline constexpr long double hbar_natural = 1.0L;

  } // namespace constants

  // =============================================================================
  // Energy units: electron volt (eV) and derivatives
  // =============================================================================
  // We define eV-scale units with magnitude 1, as dimensionless energy units
  // in the natural unit system. For SI conversions, use the constants above.

  /// electronvolt - fundamental energy unit in particle physics
  /// Symbol: eV
  /// Definition: 1 eV = 1.602176634×10⁻¹⁹ J (exactly)
  /// In natural unit context, this serves as a dimensionless energy scale
  struct electronvolt_t final : named_unit<"eV", kind_of<isq::energy_t>> {};
  inline constexpr electronvolt_t electronvolt {};
  inline constexpr electronvolt_t eV = electronvolt;

  /// kiloelectronvolt - 1 keV = 10³ eV
  struct kiloelectronvolt_t final : scaled_unit<"keV", electronvolt_t, mag<1000>> {};
  inline constexpr kiloelectronvolt_t kiloelectronvolt {};
  inline constexpr kiloelectronvolt_t keV = kiloelectronvolt;

  /// megaelectronvolt - 1 MeV = 10⁶ eV
  struct megaelectronvolt_t final : scaled_unit<"MeV", electronvolt_t, mag<100'0000>> {};
  inline constexpr megaelectronvolt_t megaelectronvolt {};
  inline constexpr megaelectronvolt_t MeV = megaelectronvolt;

  /// gigaelectronvolt - 1 GeV = 10⁹ eV (fundamental unit in particle physics)
  struct gigaelectronvolt_t final : scaled_unit<"GeV", electronvolt_t, mag<10'0000'0000>> {};
  inline constexpr gigaelectronvolt_t gigaelectronvolt {};
  inline constexpr gigaelectronvolt_t GeV = gigaelectronvolt;

  /// teraelectronvolt - 1 TeV = 10¹² eV
  struct teraelectronvolt_t final : scaled_unit<"TeV", electronvolt_t, mag<1'0000'0000'0000>> {};
  inline constexpr teraelectronvolt_t teraelectronvolt {};
  inline constexpr teraelectronvolt_t TeV = teraelectronvolt;

  // =============================================================================
  // Mass in natural units (GeV/c² → GeV when c=1)
  // =============================================================================
  // In natural units with c=1, mass has units of energy.
  // We define explicit mass units for clarity and type safety.

  /// electronvolt_per_c2 - mass unit
  /// Symbol: eV/c²
  /// When c=1, this is equivalent to eV
  struct electronvolt_per_c2_t final : named_unit<"eV/c2", kind_of<isq::mass_t>> {};
  inline constexpr electronvolt_per_c2_t electronvolt_per_c2 {};
  inline constexpr electronvolt_per_c2_t eV_c2 = electronvolt_per_c2;

  /// MeV/c² - common unit for particle masses
  struct megaelectronvolt_per_c2_t final
      : scaled_unit<"MeV/c2", electronvolt_per_c2_t, mag<100'0000>> {};
  inline constexpr megaelectronvolt_per_c2_t megaelectronvolt_per_c2 {};
  inline constexpr megaelectronvolt_per_c2_t MeV_c2 = megaelectronvolt_per_c2;

  /// GeV/c² - common unit for particle masses
  struct gigaelectronvolt_per_c2_t final
      : scaled_unit<"GeV/c2", electronvolt_per_c2_t, mag<10'0000'0000>> {};
  inline constexpr gigaelectronvolt_per_c2_t gigaelectronvolt_per_c2 {};
  inline constexpr gigaelectronvolt_per_c2_t GeV_c2 = gigaelectronvolt_per_c2;

  // =============================================================================
  // Momentum in natural units (GeV/c → GeV when c=1)
  // =============================================================================

  /// electronvolt_per_c - momentum unit
  /// Symbol: eV/c
  /// When c=1, this is equivalent to eV
  struct electronvolt_per_c_t final : named_unit<"eV/c", kind_of<isq::dimensionless_t>> {};
  inline constexpr electronvolt_per_c_t electronvolt_per_c {};
  inline constexpr electronvolt_per_c_t eV_c = electronvolt_per_c;

  /// MeV/c - momentum unit
  struct megaelectronvolt_per_c_t final
      : scaled_unit<"MeV/c", electronvolt_per_c_t, mag<100'0000>> {};
  inline constexpr megaelectronvolt_per_c_t megaelectronvolt_per_c {};
  inline constexpr megaelectronvolt_per_c_t MeV_c = megaelectronvolt_per_c;

  /// GeV/c - momentum unit
  struct gigaelectronvolt_per_c_t final
      : scaled_unit<"GeV/c", electronvolt_per_c_t, mag<10'0000'0000>> {};
  inline constexpr gigaelectronvolt_per_c_t gigaelectronvolt_per_c {};
  inline constexpr gigaelectronvolt_per_c_t GeV_c = gigaelectronvolt_per_c;

  // =============================================================================
  // Length in natural units (inverse energy: ℏc/E)
  // =============================================================================
  // In natural units, length has dimension of inverse energy.
  // 1 fm ≈ 5.068 GeV⁻¹

  /// fermi (femtometre) - common length unit in nuclear/particle physics
  /// Symbol: fm
  /// Definition: 1 fm = 10⁻¹⁵ m
  struct fermi_t final
      : named_unit<"fm", kind_of<isq::length_t>, mag_ratio<1, 1000'0000'0000'0000>> {};
  inline constexpr fermi_t fermi {};
  inline constexpr fermi_t femtometre = fermi;
  inline constexpr fermi_t femtometer = fermi;
  inline constexpr fermi_t fm         = fermi;

  // =============================================================================
  // Cross-section units
  // =============================================================================
  // The barn is a unit of area used in nuclear and particle physics for
  // cross-sections. 1 b = 10⁻²⁸ m² = 100 fm²
  //
  // Note: Due to intmax_t overflow constraints, we define barn units with
  // magnitude = 1 (treating barn as the base unit within this system).
  // For SI conversions, use runtime multiplication with 1e-28.

  /// barn - cross-section unit (base for this system)
  /// Symbol: b
  /// Definition: 1 b = 10⁻²⁸ m² = 100 fm²
  struct barn_t final : named_unit<"b", kind_of<isq::dimensionless_t>> {};
  inline constexpr barn_t barn {};

  /// millibarn - 1 mb = 10⁻³ b
  struct millibarn_t final : named_unit<"mb", kind_of<isq::dimensionless_t>, mag_ratio<1, 1000>> {};
  inline constexpr millibarn_t millibarn {};
  inline constexpr millibarn_t mb = millibarn;

  /// microbarn - 1 μb = 10⁻⁶ b
  struct microbarn_t final
      : named_unit<"ub", kind_of<isq::dimensionless_t>, mag_ratio<1, 100'0000>> {};
  inline constexpr microbarn_t microbarn {};
  inline constexpr microbarn_t ub = microbarn;

  /// nanobarn - 1 nb = 10⁻⁹ b
  struct nanobarn_t final
      : named_unit<"nb", kind_of<isq::dimensionless_t>, mag_ratio<1, 10'0000'0000>> {};
  inline constexpr nanobarn_t nanobarn {};
  inline constexpr nanobarn_t nb = nanobarn;

  /// picobarn - 1 pb = 10⁻¹² b
  struct picobarn_t final
      : named_unit<"pb", kind_of<isq::dimensionless_t>, mag_ratio<1, 1'0000'0000'0000>> {};
  inline constexpr picobarn_t picobarn {};
  inline constexpr picobarn_t pb = picobarn;

  /// femtobarn - 1 fb = 10⁻¹⁵ b (common unit at LHC)
  struct femtobarn_t final
      : named_unit<"fb", kind_of<isq::dimensionless_t>, mag_ratio<1, 1000'0000'0000'0000>> {};
  inline constexpr femtobarn_t femtobarn {};
  inline constexpr femtobarn_t fb = femtobarn;

  // =============================================================================
  // Particle masses in natural units (reference values in MeV/c²)
  // =============================================================================

  namespace particle_masses {

    /// Electron mass: m_e = 0.51099895000 MeV/c²
    inline constexpr long double electron = 0.51099895000L;

    /// Muon mass: m_μ = 105.6583755 MeV/c²
    inline constexpr long double muon = 105.6583755L;

    /// Tau lepton mass: m_τ = 1776.86 MeV/c²
    inline constexpr long double tau = 1776.86L;

    /// Proton mass: m_p = 938.27208816 MeV/c²
    inline constexpr long double proton = 938.27208816L;

    /// Neutron mass: m_n = 939.56542052 MeV/c²
    inline constexpr long double neutron = 939.56542052L;

    /// W boson mass: m_W = 80377 MeV/c² = 80.377 GeV/c²
    inline constexpr long double w_boson = 80377.0L;

    /// Z boson mass: m_Z = 91187.6 MeV/c² = 91.1876 GeV/c²
    inline constexpr long double z_boson = 91187.6L;

    /// Higgs boson mass: m_H = 125250 MeV/c² = 125.25 GeV/c²
    inline constexpr long double higgs = 125250.0L;

    /// Top quark mass: m_t = 172690 MeV/c² = 172.69 GeV/c²
    inline constexpr long double top_quark = 172690.0L;

    /// Charged pion mass: m_π± = 139.57039 MeV/c²
    inline constexpr long double charged_pion = 139.57039L;

    /// Neutral pion mass: m_π⁰ = 134.9768 MeV/c²
    inline constexpr long double neutral_pion = 134.9768L;

    /// Charged kaon mass: m_K± = 493.677 MeV/c²
    inline constexpr long double charged_kaon = 493.677L;

  } // namespace particle_masses

  // =============================================================================
  // Planck units (c = ℏ = G = 1)
  // =============================================================================
  // Planck units extend natural units by also setting the gravitational constant
  // G = 1. This creates a system where all quantities have no dimension in the
  // fundamental sense, scaled by Planck-scale values.

  namespace planck {

    /// Planck length: l_P = √(ℏG/c³) = 1.616255×10⁻³⁵ m
    inline constexpr long double length = 1.616255e-35L;

    /// Planck time: t_P = √(ℏG/c⁵) = 5.391247×10⁻⁴⁴ s
    inline constexpr long double time = 5.391247e-44L;

    /// Planck mass: m_P = √(ℏc/G) = 2.176434×10⁻⁸ kg = 1.220890×10¹⁹ GeV/c²
    inline constexpr long double mass_kg  = 2.176434e-8L;
    inline constexpr long double mass_gev = 1.220890e19L;

    /// Planck energy: E_P = m_P c² = 1.220890×10¹⁹ GeV
    inline constexpr long double energy_gev = 1.220890e19L;

    /// Planck temperature: T_P = m_P c²/k = 1.416784×10³² K
    inline constexpr long double temperature = 1.416784e32L;

    /// Planck charge: q_P = √(4πε₀ℏc) ≈ 1.875546×10⁻¹⁸ C
    inline constexpr long double charge = 1.875546e-18L;

  } // namespace planck

  // =============================================================================
  // Static assertions
  // =============================================================================

  // Verify unit types
  static_assert(Unit<electronvolt_t>);
  static_assert(Unit<megaelectronvolt_t>);
  static_assert(Unit<gigaelectronvolt_t>);
  static_assert(Unit<teraelectronvolt_t>);
  static_assert(Unit<fermi_t>);
  static_assert(Unit<barn_t>);
  static_assert(Unit<femtobarn_t>);

  // Verify NamedUnit
  static_assert(NamedUnit<electronvolt_t>);
  static_assert(NamedUnit<fermi_t>);
  static_assert(NamedUnit<barn_t>);

  // Verify ScaledUnit
  static_assert(ScaledUnit<megaelectronvolt_t>);
  static_assert(ScaledUnit<gigaelectronvolt_t>);

  // Note: barn units are NamedUnits (not scaled) to avoid overflow
  static_assert(NamedUnit<millibarn_t>);
  static_assert(NamedUnit<femtobarn_t>);

  // Verify symbols
  static_assert(electronvolt_t::symbol == fixed_string { "eV" });
  static_assert(megaelectronvolt_t::symbol == fixed_string { "MeV" });
  static_assert(gigaelectronvolt_t::symbol == fixed_string { "GeV" });
  static_assert(fermi_t::symbol == fixed_string { "fm" });
  static_assert(barn_t::symbol == fixed_string { "b" });
  static_assert(femtobarn_t::symbol == fixed_string { "fb" });

  // Verify energy unit magnitudes (relative to eV base)
  static_assert(kiloelectronvolt_t::magnitude_type::num == 1000);
  static_assert(kiloelectronvolt_t::magnitude_type::den == 1);
  static_assert(megaelectronvolt_t::magnitude_type::num == 100'0000);
  static_assert(megaelectronvolt_t::magnitude_type::den == 1);
  static_assert(gigaelectronvolt_t::magnitude_type::num == 10'0000'0000);
  static_assert(gigaelectronvolt_t::magnitude_type::den == 1);

  // =============================================================================
  // User-defined literals
  // =============================================================================

  namespace literals {

    // -------------------------------------------------------------------------
    // Energy literals (eV, keV, MeV, GeV, TeV)
    // -------------------------------------------------------------------------

    /// User-defined literal for electronvolts (integer)
    [[nodiscard]]
    consteval auto operator""_eV(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), electronvolt);
    }

    /// User-defined literal for electronvolts (floating point)
    [[nodiscard]]
    consteval auto operator""_eV(long double val) noexcept {
      return make_quantity(static_cast<double>(val), electronvolt);
    }

    /// User-defined literal for kiloelectronvolts (integer)
    [[nodiscard]]
    consteval auto operator""_keV(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), kiloelectronvolt);
    }

    /// User-defined literal for kiloelectronvolts (floating point)
    [[nodiscard]]
    consteval auto operator""_keV(long double val) noexcept {
      return make_quantity(static_cast<double>(val), kiloelectronvolt);
    }

    /// User-defined literal for megaelectronvolts (integer)
    [[nodiscard]]
    consteval auto operator""_MeV(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), megaelectronvolt);
    }

    /// User-defined literal for megaelectronvolts (floating point)
    [[nodiscard]]
    consteval auto operator""_MeV(long double val) noexcept {
      return make_quantity(static_cast<double>(val), megaelectronvolt);
    }

    /// User-defined literal for gigaelectronvolts (integer)
    [[nodiscard]]
    consteval auto operator""_GeV(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), gigaelectronvolt);
    }

    /// User-defined literal for gigaelectronvolts (floating point)
    [[nodiscard]]
    consteval auto operator""_GeV(long double val) noexcept {
      return make_quantity(static_cast<double>(val), gigaelectronvolt);
    }

    /// User-defined literal for teraelectronvolts (integer)
    [[nodiscard]]
    consteval auto operator""_TeV(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), teraelectronvolt);
    }

    /// User-defined literal for teraelectronvolts (floating point)
    [[nodiscard]]
    consteval auto operator""_TeV(long double val) noexcept {
      return make_quantity(static_cast<double>(val), teraelectronvolt);
    }

    // -------------------------------------------------------------------------
    // Mass literals (eV/c², MeV/c², GeV/c²)
    // -------------------------------------------------------------------------

    /// User-defined literal for eV/c² (integer)
    [[nodiscard]]
    consteval auto operator""_eVc2(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), electronvolt_per_c2);
    }

    /// User-defined literal for eV/c² (floating point)
    [[nodiscard]]
    consteval auto operator""_eVc2(long double val) noexcept {
      return make_quantity(static_cast<double>(val), electronvolt_per_c2);
    }

    /// User-defined literal for MeV/c² (integer)
    [[nodiscard]]
    consteval auto operator""_MeVc2(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), megaelectronvolt_per_c2);
    }

    /// User-defined literal for MeV/c² (floating point)
    [[nodiscard]]
    consteval auto operator""_MeVc2(long double val) noexcept {
      return make_quantity(static_cast<double>(val), megaelectronvolt_per_c2);
    }

    /// User-defined literal for GeV/c² (integer)
    [[nodiscard]]
    consteval auto operator""_GeVc2(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), gigaelectronvolt_per_c2);
    }

    /// User-defined literal for GeV/c² (floating point)
    [[nodiscard]]
    consteval auto operator""_GeVc2(long double val) noexcept {
      return make_quantity(static_cast<double>(val), gigaelectronvolt_per_c2);
    }

    // -------------------------------------------------------------------------
    // Momentum literals (eV/c, MeV/c, GeV/c)
    // -------------------------------------------------------------------------

    /// User-defined literal for eV/c (integer)
    [[nodiscard]]
    consteval auto operator""_eVc(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), electronvolt_per_c);
    }

    /// User-defined literal for eV/c (floating point)
    [[nodiscard]]
    consteval auto operator""_eVc(long double val) noexcept {
      return make_quantity(static_cast<double>(val), electronvolt_per_c);
    }

    /// User-defined literal for MeV/c (integer)
    [[nodiscard]]
    consteval auto operator""_MeVc(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), megaelectronvolt_per_c);
    }

    /// User-defined literal for MeV/c (floating point)
    [[nodiscard]]
    consteval auto operator""_MeVc(long double val) noexcept {
      return make_quantity(static_cast<double>(val), megaelectronvolt_per_c);
    }

    /// User-defined literal for GeV/c (integer)
    [[nodiscard]]
    consteval auto operator""_GeVc(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), gigaelectronvolt_per_c);
    }

    /// User-defined literal for GeV/c (floating point)
    [[nodiscard]]
    consteval auto operator""_GeVc(long double val) noexcept {
      return make_quantity(static_cast<double>(val), gigaelectronvolt_per_c);
    }

    // -------------------------------------------------------------------------
    // Length literal (fermi/femtometre)
    // -------------------------------------------------------------------------

    /// User-defined literal for fermi/femtometre (integer)
    [[nodiscard]]
    consteval auto operator""_fm(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), fermi);
    }

    /// User-defined literal for fermi/femtometre (floating point)
    [[nodiscard]]
    consteval auto operator""_fm(long double val) noexcept {
      return make_quantity(static_cast<double>(val), fermi);
    }

    // -------------------------------------------------------------------------
    // Cross-section literals (barn and subunits)
    // -------------------------------------------------------------------------

    /// User-defined literal for barns (integer)
    [[nodiscard]]
    consteval auto operator""_b(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), barn);
    }

    /// User-defined literal for barns (floating point)
    [[nodiscard]]
    consteval auto operator""_b(long double val) noexcept {
      return make_quantity(static_cast<double>(val), barn);
    }

    /// User-defined literal for millibarns (integer)
    [[nodiscard]]
    consteval auto operator""_mb(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), millibarn);
    }

    /// User-defined literal for millibarns (floating point)
    [[nodiscard]]
    consteval auto operator""_mb(long double val) noexcept {
      return make_quantity(static_cast<double>(val), millibarn);
    }

    /// User-defined literal for microbarns (integer)
    [[nodiscard]]
    consteval auto operator""_ub(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), microbarn);
    }

    /// User-defined literal for microbarns (floating point)
    [[nodiscard]]
    consteval auto operator""_ub(long double val) noexcept {
      return make_quantity(static_cast<double>(val), microbarn);
    }

    /// User-defined literal for nanobarns (integer)
    [[nodiscard]]
    consteval auto operator""_nb(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), nanobarn);
    }

    /// User-defined literal for nanobarns (floating point)
    [[nodiscard]]
    consteval auto operator""_nb(long double val) noexcept {
      return make_quantity(static_cast<double>(val), nanobarn);
    }

    /// User-defined literal for picobarns (integer)
    [[nodiscard]]
    consteval auto operator""_pb(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), picobarn);
    }

    /// User-defined literal for picobarns (floating point)
    [[nodiscard]]
    consteval auto operator""_pb(long double val) noexcept {
      return make_quantity(static_cast<double>(val), picobarn);
    }

    /// User-defined literal for femtobarns (integer)
    [[nodiscard]]
    consteval auto operator""_fb(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), femtobarn);
    }

    /// User-defined literal for femtobarns (floating point)
    [[nodiscard]]
    consteval auto operator""_fb(long double val) noexcept {
      return make_quantity(static_cast<double>(val), femtobarn);
    }

  } // namespace literals

} // namespace units::natural
