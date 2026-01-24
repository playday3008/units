// SPDX-License-Identifier: MIT

/// @file main.cpp
/// @brief Comprehensive examples demonstrating the C++23 units library.
///
/// This file showcases the library's capabilities:
/// - Basic quantity creation with SI literals
/// - Unit conversions
/// - Dimensional analysis (derived quantities)
/// - Temperature scale conversions
/// - Mathematical functions
/// - Formatting with std::format
/// - Imperial/CGS units and cross-system conversions
/// - Physical constants
///
/// Build:
///   xmake run examples
///   cmake --build build && ./build/examples

#include <format>
#include <print>

#include <units/units.hpp>

// =============================================================================
// Example 1: Basic Quantities and SI Literals
// =============================================================================

static void basic_quantities() {
  std::println("=== Basic Quantities and SI Literals ===\n");

  using namespace units::si::literals;

  // Create quantities using user-defined literals
  auto distance = 100_m;
  auto time     = 9.58_s; // Usain Bolt's 100m world record
  auto mass     = 75.5_kg;

  std::println("Distance: {}", distance);
  std::println("Time: {}", time);
  std::println("Mass: {}", mass);

  // Prefixed units
  auto marathon      = 42.195_km;
  auto reaction_time = 150_ms;

  std::println("Marathon distance: {}", marathon);
  std::println("Reaction time: {}", reaction_time);
  std::println();
}

// =============================================================================
// Example 2: Unit Conversions
// =============================================================================

static void unit_conversions() {
  std::println("=== Unit Conversions ===\n");

  using namespace units::si::literals;
  using namespace units::si;

  // Convert between SI units
  auto dist_km = 5_km;
  auto dist_m  = dist_km.in(metre);
  auto dist_cm = dist_km.in(centimetre);

  std::println("5 km = {} = {}", dist_m, dist_cm);

  // Time conversions
  auto duration = 3600_s;
  std::println("3600 s = {} (in seconds)", duration);

  // Mass conversions
  auto mass_kg = 1_kg;
  auto mass_g  = mass_kg.in(gram);
  std::println("1 kg = {}", mass_g);

  // Mixed unit arithmetic - automatic conversion
  auto total = 1_km + 500_m; // Result in km (first operand's unit)
  std::println("1 km + 500 m = {}", total);
  std::println();
}

// =============================================================================
// Example 3: Dimensional Analysis - Derived Quantities
// =============================================================================

static void dimensional_analysis() {
  std::println("=== Dimensional Analysis ===\n");

  using namespace units::si::literals;
  using namespace units::si;

  // Speed = distance / time
  auto distance = 100_m;
  auto time     = 9.58_s;
  auto speed    = distance / time;
  std::println("100m sprint speed: {} m/s", speed.value());

  // Force = mass * acceleration (F = ma)
  auto mass         = 10_kg;
  auto acceleration = distance / (time * time);
  auto force        = mass * acceleration;
  std::println("Force: {} (calculated)", force.value());

  // Energy = force * distance (Work)
  auto work = force * distance;
  std::println("Work done: {} (calculated)", work.value());

  // Power = energy / time
  auto power = work / time;
  std::println("Power: {} (calculated)", power.value());

  // Area and volume
  auto length = 5_m;
  auto width  = 3_m;
  auto area   = length * width;
  std::println("Area (5m x 3m): {} m^2", area.value());

  std::println();
}

// =============================================================================
// Example 4: Temperature Conversions
// =============================================================================

static void temperature_conversions() {
  std::println("=== Temperature Conversions ===\n");

  using namespace units::si;

  // Create temperature points in different scales
  auto boiling_c   = celsius_pt(100.0);   // Water boiling point
  auto freezing_c  = celsius_pt(0.0);     // Water freezing point
  auto body_temp_f = fahrenheit_pt(98.6); // Normal body temperature

  // Convert between scales
  auto boiling_k = to_kelvin(boiling_c);
  auto boiling_f = to_fahrenheit(boiling_c);

  std::println("Water boiling point:");
  std::println("  {} degC", boiling_c.value());
  std::println("  {} K", boiling_k.value());
  std::println("  {} degF", boiling_f.value());

  auto freezing_k = to_kelvin(freezing_c);
  auto freezing_f = to_fahrenheit(freezing_c);

  std::println("\nWater freezing point:");
  std::println("  {} degC", freezing_c.value());
  std::println("  {} K", freezing_k.value());
  std::println("  {} degF", freezing_f.value());

  auto body_c = to_celsius(body_temp_f);
  auto body_k = to_kelvin(body_temp_f);

  std::println("\nNormal body temperature:");
  std::println("  {} degF", body_temp_f.value());
  std::println("  {} degC", body_c.value());
  std::println("  {} K", body_k.value());

  // Absolute zero
  auto abs_zero_k = kelvin_pt(0.0);
  auto abs_zero_c = to_celsius(abs_zero_k);
  auto abs_zero_f = to_fahrenheit(abs_zero_k);

  std::println("\nAbsolute zero:");
  std::println("  {} K", abs_zero_k.value());
  std::println("  {} degC", abs_zero_c.value());
  std::println("  {} degF", abs_zero_f.value());
  std::println();
}

// =============================================================================
// Example 5: Mathematical Functions
// =============================================================================

static void math_functions() {
  std::println("=== Mathematical Functions ===\n");

  using namespace units::si::literals;
  using namespace units;

  // Absolute value
  auto negative_dist = -5_m;
  auto positive_dist = abs(negative_dist);
  std::println("abs(-5 m) = {}", positive_dist);

  // Min/max
  auto a = 10_m;
  auto b = 25_m;
  std::println("min(10 m, 25 m) = {}", min(a, b));
  std::println("max(10 m, 25 m) = {}", max(a, b));

  // Clamp
  auto value = 150_m;
  auto lo    = 10_m;
  auto hi    = 100_m;
  std::println("clamp(150 m, 10 m, 100 m) = {}", clamp(value, lo, hi));

  // Floor, ceil, round
  auto fractional = make_quantity(3.7, si::metre);
  std::println("\nRounding 3.7 m:");
  std::println("  floor: {}", floor(fractional));
  std::println("  ceil: {}", ceil(fractional));
  std::println("  round: {}", round(fractional));

  // Trigonometry (using radians)
  auto angle = 1.5708_rad; // approximately pi/2
  std::println("\nTrigonometry (angle = pi/2 rad):");
  std::println("  sin(pi/2) = {}", sin(angle));
  std::println("  cos(pi/2) = {}", cos(angle));

  // Approximate equality check
  auto measured = 10.0000000001_m; // Very close to 10 m
  auto expected = 10_m;
  std::println("\napprox_equal(10.0000000001 m, 10 m): {}",
               approx_equal(measured, expected) ? "true" : "false");
  std::println();
}

// =============================================================================
// Example 6: Formatting Options
// =============================================================================

static void formatting_examples() {
  std::println("=== Formatting Options ===\n");

  using namespace units::si::literals;
  using namespace units;

  auto distance = 1234.5678_m;
  auto speed    = 100_m / 9.58_s;

  // Default formatting
  std::println("Default: {}", distance);

  // Without unit (using 'n' flag)
  std::println("Value only: {:n}", distance);

  // Using to_string helpers
  std::println("\nto_string: {}", to_string(speed));
  std::println("to_string with precision: {}", to_string(speed, 3));
  std::println("value_to_string: {}", value_to_string(speed));
  std::println();
}

// =============================================================================
// Example 7: Imperial Units
// =============================================================================

static void imperial_units() {
  std::println("=== Imperial Units ===\n");

  using namespace units;

  // Create imperial quantities
  auto dist_mi = make_quantity(26.2, imperial::mile);   // Marathon
  auto dist_ft = make_quantity(5280.0, imperial::foot); // 1 mile in feet
  auto mass_lb = make_quantity(150.0, imperial::pound); // Body weight

  std::println("Marathon: {}", dist_mi);
  std::println("5280 feet: {}", dist_ft);
  std::println("Body weight: {}", mass_lb);

  // Imperial to SI conversions
  auto dist_m  = dist_ft.in(si::metre);
  auto mass_kg = mass_lb.in(si::kilogram);

  std::println("\nConversions:");
  std::println("5280 ft = {}", dist_m);
  std::println("150 lb = {}", mass_kg);

  // Other imperial units
  auto height_in = make_quantity(72.0, imperial::inch); // 6 feet
  auto depth_ftm = make_quantity(10.0, imperial::fathom);

  std::println("\nHeight: {} = {} in metres", height_in, height_in.in(si::metre));
  std::println("Depth: {} = {} in metres", depth_ftm, depth_ftm.in(si::metre));
  std::println();
}

// =============================================================================
// Example 8: CGS Units
// =============================================================================

static void cgs_units() {
  std::println("=== CGS Units ===\n");

  using namespace units;

  // CGS mechanical units
  auto force_dyn  = make_quantity(1e5, cgs::dyne);  // 1 N = 10^5 dyn
  auto energy_erg = make_quantity(1e7, cgs::erg);   // 1 J = 10^7 erg
  auto viscosity  = make_quantity(1.0, cgs::poise); // Dynamic viscosity

  std::println("Force: {}", force_dyn);
  std::println("Energy: {}", energy_erg);
  std::println("Viscosity: {}", viscosity);

  // Convert to SI
  auto force_n  = force_dyn.in(si::newton);
  auto energy_j = energy_erg.in(si::joule);

  std::println("\nConversions:");
  std::println("10^5 dyn = {}", force_n);
  std::println("10^7 erg = {}", energy_j);

  // CGS electromagnetic units
  auto magnetic = make_quantity(1.0, cgs::gauss);
  std::println("\nMagnetic field: {}", magnetic);
  std::println();
}

// =============================================================================
// Example 9: Physical Constants
// =============================================================================

static void physical_constants() {
  std::println("=== Physical Constants ===\n");

  using namespace units::si;
  using namespace units::si::literals;

  // Fundamental constants
  std::println("Speed of light (c): {} m/s", speed_of_light.value());
  std::println("Planck constant (h): {} J*s", planck_constant.value());
  std::println("Elementary charge (e): {} C", elementary_charge.value());

  // Thermodynamic constants
  std::println("\nBoltzmann constant (k): {} J/K", boltzmann_constant.value());

  // Use constants in calculations
  // E = mc^2 example
  auto mass   = 1_kg;
  auto c      = speed_of_light;
  auto energy = mass * c * c;
  std::println("\nE = mc^2 for 1 kg:");
  std::println("Energy = {} (raw value)", energy.value());

  // Photon energy: E = hf
  auto frequency     = make_quantity(5e14, units::si::hertz); // Green light ~500nm
  auto photon_energy = planck_constant * frequency;
  std::println("\nPhoton energy at 500 THz (green light):");
  std::println("E = hf = {} (raw value)", photon_energy.value());
  std::println();
}

// =============================================================================
// Example 10: Natural Units (Particle Physics)
// =============================================================================

static void natural_units() {
  std::println("=== Natural Units (Particle Physics) ===\n");

  using namespace units;

  // Particle energies in electronvolts
  auto electron_energy = make_quantity(0.511, natural::MeV); // Electron rest mass
  auto proton_energy   = make_quantity(938.3, natural::MeV); // Proton rest mass

  std::println("Electron rest mass: {} MeV", electron_energy.value());
  std::println("Proton rest mass: {} MeV", proton_energy.value());

  // Cross-sections in barns
  auto cross_section = make_quantity(100.0, natural::millibarn);
  std::println("\nCross-section: {} mb", cross_section.value());

  // Length in fermi (femtometres)
  auto nuclear_radius = make_quantity(1.2, natural::fermi);
  std::println("Nuclear radius: {} fm", nuclear_radius.value());
  std::println();
}

// =============================================================================
// Example 11: Compile-Time Calculations
// =============================================================================

static void compile_time_examples() {
  std::println("=== Compile-Time Calculations ===\n");

  using namespace units::si::literals;

  // All these calculations happen at compile time
  constexpr auto distance = 100_m;
  constexpr auto time     = 10_s;
  constexpr auto speed    = distance / time;

  // Verify at compile time
  static_assert(speed.value() == 10.0);
  static_assert(distance.value() == 100.0);

  // Compile-time unit conversion
  constexpr auto km = 1_km;
  constexpr auto m  = km.in(units::si::metre);
  static_assert(m.value() == 1000.0);

  std::println("All calculations verified at compile time!");
  std::println("speed = {} m/s", speed.value());
  std::println("1 km = {} m", m.value());

  // Compile-time math
  constexpr auto negative = -5_m;
  constexpr auto absolute = units::abs(negative);
  static_assert(absolute.value() == 5.0);
  std::println("abs(-5 m) = {} m (compile-time)", absolute.value());
  std::println();
}

// =============================================================================
// Example 12: Real-World Application - Kinematics
// =============================================================================

static void kinematics_example() {
  std::println("=== Real-World Application: Kinematics ===\n");

  using namespace units::si::literals;
  using namespace units;

  // Problem: A car accelerates from rest to 100 km/h in 8 seconds.
  // Calculate: acceleration, distance traveled, and final kinetic energy.

  // Using raw values for demonstration (100 km/h = 27.778 m/s)
  constexpr double v0 = 0.0;    // initial speed (m/s)
  constexpr double v1 = 27.778; // final speed (m/s), 100 km/h
  constexpr double t  = 8.0;    // time (s)
  constexpr double m  = 1500.0; // mass (kg)

  std::println("Car acceleration problem:");
  std::println("  Initial speed: {} m/s", v0);
  std::println("  Final speed: {} m/s (100 km/h)", v1);
  std::println("  Time: {} s", t);
  std::println("  Mass: {} kg", m);

  // Calculate acceleration: a = (v - u) / t
  double accel = (v1 - v0) / t;
  std::println("\nResults:");
  std::println("  Acceleration: {} m/s^2", accel);

  // Calculate distance: s = ut + 0.5*a*t^2 (with u=0)
  double dist = 0.5 * accel * t * t;
  std::println("  Distance traveled: {} m", dist);

  // Calculate kinetic energy: KE = 0.5 * m * v^2
  double ke = 0.5 * m * v1 * v1;
  std::println("  Final kinetic energy: {} J", ke);

  // Now demonstrate dimensional analysis with quantities
  std::println("\n--- Dimensional analysis example ---");
  auto distance  = 100_m;
  auto time_qty  = 10_s;
  auto speed     = distance / time_qty;
  auto accel_qty = speed / time_qty;
  auto mass_qty  = 5_kg;
  auto force     = mass_qty * accel_qty;

  std::println("  distance / time = speed");
  std::println("    {} / {} = {} m/s", distance, time_qty, speed.value());
  std::println("  speed / time = acceleration");
  std::println("    {} m/s / {} = {} m/s^2", speed.value(), time_qty, accel_qty.value());
  std::println("  mass * acceleration = force");
  std::println("    {} * {} m/s^2 = {} N", mass_qty, accel_qty.value(), force.value());
  std::println();
}

// =============================================================================
// Main
// =============================================================================

auto main() -> int {
  std::println("C++23 Units Library - Comprehensive Examples\n");
  std::println("=============================================\n");

  basic_quantities();
  unit_conversions();
  dimensional_analysis();
  temperature_conversions();
  math_functions();
  formatting_examples();
  imperial_units();
  cgs_units();
  physical_constants();
  natural_units();
  compile_time_examples();
  kinematics_example();

  std::println("=============================================");
  std::println("All examples completed successfully!");

  return 0;
}
