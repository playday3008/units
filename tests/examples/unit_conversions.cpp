// SPDX-License-Identifier: MIT
// README example: Unit Conversions

#include <units/units.hpp>

auto main() -> int {
  using namespace units::si::literals;

  auto distance_m  = 1500.0_m;
  auto distance_km = distance_m.in(units::si::kilometre); // 1.5 km

  // Get raw value in specific unit
  const double value_in_km = distance_m.numerical_value_in(units::si::kilometre);

  (void)distance_km;
  (void)value_in_km;
  return 0;
}
