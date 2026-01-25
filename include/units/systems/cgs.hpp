// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/quantity_spec.hpp>
#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/isq/mechanics.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/prefixes.hpp>

namespace units::cgs {

  // =============================================================================
  // CGS Base Units
  // =============================================================================
  // The CGS (centimetre-gram-second) system uses:
  // - centimetre for length (1 cm = 0.01 m)
  // - gram for mass (1 g = 0.001 kg)
  // - second for time (same as SI)
  //
  // Note: centimetre and gram are already defined in SI prefixes.
  // We re-export them here for convenience and define CGS-specific derived units.

  // Re-export base units from SI
  using si::centimeter; // American spelling
  using si::centimetre;
  using si::centimetre_t;
  using si::gram;
  using si::gram_t;
  using si::second;
  using si::second_t;

  // =============================================================================
  // CGS Mechanical Derived Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Force: dyne (dyn)
  // -----------------------------------------------------------------------------

  /// dyne - CGS unit of force
  /// Symbol: dyn
  /// Definition: 1 dyn = 1 g⋅cm/s² = 10⁻⁵ N
  /// The force required to accelerate 1 gram at 1 cm/s²
  struct dyne_t final : named_unit<"dyn", kind_of<isq::force_t>, mag_ratio<1, 10'0000>> {};
  inline constexpr dyne_t dyne {};

  // -----------------------------------------------------------------------------
  // Energy: erg
  // -----------------------------------------------------------------------------

  /// erg - CGS unit of energy
  /// Symbol: erg
  /// Definition: 1 erg = 1 g⋅cm²/s² = 1 dyn⋅cm = 10⁻⁷ J
  /// The work done by a force of one dyne over one centimetre
  struct erg_t final : named_unit<"erg", kind_of<isq::energy_t>, mag_ratio<1, 1000'0000>> {};
  inline constexpr erg_t erg {};

  // -----------------------------------------------------------------------------
  // Power: erg per second
  // -----------------------------------------------------------------------------

  /// erg_per_second - CGS unit of power
  /// Symbol: erg/s
  /// Definition: 1 erg/s = 10⁻⁷ W
  struct erg_per_second_t final
      : named_unit<"erg/s", kind_of<isq::power_t>, mag_ratio<1, 1000'0000>> {};
  inline constexpr erg_per_second_t erg_per_second {};

  // -----------------------------------------------------------------------------
  // Pressure: barye (Ba)
  // -----------------------------------------------------------------------------

  /// barye - CGS unit of pressure
  /// Symbol: Ba
  /// Definition: 1 Ba = 1 dyn/cm² = 0.1 Pa = 10⁻¹ Pa
  /// Also known as barad
  struct barye_t final : named_unit<"Ba", kind_of<isq::pressure_t>, mag_ratio<1, 10>> {};
  inline constexpr barye_t barye {};
  inline constexpr barye_t barad = barye;

  // -----------------------------------------------------------------------------
  // Dynamic Viscosity: poise (P)
  // -----------------------------------------------------------------------------

  /// poise - CGS unit of dynamic viscosity
  /// Symbol: P
  /// Definition: 1 P = 1 g/(cm⋅s) = 0.1 Pa⋅s = 0.1 kg/(m⋅s)
  /// Named after Jean Léonard Marie Poiseuille
  struct poise_t final : named_unit<"P", kind_of<isq::dimensionless_t>, mag_ratio<1, 10>> {};
  inline constexpr poise_t poise {};

  /// centipoise - commonly used submultiple
  /// Symbol: cP
  /// Definition: 1 cP = 0.01 P = 0.001 Pa⋅s = 1 mPa⋅s
  struct centipoise_t final : scaled_unit<"cP", poise_t, mag_ratio<1, 100>> {};
  inline constexpr centipoise_t centipoise {};

  // -----------------------------------------------------------------------------
  // Kinematic Viscosity: stokes (St)
  // -----------------------------------------------------------------------------

  /// stokes - CGS unit of kinematic viscosity
  /// Symbol: St
  /// Definition: 1 St = 1 cm²/s = 10⁻⁴ m²/s
  /// Named after George Gabriel Stokes
  struct stokes_t final : named_unit<"St", kind_of<isq::dimensionless_t>, mag_ratio<1, 10000>> {};
  inline constexpr stokes_t stokes {};

  /// centistokes - commonly used submultiple
  /// Symbol: cSt
  /// Definition: 1 cSt = 0.01 St = 10⁻⁶ m²/s = 1 mm²/s
  struct centistokes_t final : scaled_unit<"cSt", stokes_t, mag_ratio<1, 100>> {};
  inline constexpr centistokes_t centistokes {};

  // -----------------------------------------------------------------------------
  // Acceleration: gal (Gal)
  // -----------------------------------------------------------------------------

  /// gal - CGS unit of acceleration
  /// Symbol: Gal
  /// Definition: 1 Gal = 1 cm/s² = 0.01 m/s²
  /// Used primarily in gravimetry; named after Galileo Galilei
  struct gal_t final : named_unit<"Gal", kind_of<isq::acceleration_t>, mag_ratio<1, 100>> {};
  inline constexpr gal_t gal {};

  /// milligal - commonly used submultiple
  /// Symbol: mGal
  /// Definition: 1 mGal = 0.001 Gal = 10⁻⁵ m/s²
  struct milligal_t final : scaled_unit<"mGal", gal_t, mag_ratio<1, 1000>> {};
  inline constexpr milligal_t milligal {};

  // =============================================================================
  // CGS Electromagnetic Units (Gaussian CGS)
  // =============================================================================
  // The Gaussian CGS system is commonly used in physics, particularly
  // in electromagnetism. Note that Gaussian units have different
  // dimensional analysis than SI electromagnetic units.

  // -----------------------------------------------------------------------------
  // Magnetic Flux Density: gauss (G)
  // -----------------------------------------------------------------------------

  /// gauss - CGS unit of magnetic flux density
  /// Symbol: G
  /// Definition: 1 G = 10⁻⁴ T = 0.0001 T
  /// Named after Carl Friedrich Gauss
  struct gauss_t final : named_unit<"G", kind_of<isq::dimensionless_t>, mag_ratio<1, 10000>> {};
  inline constexpr gauss_t gauss {};

  // -----------------------------------------------------------------------------
  // Magnetic Flux: maxwell (Mx)
  // -----------------------------------------------------------------------------

  /// maxwell - CGS unit of magnetic flux
  /// Symbol: Mx
  /// Definition: 1 Mx = 10⁻⁸ Wb = 10⁻⁸ V⋅s
  /// Named after James Clerk Maxwell
  struct maxwell_t final
      : named_unit<"Mx", kind_of<isq::dimensionless_t>, mag_ratio<1, 1'0000'0000>> {};
  inline constexpr maxwell_t maxwell {};

  // -----------------------------------------------------------------------------
  // Magnetic Field Strength: oersted (Oe)
  // -----------------------------------------------------------------------------

  /// oersted - CGS unit of magnetic field strength
  /// Symbol: Oe
  /// Definition: 1 Oe = 1000/(4π) A/m ≈ 79.5775 A/m
  /// For simplicity, we use the approximation 1000/4π ≈ 250/π
  /// Named after Hans Christian Ørsted
  ///
  /// Note: The exact conversion is 1 Oe = 1000/(4π) A/m
  /// We approximate this as 79577/1000 ≈ 79.577 for rational arithmetic
  struct oersted_t final
      : named_unit<"Oe", kind_of<isq::dimensionless_t>, mag_ratio<79577, 100'0000>> {};
  inline constexpr oersted_t oersted {};

  // =============================================================================
  // CGS Optical Units
  // =============================================================================

  // -----------------------------------------------------------------------------
  // Luminance: stilb (sb)
  // -----------------------------------------------------------------------------

  /// stilb - CGS unit of luminance
  /// Symbol: sb
  /// Definition: 1 sb = 1 cd/cm² = 10⁴ cd/m²
  struct stilb_t final : named_unit<"sb", kind_of<isq::luminous_intensity_t>, mag<10000>> {};
  inline constexpr stilb_t stilb {};

  // -----------------------------------------------------------------------------
  // Illuminance: phot (ph)
  // -----------------------------------------------------------------------------

  /// phot - CGS unit of illuminance
  /// Symbol: ph
  /// Definition: 1 ph = 1 lm/cm² = 10⁴ lx
  struct phot_t final : named_unit<"ph", kind_of<isq::luminous_intensity_t>, mag<10000>> {};
  inline constexpr phot_t phot {};

  // =============================================================================
  // Conversion utilities
  // =============================================================================

  namespace detail {

    /// Conversion factor from dyne to newton
    inline constexpr double dyne_to_newton = 1e-5;

    /// Conversion factor from erg to joule
    inline constexpr double erg_to_joule = 1e-7;

    /// Conversion factor from barye to pascal
    inline constexpr double barye_to_pascal = 0.1;

    /// Conversion factor from poise to pascal-second
    inline constexpr double poise_to_pascal_second = 0.1;

    /// Conversion factor from stokes to m²/s
    inline constexpr double stokes_to_m2_per_s = 1e-4;

    /// Conversion factor from gauss to tesla
    inline constexpr double gauss_to_tesla = 1e-4;

    /// Conversion factor from maxwell to weber
    inline constexpr double maxwell_to_weber = 1e-8;

  } // namespace detail

  // =============================================================================
  // Static assertions to verify CGS unit properties
  // =============================================================================

  // All CGS units satisfy Unit concept
  static_assert(Unit<dyne_t>);
  static_assert(Unit<erg_t>);
  static_assert(Unit<barye_t>);
  static_assert(Unit<poise_t>);
  static_assert(Unit<stokes_t>);
  static_assert(Unit<gal_t>);
  static_assert(Unit<gauss_t>);
  static_assert(Unit<maxwell_t>);
  static_assert(Unit<oersted_t>);
  static_assert(Unit<stilb_t>);
  static_assert(Unit<phot_t>);

  // All CGS units satisfy NamedUnit concept
  static_assert(NamedUnit<dyne_t>);
  static_assert(NamedUnit<erg_t>);
  static_assert(NamedUnit<barye_t>);
  static_assert(NamedUnit<poise_t>);
  static_assert(NamedUnit<stokes_t>);
  static_assert(NamedUnit<gal_t>);

  // Verify magnitude values (conversion factors to SI)
  // dyne: 1 dyn = 10^-5 N
  static_assert(dyne_t::magnitude_type::num == 1);
  static_assert(dyne_t::magnitude_type::den == 10'0000);

  // erg: 1 erg = 10^-7 J
  static_assert(erg_t::magnitude_type::num == 1);
  static_assert(erg_t::magnitude_type::den == 1000'0000);

  // barye: 1 Ba = 0.1 Pa = 1/10 Pa
  static_assert(barye_t::magnitude_type::num == 1);
  static_assert(barye_t::magnitude_type::den == 10);

  // poise: 1 P = 0.1 Pa⋅s
  static_assert(poise_t::magnitude_type::num == 1);
  static_assert(poise_t::magnitude_type::den == 10);

  // stokes: 1 St = 10^-4 m²/s
  static_assert(stokes_t::magnitude_type::num == 1);
  static_assert(stokes_t::magnitude_type::den == 10000);

  // gal: 1 Gal = 0.01 m/s²
  static_assert(gal_t::magnitude_type::num == 1);
  static_assert(gal_t::magnitude_type::den == 100);

  // gauss: 1 G = 10^-4 T
  static_assert(gauss_t::magnitude_type::num == 1);
  static_assert(gauss_t::magnitude_type::den == 10000);

  // maxwell: 1 Mx = 10^-8 Wb
  static_assert(maxwell_t::magnitude_type::num == 1);
  static_assert(maxwell_t::magnitude_type::den == 1'0000'0000);

  // Verify symbols
  static_assert(dyne_t::symbol == fixed_string { "dyn" });
  static_assert(erg_t::symbol == fixed_string { "erg" });
  static_assert(barye_t::symbol == fixed_string { "Ba" });
  static_assert(poise_t::symbol == fixed_string { "P" });
  static_assert(stokes_t::symbol == fixed_string { "St" });
  static_assert(gal_t::symbol == fixed_string { "Gal" });
  static_assert(gauss_t::symbol == fixed_string { "G" });
  static_assert(maxwell_t::symbol == fixed_string { "Mx" });

  // Verify scaled units
  static_assert(centipoise_t::magnitude_type::num == 1);
  static_assert(centipoise_t::magnitude_type::den == 1000);
  static_assert(centistokes_t::magnitude_type::num == 1);
  static_assert(centistokes_t::magnitude_type::den == 100'0000);
  static_assert(milligal_t::magnitude_type::num == 1);
  static_assert(milligal_t::magnitude_type::den == 10'0000);

} // namespace units::cgs
