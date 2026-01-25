// SPDX-License-Identifier: MIT

// Static assertion tests for Phase 6: Additional Unit Systems
// Tests CGS, Imperial, SI constants, and Natural unit systems

#include <units/core/quantity.hpp>
#include <units/core/unit.hpp>
#include <units/systems/cgs.hpp>
#include <units/systems/imperial.hpp>
#include <units/systems/natural.hpp>
#include <units/systems/si/constants.hpp>

namespace units::test {

  // =============================================================================
  // CGS Unit Tests
  // =============================================================================

  namespace cgs_tests {

    // Test that CGS units satisfy Unit concept
    static_assert(Unit<cgs::dyne_t>);
    static_assert(Unit<cgs::erg_t>);
    static_assert(Unit<cgs::barye_t>);
    static_assert(Unit<cgs::poise_t>);
    static_assert(Unit<cgs::stokes_t>);
    static_assert(Unit<cgs::gal_t>);
    static_assert(Unit<cgs::gauss_t>);
    static_assert(Unit<cgs::maxwell_t>);
    static_assert(Unit<cgs::oersted_t>);
    static_assert(Unit<cgs::stilb_t>);
    static_assert(Unit<cgs::phot_t>);

    // Test that CGS units satisfy NamedUnit concept
    static_assert(NamedUnit<cgs::dyne_t>);
    static_assert(NamedUnit<cgs::erg_t>);
    static_assert(NamedUnit<cgs::barye_t>);
    static_assert(NamedUnit<cgs::poise_t>);
    static_assert(NamedUnit<cgs::stokes_t>);
    static_assert(NamedUnit<cgs::gal_t>);

    // Test scaled CGS units
    static_assert(ScaledUnit<cgs::centipoise_t>);
    static_assert(ScaledUnit<cgs::centistokes_t>);
    static_assert(ScaledUnit<cgs::milligal_t>);

    // Verify CGS magnitude values (conversion factors to SI)
    // dyne: 1 dyn = 10^-5 N
    static_assert(cgs::dyne_t::magnitude_type::num == 1);
    static_assert(cgs::dyne_t::magnitude_type::den == 10'0000);

    // erg: 1 erg = 10^-7 J
    static_assert(cgs::erg_t::magnitude_type::num == 1);
    static_assert(cgs::erg_t::magnitude_type::den == 1000'0000);

    // barye: 1 Ba = 0.1 Pa
    static_assert(cgs::barye_t::magnitude_type::num == 1);
    static_assert(cgs::barye_t::magnitude_type::den == 10);

    // poise: 1 P = 0.1 Pa·s
    static_assert(cgs::poise_t::magnitude_type::num == 1);
    static_assert(cgs::poise_t::magnitude_type::den == 10);

    // stokes: 1 St = 10^-4 m²/s
    static_assert(cgs::stokes_t::magnitude_type::num == 1);
    static_assert(cgs::stokes_t::magnitude_type::den == 10000);

    // gal: 1 Gal = 0.01 m/s²
    static_assert(cgs::gal_t::magnitude_type::num == 1);
    static_assert(cgs::gal_t::magnitude_type::den == 100);

    // gauss: 1 G = 10^-4 T
    static_assert(cgs::gauss_t::magnitude_type::num == 1);
    static_assert(cgs::gauss_t::magnitude_type::den == 10000);

    // maxwell: 1 Mx = 10^-8 Wb
    static_assert(cgs::maxwell_t::magnitude_type::num == 1);
    static_assert(cgs::maxwell_t::magnitude_type::den == 1'0000'0000);

    // Verify symbols
    static_assert(cgs::dyne_t::symbol == fixed_string { "dyn" });
    static_assert(cgs::erg_t::symbol == fixed_string { "erg" });
    static_assert(cgs::barye_t::symbol == fixed_string { "Ba" });
    static_assert(cgs::poise_t::symbol == fixed_string { "P" });
    static_assert(cgs::stokes_t::symbol == fixed_string { "St" });
    static_assert(cgs::gal_t::symbol == fixed_string { "Gal" });
    static_assert(cgs::gauss_t::symbol == fixed_string { "G" });
    static_assert(cgs::maxwell_t::symbol == fixed_string { "Mx" });

    // Verify scaled unit magnitudes
    // centipoise: 1 cP = 0.001 Pa·s
    static_assert(cgs::centipoise_t::magnitude_type::num == 1);
    static_assert(cgs::centipoise_t::magnitude_type::den == 1000);

    // centistokes: 1 cSt = 10^-6 m²/s
    static_assert(cgs::centistokes_t::magnitude_type::num == 1);
    static_assert(cgs::centistokes_t::magnitude_type::den == 100'0000);

    // milligal: 1 mGal = 10^-5 m/s²
    static_assert(cgs::milligal_t::magnitude_type::num == 1);
    static_assert(cgs::milligal_t::magnitude_type::den == 10'0000);

  } // namespace cgs_tests

  // =============================================================================
  // Imperial Unit Tests
  // =============================================================================

  namespace imperial_tests {

    // Test that Imperial units satisfy Unit concept
    // Length
    static_assert(Unit<imperial::inch_t>);
    static_assert(Unit<imperial::foot_t>);
    static_assert(Unit<imperial::yard_t>);
    static_assert(Unit<imperial::mile_t>);
    static_assert(Unit<imperial::nautical_mile_t>);
    static_assert(Unit<imperial::fathom_t>);
    static_assert(Unit<imperial::chain_t>);
    static_assert(Unit<imperial::furlong_t>);
    static_assert(Unit<imperial::thou_t>);

    // Mass
    static_assert(Unit<imperial::grain_t>);
    static_assert(Unit<imperial::ounce_t>);
    static_assert(Unit<imperial::pound_t>);
    static_assert(Unit<imperial::stone_t>);
    static_assert(Unit<imperial::short_hundredweight_t>);
    static_assert(Unit<imperial::long_hundredweight_t>);
    static_assert(Unit<imperial::short_ton_t>);
    static_assert(Unit<imperial::long_ton_t>);

    // Volume
    static_assert(Unit<imperial::us_fluid_ounce_t>);
    static_assert(Unit<imperial::imperial_fluid_ounce_t>);
    static_assert(Unit<imperial::us_pint_t>);
    static_assert(Unit<imperial::imperial_pint_t>);
    static_assert(Unit<imperial::us_quart_t>);
    static_assert(Unit<imperial::imperial_quart_t>);
    static_assert(Unit<imperial::us_gallon_t>);
    static_assert(Unit<imperial::imperial_gallon_t>);
    static_assert(Unit<imperial::us_barrel_t>);

    // Area
    static_assert(Unit<imperial::acre_t>);
    static_assert(Unit<imperial::square_foot_t>);
    static_assert(Unit<imperial::square_inch_t>);
    static_assert(Unit<imperial::square_mile_t>);

    // Speed
    static_assert(Unit<imperial::mile_per_hour_t>);
    static_assert(Unit<imperial::knot_t>);
    static_assert(Unit<imperial::foot_per_second_t>);

    // Force, pressure, energy, power
    static_assert(Unit<imperial::pound_force_t>);
    static_assert(Unit<imperial::psi_t>);
    static_assert(Unit<imperial::foot_pound_t>);
    static_assert(Unit<imperial::btu_t>);
    static_assert(Unit<imperial::therm_t>);
    static_assert(Unit<imperial::horsepower_t>);

    // Test that Imperial units satisfy NamedUnit concept
    static_assert(NamedUnit<imperial::inch_t>);
    static_assert(NamedUnit<imperial::foot_t>);
    static_assert(NamedUnit<imperial::yard_t>);
    static_assert(NamedUnit<imperial::mile_t>);
    static_assert(NamedUnit<imperial::pound_t>);
    static_assert(NamedUnit<imperial::ounce_t>);
    static_assert(NamedUnit<imperial::us_gallon_t>);
    static_assert(NamedUnit<imperial::imperial_gallon_t>);

    // Verify key conversion factors (GCD-normalized)
    // inch: 1 in = 0.0254 m = 254/10000 m = 127/5000 m
    static_assert(imperial::inch_t::magnitude_type::num == 127);
    static_assert(imperial::inch_t::magnitude_type::den == 5000);

    // foot: 1 ft = 0.3048 m = 3048/10000 m = 381/1250 m
    static_assert(imperial::foot_t::magnitude_type::num == 381);
    static_assert(imperial::foot_t::magnitude_type::den == 1250);

    // yard: 1 yd = 0.9144 m = 9144/10000 m = 1143/1250 m
    static_assert(imperial::yard_t::magnitude_type::num == 1143);
    static_assert(imperial::yard_t::magnitude_type::den == 1250);

    // nautical mile: 1 nmi = 1852 m
    static_assert(imperial::nautical_mile_t::magnitude_type::num == 1852);
    static_assert(imperial::nautical_mile_t::magnitude_type::den == 1);

    // Verify length relationships
    // 12 inches = 1 foot
    // inch_mag * 12 = 127/5000 * 12 = 1524/5000 = 381/1250 = foot_mag ✓

    // 3 feet = 1 yard
    // foot_mag * 3 = 381/1250 * 3 = 1143/1250 = yard_mag ✓

    // Verify symbols
    static_assert(imperial::inch_t::symbol == fixed_string { "in" });
    static_assert(imperial::foot_t::symbol == fixed_string { "ft" });
    static_assert(imperial::yard_t::symbol == fixed_string { "yd" });
    static_assert(imperial::mile_t::symbol == fixed_string { "mi" });
    static_assert(imperial::nautical_mile_t::symbol == fixed_string { "nmi" });
    static_assert(imperial::pound_t::symbol == fixed_string { "lb" });
    static_assert(imperial::ounce_t::symbol == fixed_string { "oz" });
    static_assert(imperial::us_gallon_t::symbol == fixed_string { "gal" });
    static_assert(imperial::imperial_gallon_t::symbol == fixed_string { "gal" });
    static_assert(imperial::mile_per_hour_t::symbol == fixed_string { "mph" });
    static_assert(imperial::knot_t::symbol == fixed_string { "kn" });
    static_assert(imperial::horsepower_t::symbol == fixed_string { "hp" });

  } // namespace imperial_tests

  // =============================================================================
  // SI Constants Tests
  // =============================================================================

  namespace si_constants_tests {

    // Test that constant values are correct
    static_assert(si::constants::speed_of_light_value == 2'9979'2458LL);
    static_assert(si::constants::standard_atmosphere_value == 10'1325LL);
    static_assert(si::constants::luminous_efficacy_value == 683);

    // Test that quantities satisfy Quantity concept
    static_assert(Quantity<decltype(si::speed_of_light)>);
    static_assert(Quantity<decltype(si::planck_constant)>);
    static_assert(Quantity<decltype(si::elementary_charge)>);
    static_assert(Quantity<decltype(si::boltzmann_constant)>);
    static_assert(Quantity<decltype(si::avogadro_constant)>);
    static_assert(Quantity<decltype(si::standard_gravity)>);
    static_assert(Quantity<decltype(si::electron_mass)>);
    static_assert(Quantity<decltype(si::proton_mass)>);
    static_assert(Quantity<decltype(si::atomic_mass_constant)>);
    static_assert(Quantity<decltype(si::molar_gas_constant)>);
    static_assert(Quantity<decltype(si::standard_atmosphere)>);

    // Verify speed of light value
    static_assert(si::speed_of_light.value() == 2'9979'2458LL);
    static_assert(si::c.value() == 2'9979'2458LL);

    // Verify standard atmosphere value
    static_assert(si::standard_atmosphere.value() == 10'1325LL);
    static_assert(si::atm.value() == 10'1325LL);

    // Test derived constant values
    // Planck constant: h = 6.62607015×10⁻³⁴ J·s
    static_assert(si::constants::planck_constant_value > 6.6e-34L);
    static_assert(si::constants::planck_constant_value < 6.7e-34L);

    // Elementary charge: e = 1.602176634×10⁻¹⁹ C
    static_assert(si::constants::elementary_charge_value > 1.6e-19L);
    static_assert(si::constants::elementary_charge_value < 1.7e-19L);

    // Boltzmann constant: k = 1.380649×10⁻²³ J/K
    static_assert(si::constants::boltzmann_constant_value > 1.38e-23L);
    static_assert(si::constants::boltzmann_constant_value < 1.39e-23L);

    // Avogadro constant: NA = 6.02214076×10²³ mol⁻¹
    static_assert(si::constants::avogadro_constant_value > 6.0e23L);
    static_assert(si::constants::avogadro_constant_value < 6.1e23L);

    // Standard gravity: g = 9.80665 m/s²
    static_assert(si::constants::standard_gravity_value > 9.8L);
    static_assert(si::constants::standard_gravity_value < 9.9L);

    // Electron mass: me = 9.1093837015×10⁻³¹ kg
    static_assert(si::constants::electron_mass_value > 9.1e-31L);
    static_assert(si::constants::electron_mass_value < 9.2e-31L);

    // Proton mass: mp = 1.67262192369×10⁻²⁷ kg
    static_assert(si::constants::proton_mass_value > 1.67e-27L);
    static_assert(si::constants::proton_mass_value < 1.68e-27L);

    // Fine structure constant: α ≈ 1/137
    static_assert(si::constants::fine_structure_constant_value > 7.29e-3L);
    static_assert(si::constants::fine_structure_constant_value < 7.30e-3L);
    static_assert(si::constants::inverse_fine_structure_value > 137.0L);
    static_assert(si::constants::inverse_fine_structure_value < 138.0L);

    // Molar gas constant: R = 8.314... J/(mol·K)
    static_assert(si::constants::molar_gas_constant_value > 8.31L);
    static_assert(si::constants::molar_gas_constant_value < 8.32L);

  } // namespace si_constants_tests

  // =============================================================================
  // Natural Unit Tests
  // =============================================================================

  namespace natural_tests {

    // Test that natural units satisfy Unit concept
    static_assert(Unit<natural::electronvolt_t>);
    static_assert(Unit<natural::kiloelectronvolt_t>);
    static_assert(Unit<natural::megaelectronvolt_t>);
    static_assert(Unit<natural::gigaelectronvolt_t>);
    static_assert(Unit<natural::teraelectronvolt_t>);
    static_assert(Unit<natural::fermi_t>);
    static_assert(Unit<natural::barn_t>);
    static_assert(Unit<natural::millibarn_t>);
    static_assert(Unit<natural::microbarn_t>);
    static_assert(Unit<natural::nanobarn_t>);
    static_assert(Unit<natural::picobarn_t>);
    static_assert(Unit<natural::femtobarn_t>);

    // Mass and momentum units
    static_assert(Unit<natural::electronvolt_per_c2_t>);
    static_assert(Unit<natural::megaelectronvolt_per_c2_t>);
    static_assert(Unit<natural::gigaelectronvolt_per_c2_t>);
    static_assert(Unit<natural::electronvolt_per_c_t>);
    static_assert(Unit<natural::megaelectronvolt_per_c_t>);
    static_assert(Unit<natural::gigaelectronvolt_per_c_t>);

    // Test NamedUnit
    static_assert(NamedUnit<natural::electronvolt_t>);
    static_assert(NamedUnit<natural::fermi_t>);
    static_assert(NamedUnit<natural::barn_t>);
    static_assert(NamedUnit<natural::electronvolt_per_c2_t>);
    static_assert(NamedUnit<natural::electronvolt_per_c_t>);

    // Test ScaledUnit (energy units)
    static_assert(ScaledUnit<natural::kiloelectronvolt_t>);
    static_assert(ScaledUnit<natural::megaelectronvolt_t>);
    static_assert(ScaledUnit<natural::gigaelectronvolt_t>);
    static_assert(ScaledUnit<natural::teraelectronvolt_t>);

    // Barn units are NamedUnits (not scaled) to avoid integer overflow
    static_assert(NamedUnit<natural::millibarn_t>);
    static_assert(NamedUnit<natural::femtobarn_t>);

    // Verify symbols
    static_assert(natural::electronvolt_t::symbol == fixed_string { "eV" });
    static_assert(natural::kiloelectronvolt_t::symbol == fixed_string { "keV" });
    static_assert(natural::megaelectronvolt_t::symbol == fixed_string { "MeV" });
    static_assert(natural::gigaelectronvolt_t::symbol == fixed_string { "GeV" });
    static_assert(natural::teraelectronvolt_t::symbol == fixed_string { "TeV" });
    static_assert(natural::fermi_t::symbol == fixed_string { "fm" });
    static_assert(natural::barn_t::symbol == fixed_string { "b" });
    static_assert(natural::femtobarn_t::symbol == fixed_string { "fb" });
    static_assert(natural::electronvolt_per_c2_t::symbol == fixed_string { "eV/c2" });
    static_assert(natural::megaelectronvolt_per_c2_t::symbol == fixed_string { "MeV/c2" });
    static_assert(natural::gigaelectronvolt_per_c2_t::symbol == fixed_string { "GeV/c2" });

    // Verify energy unit magnitudes (relative to eV base)
    static_assert(natural::kiloelectronvolt_t::magnitude_type::num == 1000);
    static_assert(natural::kiloelectronvolt_t::magnitude_type::den == 1);
    static_assert(natural::megaelectronvolt_t::magnitude_type::num == 100'0000);
    static_assert(natural::megaelectronvolt_t::magnitude_type::den == 1);
    static_assert(natural::gigaelectronvolt_t::magnitude_type::num == 10'0000'0000);
    static_assert(natural::gigaelectronvolt_t::magnitude_type::den == 1);
    static_assert(natural::teraelectronvolt_t::magnitude_type::num == 1'0000'0000'0000);
    static_assert(natural::teraelectronvolt_t::magnitude_type::den == 1);

    // Verify fermi magnitude (10^-15 m)
    static_assert(natural::fermi_t::magnitude_type::num == 1);
    static_assert(natural::fermi_t::magnitude_type::den == 1000'0000'0000'0000);

    // Test conversion constants
    static_assert(natural::constants::eV_in_joules > 1.6e-19L);
    static_assert(natural::constants::eV_in_joules < 1.7e-19L);
    static_assert(natural::constants::GeV_in_joules > 1.6e-10L);
    static_assert(natural::constants::GeV_in_joules < 1.7e-10L);
    static_assert(natural::constants::c_natural == 1.0L);
    static_assert(natural::constants::hbar_natural == 1.0L);

    // Verify particle mass values (in MeV/c²)
    static_assert(natural::particle_masses::electron > 0.51L);
    static_assert(natural::particle_masses::electron < 0.52L);
    static_assert(natural::particle_masses::proton > 938.0L);
    static_assert(natural::particle_masses::proton < 939.0L);
    static_assert(natural::particle_masses::neutron > 939.0L);
    static_assert(natural::particle_masses::neutron < 940.0L);

    // Verify Planck scale values
    static_assert(natural::planck::length > 1.6e-35L);
    static_assert(natural::planck::length < 1.7e-35L);
    static_assert(natural::planck::mass_gev > 1.2e19L);
    static_assert(natural::planck::mass_gev < 1.3e19L);

    // -------------------------------------------------------------------------
    // Natural unit literals tests
    // -------------------------------------------------------------------------

    namespace literals_tests {
      using namespace natural::literals;

      // Energy literals
      constexpr auto energy_ev  = 1.0_eV;
      constexpr auto energy_kev = 1.0_keV;
      constexpr auto energy_mev = 1.0_MeV;
      constexpr auto energy_gev = 1.0_GeV;
      constexpr auto energy_tev = 1.0_TeV;

      static_assert(Quantity<decltype(energy_ev)>);
      static_assert(Quantity<decltype(energy_kev)>);
      static_assert(Quantity<decltype(energy_mev)>);
      static_assert(Quantity<decltype(energy_gev)>);
      static_assert(Quantity<decltype(energy_tev)>);

      static_assert(energy_ev.value() == 1.0);
      static_assert(energy_kev.value() == 1.0);
      static_assert(energy_mev.value() == 1.0);
      static_assert(energy_gev.value() == 1.0);
      static_assert(energy_tev.value() == 1.0);

      // Mass literals (eV/c²)
      constexpr auto mass_ev  = 1.0_eVc2;
      constexpr auto mass_mev = 1.0_MeVc2;
      constexpr auto mass_gev = 1.0_GeVc2;

      static_assert(Quantity<decltype(mass_ev)>);
      static_assert(Quantity<decltype(mass_mev)>);
      static_assert(Quantity<decltype(mass_gev)>);

      static_assert(mass_ev.value() == 1.0);
      static_assert(mass_mev.value() == 1.0);
      static_assert(mass_gev.value() == 1.0);

      // Momentum literals (eV/c)
      constexpr auto mom_ev  = 1.0_eVc;
      constexpr auto mom_mev = 1.0_MeVc;
      constexpr auto mom_gev = 1.0_GeVc;

      static_assert(Quantity<decltype(mom_ev)>);
      static_assert(Quantity<decltype(mom_mev)>);
      static_assert(Quantity<decltype(mom_gev)>);

      static_assert(mom_ev.value() == 1.0);
      static_assert(mom_mev.value() == 1.0);
      static_assert(mom_gev.value() == 1.0);

      // Length literal (fermi)
      constexpr auto length_fm = 1.0_fm;

      static_assert(Quantity<decltype(length_fm)>);
      static_assert(length_fm.value() == 1.0);

      // Cross-section literals
      constexpr auto xs_b  = 1.0_b;
      constexpr auto xs_mb = 1.0_mb;
      constexpr auto xs_ub = 1.0_ub;
      constexpr auto xs_nb = 1.0_nb;
      constexpr auto xs_pb = 1.0_pb;
      constexpr auto xs_fb = 1.0_fb;

      static_assert(Quantity<decltype(xs_b)>);
      static_assert(Quantity<decltype(xs_mb)>);
      static_assert(Quantity<decltype(xs_ub)>);
      static_assert(Quantity<decltype(xs_nb)>);
      static_assert(Quantity<decltype(xs_pb)>);
      static_assert(Quantity<decltype(xs_fb)>);

      static_assert(xs_b.value() == 1.0);
      static_assert(xs_mb.value() == 1.0);
      static_assert(xs_ub.value() == 1.0);
      static_assert(xs_nb.value() == 1.0);
      static_assert(xs_pb.value() == 1.0);
      static_assert(xs_fb.value() == 1.0);

      // Integer literal tests
      constexpr auto int_gev    = 125_GeV;
      constexpr auto int_mev_c2 = 938_MeVc2;
      constexpr auto int_fb     = 100_fb;

      static_assert(int_gev.value() == 125.0);
      static_assert(int_mev_c2.value() == 938.0);
      static_assert(int_fb.value() == 100.0);

    } // namespace literals_tests

  } // namespace natural_tests

} // namespace units::test

// Main function - tests pass if compilation succeeds
auto main() -> int {
  return 0;
}
