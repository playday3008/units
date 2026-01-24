// SPDX-License-Identifier: MIT
// README example: Derived Quantities

#include <units/units.hpp>

auto main() -> int {
  using namespace units::si::literals;

  auto distance = 100.0_m;
  auto time     = 10.0_s;
  auto velocity = distance / time; // 10 m/s

  auto mass  = 5.0_kg;
  auto force = mass * (velocity / time); // 5 N (acceleration = 1 m/s^2)

  (void)force;
  return 0;
}
