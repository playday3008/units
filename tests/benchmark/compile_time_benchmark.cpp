// SPDX-License-Identifier: MIT

/// @file compile_time_benchmark.cpp
/// @brief Compile-time benchmark for the units library.
///
/// This file exercises many library features to measure compilation time.
/// Build with timing flags to measure compilation overhead:
///
///   time clang++ -std=c++23 -ftime-report -c compile_time_benchmark.cpp
///   time g++ -std=c++23 -ftime-report -c compile_time_benchmark.cpp
///
/// Or use xmake:
///   xmake build compile_time_benchmark

#include <numbers>

#include <units/units.hpp>

using namespace units;
using namespace units::si;
using namespace units::si::literals;
using namespace units::natural::literals;

// =============================================================================
// SI Base Unit Operations
// =============================================================================

namespace benchmark_si_base {

  // Length operations
  constexpr auto length_m  = 100.0_m;
  constexpr auto length_km = 1.5_km;
  constexpr auto length_cm = 50.0_cm;
  constexpr auto length_mm = 250.0_mm;

  constexpr auto length_sum  = length_m + length_km.in(metre);
  constexpr auto length_diff = length_km.in(metre)-length_m;

  static_assert(length_m.value() == 100.0);
  static_assert(length_km.value() == 1.5);
  static_assert(length_cm.value() == 50.0);
  static_assert(length_mm.value() == 250.0);
  static_assert(length_sum.value() == 1600.0);
  static_assert(length_diff.value() == 1400.0);

  // Mass operations
  constexpr auto mass_kg = 75.0_kg;
  constexpr auto mass_g  = 500.0_g;

  constexpr auto mass_sum = mass_kg + mass_g.in(kilogram);
  static_assert(mass_kg.value() == 75.0);
  static_assert(mass_sum.value() == 75.5);

  // Time operations
  constexpr auto time_s  = 60.0_s;
  constexpr auto time_ms = 500.0_ms;

  constexpr auto time_sum = time_s + time_ms.in(second);
  static_assert(time_s.value() == 60.0);
  static_assert(time_sum.value() == 60.5);

  // Electric current
  constexpr auto current = 2.5_A;
  static_assert(current.value() == 2.5);

  // Temperature
  constexpr auto temp = 300.0_K;
  static_assert(temp.value() == 300.0);

  // Amount of substance
  constexpr auto amount = 1.0_mol;
  static_assert(amount.value() == 1.0);

  // Luminous intensity
  constexpr auto luminous = 100.0_cd;
  static_assert(luminous.value() == 100.0);

} // namespace benchmark_si_base

// =============================================================================
// Derived Quantity Calculations
// =============================================================================

namespace benchmark_derived {

  // Speed calculation
  constexpr auto distance = 100.0_m;
  constexpr auto time     = 9.58_s;
  constexpr auto speed    = distance / time;

  static_assert(speed.value() > 10.0);
  static_assert(speed.value() < 11.0);

  // Force calculation: F = m * a
  constexpr auto mass         = 10.0_kg;
  constexpr auto unit_time    = 1.0_s;
  constexpr auto accel_dist   = make_quantity(9.81, metre);
  constexpr auto acceleration = accel_dist / (unit_time * unit_time);
  constexpr auto force        = mass * acceleration;

  static_assert(force.value() > 98.0);
  static_assert(force.value() < 99.0);

  // Energy calculation: E = F * d
  constexpr auto work_distance = 5.0_m;
  constexpr auto energy        = force * work_distance;

  static_assert(energy.value() > 490.0);
  static_assert(energy.value() < 500.0);

  // Power calculation: P = E / t
  constexpr auto power_time = 2.0_s;
  constexpr auto power      = energy / power_time;

  static_assert(power.value() > 245.0);
  static_assert(power.value() < 250.0);

  // Pressure calculation: P = F / A
  constexpr auto area_side = operator""_m(std::numbers::sqrt2_v<long double>); // sqrt(2)
  constexpr auto area      = area_side * area_side;
  constexpr auto pressure  = force / area;

  static_assert(pressure.value() > 49.0);
  static_assert(pressure.value() < 50.0);

  // Frequency
  constexpr auto period    = 0.001_s;
  constexpr auto frequency = make_quantity(1.0, si::hertz) / period * period;

  static_assert(frequency.value() == 1.0);

  // Named SI derived units
  constexpr auto force_n     = 100.0_N;
  constexpr auto energy_j    = 500.0_J;
  constexpr auto power_w     = 1000.0_W;
  constexpr auto pressure_pa = 101325.0_Pa;
  constexpr auto freq_hz     = 50.0_Hz;

  static_assert(force_n.value() == 100.0);
  static_assert(energy_j.value() == 500.0);
  static_assert(power_w.value() == 1000.0);
  static_assert(pressure_pa.value() == 101325.0);
  static_assert(freq_hz.value() == 50.0);

} // namespace benchmark_derived

// =============================================================================
// Temperature Conversions (quantity_point)
// =============================================================================

namespace benchmark_temperature {

  // Kelvin temperature points
  constexpr auto kelvin_0   = kelvin_pt(0.0);
  constexpr auto kelvin_273 = kelvin_pt(273.15);
  constexpr auto kelvin_373 = kelvin_pt(373.15);

  static_assert(kelvin_0.value() == 0.0);
  static_assert(kelvin_273.value() == 273.15);
  static_assert(kelvin_373.value() == 373.15);

  // Celsius temperature points
  constexpr auto celsius_0   = celsius_pt(0.0);
  constexpr auto celsius_100 = celsius_pt(100.0);
  constexpr auto celsius_m40 = celsius_pt(-40.0);

  static_assert(celsius_m40.value() == -40.0);

  // Fahrenheit temperature points
  constexpr auto fahrenheit_32  = fahrenheit_pt(32.0);
  constexpr auto fahrenheit_212 = fahrenheit_pt(212.0);
  constexpr auto fahrenheit_m40 = fahrenheit_pt(-40.0);

  static_assert(fahrenheit_212.value() == 212.0);
  static_assert(fahrenheit_m40.value() == -40.0);

  // Temperature differences
  constexpr auto temp_diff = celsius_100 - celsius_0;
  static_assert(temp_diff.value() == 100.0);

  // Convert Celsius to Kelvin
  constexpr auto celsius_0_to_k = to_kelvin(celsius_0);
  static_assert(celsius_0_to_k.value() == 273.15);

  // Convert Fahrenheit to Celsius (32°F == 0°C)
  constexpr auto freezing_f_to_c = to_celsius(fahrenheit_32);
  static_assert(freezing_f_to_c.value() == 0.0);

  // Convert Kelvin to Fahrenheit (0K == -459.67°F)
  constexpr auto abs_zero_f = to_fahrenheit(kelvin_0);
  static_assert(abs_zero_f.value() == -459.67);

} // namespace benchmark_temperature

// =============================================================================
// Cross-System Conversions
// =============================================================================

namespace benchmark_conversions {

  // Imperial units
  using namespace imperial;

  constexpr auto length_ft = make_quantity(100.0, foot);
  constexpr auto length_in = make_quantity(12.0, inch);
  constexpr auto length_mi = make_quantity(1.0, mile);
  constexpr auto length_yd = make_quantity(100.0, yard);

  // Convert imperial to SI
  constexpr auto ft_to_m = length_ft.in(metre);
  static_assert(ft_to_m.value() > 30.0);
  static_assert(ft_to_m.value() < 31.0);

  constexpr auto in_to_m = length_in.in(metre);
  static_assert(in_to_m.value() > 0.30);
  static_assert(in_to_m.value() < 0.31);

  constexpr auto mi_to_m = length_mi.in(metre);
  static_assert(mi_to_m.value() > 1609.0);
  static_assert(mi_to_m.value() < 1610.0);

  constexpr auto yd_to_m = length_yd.in(metre);
  static_assert(yd_to_m.value() > 91.0);
  static_assert(yd_to_m.value() < 92.0);

  // Mass conversions
  constexpr auto mass_lb = make_quantity(150.0, pound);
  constexpr auto mass_oz = make_quantity(16.0, ounce);

  constexpr auto lb_to_kg = mass_lb.in(kilogram);
  static_assert(lb_to_kg.value() > 68.0);
  static_assert(lb_to_kg.value() < 69.0);

  constexpr auto oz_to_kg = mass_oz.in(kilogram);
  static_assert(oz_to_kg.value() > 0.45);
  static_assert(oz_to_kg.value() < 0.46);

  // CGS units
  using namespace cgs;

  constexpr auto force_dyn  = make_quantity(1.0e5, dyne);
  constexpr auto energy_erg = make_quantity(1.0e7, erg);

  constexpr auto dyn_to_n = force_dyn.in(newton);
  static_assert(dyn_to_n.value() == 1.0);

  constexpr auto erg_to_j = energy_erg.in(joule);
  static_assert(erg_to_j.value() == 1.0);

} // namespace benchmark_conversions

// =============================================================================
// Natural Units
// =============================================================================

namespace benchmark_natural {

  // Energy in eV
  constexpr auto energy_ev  = 1.0_eV;
  constexpr auto energy_kev = 1.0_keV;
  constexpr auto energy_mev = 1.0_MeV;
  constexpr auto energy_gev = 1.0_GeV;
  constexpr auto energy_tev = 1.0_TeV;

  static_assert(energy_ev.value() == 1.0);
  static_assert(energy_kev.value() == 1.0);
  static_assert(energy_mev.value() == 1.0);
  static_assert(energy_gev.value() == 1.0);
  static_assert(energy_tev.value() == 1.0);

  // Mass in eV/c²
  constexpr auto mass_evc2  = 0.511_MeVc2; // Electron mass
  constexpr auto mass_gevc2 = 0.938_GeVc2; // Proton mass

  static_assert(mass_evc2.value() > 0.5);
  static_assert(mass_gevc2.value() > 0.9);

  // Momentum in eV/c
  constexpr auto momentum_mev = 100.0_MeVc;
  constexpr auto momentum_gev = 1.0_GeVc;

  static_assert(momentum_mev.value() == 100.0);
  static_assert(momentum_gev.value() == 1.0);

  // Cross-sections
  constexpr auto xs_b  = 1.0_b;
  constexpr auto xs_mb = 1.0_mb;
  constexpr auto xs_fb = 100.0_fb;

  static_assert(xs_b.value() == 1.0);
  static_assert(xs_mb.value() == 1.0);
  static_assert(xs_fb.value() == 100.0);

  // Length (fermi)
  constexpr auto length_fm = 1.0_fm;
  static_assert(length_fm.value() == 1.0);

} // namespace benchmark_natural

// =============================================================================
// Mathematical Functions
// =============================================================================

namespace benchmark_math {

  // Absolute value
  constexpr auto pos_val = 5.0_m;
  constexpr auto neg_val = make_quantity(-5.0, metre);

  constexpr auto abs_pos = abs(pos_val);
  constexpr auto abs_neg = abs(neg_val);

  static_assert(abs_pos.value() == 5.0);
  static_assert(abs_neg.value() == 5.0);

  // Min/Max
  constexpr auto val_a = 3.0_m;
  constexpr auto val_b = 7.0_m;

  constexpr auto min_val = min(val_a, val_b);
  constexpr auto max_val = max(val_a, val_b);

  static_assert(min_val.value() == 3.0);
  static_assert(max_val.value() == 7.0);

  // Rounding functions
  constexpr auto to_round = make_quantity(3.7, metre);

  constexpr auto floored = floor(to_round);
  constexpr auto ceiled  = ceil(to_round);
  constexpr auto rounded = round(to_round);
  constexpr auto trunced = trunc(to_round);

  static_assert(floored.value() == 3.0);
  static_assert(ceiled.value() == 4.0);
  static_assert(rounded.value() == 4.0);
  static_assert(trunced.value() == 3.0);

  // Powers - using simple values
  constexpr auto base    = 3.0_m;
  constexpr auto squared = base * base;
  constexpr auto cubed   = base * base * base;

  static_assert(squared.value() == 9.0);
  static_assert(cubed.value() == 27.0);

  // Trigonometric functions (return Rep, not quantity)
  constexpr auto angle = make_quantity(0.0, si::radian);
  constexpr auto sin_0 = sin(angle);
  constexpr auto cos_0 = cos(angle);

  static_assert(sin_0 == 0.0);
  static_assert(cos_0 == 1.0);

} // namespace benchmark_math

// =============================================================================
// SI Physical Constants
// =============================================================================

namespace benchmark_constants {

  // Speed of light
  constexpr auto c = si::speed_of_light;
  static_assert(c.value() == 2'9979'2458LL);

  // Planck constant
  constexpr auto h = si::planck_constant;
  static_assert(h.value() > 6.6e-34L);
  static_assert(h.value() < 6.7e-34L);

  // Elementary charge
  constexpr auto e = si::elementary_charge;
  static_assert(e.value() > 1.6e-19L);
  static_assert(e.value() < 1.7e-19L);

  // Boltzmann constant
  constexpr auto k = si::boltzmann_constant;
  static_assert(k.value() > 1.38e-23L);
  static_assert(k.value() < 1.39e-23L);

  // Avogadro constant
  constexpr auto na = si::avogadro_constant;
  static_assert(na.value() > 6.0e23L);
  static_assert(na.value() < 6.1e23L);

  // Standard gravity
  constexpr auto g = si::standard_gravity;
  static_assert(g.value() > 9.8L);
  static_assert(g.value() < 9.9L);

  // Standard atmosphere
  constexpr auto atm = si::standard_atmosphere;
  static_assert(atm.value() == 10'1325LL);

} // namespace benchmark_constants

// =============================================================================
// ISQ Quantity Specs
// =============================================================================

namespace benchmark_isq {

  // Base quantities
  static_assert(isq::length_t::character == quantity_character::scalar);
  static_assert(isq::mass_t::character == quantity_character::scalar);
  static_assert(isq::time_t::character == quantity_character::scalar);

  // Mechanics quantities
  static_assert(isq::velocity_t::character == quantity_character::vector);
  static_assert(isq::force_t::character == quantity_character::vector);
  static_assert(isq::energy_t::character == quantity_character::scalar);
  static_assert(isq::power_t::character == quantity_character::scalar);

  // Hierarchy checks
  static_assert(is_child_of_v<isq::work_t, isq::energy_t>);
  static_assert(is_child_of_v<isq::kinetic_energy_t, isq::energy_t>);
  static_assert(!is_child_of_v<isq::torque_t, isq::energy_t>);

  // Thermodynamics quantities
  static_assert(isq::heat_capacity_t::character == quantity_character::scalar);
  static_assert(isq::entropy_t::character == quantity_character::scalar);
  static_assert(is_child_of_v<isq::enthalpy_t, isq::energy_t>);

  // Electromagnetism quantities
  static_assert(isq::electric_charge_t::character == quantity_character::scalar);
  static_assert(isq::voltage_t::character == quantity_character::scalar);
  static_assert(isq::magnetic_flux_density_t::character == quantity_character::vector);

} // namespace benchmark_isq

// =============================================================================
// Formatting (compile-time string generation)
// =============================================================================

namespace benchmark_format {

  // Unit symbols
  static_assert(si::metre_t::symbol == fixed_string { "m" });
  static_assert(si::kilogram_t::symbol == fixed_string { "kg" });
  static_assert(si::second_t::symbol == fixed_string { "s" });
  static_assert(si::newton_t::symbol == fixed_string { "N" });
  static_assert(si::joule_t::symbol == fixed_string { "J" });
  static_assert(si::watt_t::symbol == fixed_string { "W" });

  // Prefixed unit symbols
  static_assert(si::kilometre_t::symbol == fixed_string { "km" });
  static_assert(si::centimetre_t::symbol == fixed_string { "cm" });
  static_assert(si::millimetre_t::symbol == fixed_string { "mm" });
  static_assert(si::millisecond_t::symbol == fixed_string { "ms" });

  // Natural unit symbols
  static_assert(natural::electronvolt_t::symbol == fixed_string { "eV" });
  static_assert(natural::megaelectronvolt_t::symbol == fixed_string { "MeV" });
  static_assert(natural::gigaelectronvolt_t::symbol == fixed_string { "GeV" });
  static_assert(natural::fermi_t::symbol == fixed_string { "fm" });
  static_assert(natural::femtobarn_t::symbol == fixed_string { "fb" });

} // namespace benchmark_format

// Main function - benchmark passes if compilation succeeds
auto main() -> int {
  // Runtime verification that compile-time values are accessible
  volatile auto v1 = benchmark_si_base::length_m.value();
  volatile auto v2 = benchmark_derived::speed.value();
  volatile auto v3 = benchmark_natural::energy_gev.value();
  volatile auto v4 = benchmark_math::abs_pos.value();
  volatile auto v5 = benchmark_constants::c.value();

  (void)v1;
  (void)v2;
  (void)v3;
  (void)v4;
  (void)v5;

  return 0;
}
