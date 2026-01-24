// SPDX-License-Identifier: MIT
// README example: Quick Start

#include <units/units.hpp>

#include <format>
#include <iostream>

auto main() -> int {
  using namespace units::si::literals;

  // Basic quantities
  auto distance = 100_m;
  auto time     = 9.58_s;
  auto speed    = distance / time; // ~10.44 m/s

  // Unit conversions
  auto distance_km = distance.in(units::si::kilometre); // 0.1 km

  // Formatting
  std::cout << std::format("Speed: {:.2f}\n", speed); // "Speed: 10.44 m/s"

  // Type safety - these would fail to compile:
  // auto error = distance + time;     // Can't add metres to seconds
  // auto wrong = speed + distance;    // Can't add m/s to m

  // Temperature with proper affine handling
  auto boiling_point = units::si::celsius_pt(100.0); // 100 degrees Celsius

  (void)distance_km;
  (void)boiling_point;
  return 0;
}
