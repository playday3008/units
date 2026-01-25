// SPDX-License-Identifier: MIT
// README example: Mathematical Functions

#include <units/math.hpp>
#include <units/units.hpp>

auto main() -> int {
  using namespace units::si::literals;

  auto distance     = -5.0_m;
  auto abs_distance = units::abs(distance); // 5 m

  auto         angle   = 1.5707963_rad;     // pi/2 radians
  const double sin_val = units::sin(angle); // ~1.0
  const double cos_val = units::cos(angle); // ~0.0

  (void)abs_distance;
  (void)sin_val;
  (void)cos_val;
  return 0;
}
