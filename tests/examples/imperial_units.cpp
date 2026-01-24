// SPDX-License-Identifier: MIT
// README example: Imperial Units

#include <units/units.hpp>

auto main() -> int {
  using namespace units::imperial;
  using namespace units;

  auto length = make_quantity(5.0, foot);          // 5 feet
  auto weight = make_quantity(150.0, pound);       // 150 pounds
  auto speed  = make_quantity(60.0, mile_per_hour);

  // Convert to SI
  auto length_m = length.in(units::si::metre); // ~1.524 m

  (void)weight;
  (void)speed;
  (void)length_m;
  return 0;
}
