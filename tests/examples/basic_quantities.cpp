// SPDX-License-Identifier: MIT
// README example: Basic Quantities

#include <units/units.hpp>

auto main() -> int {
  using namespace units::si::literals;

  auto length = 5.0_m;   // 5 metres
  auto mass   = 2.5_kg;  // 2.5 kilograms
  auto time   = 10.0_s;  // 10 seconds
  auto force  = 100.0_N; // 100 newtons
  auto energy = 500.0_J; // 500 joules

  (void)length;
  (void)mass;
  (void)time;
  (void)force;
  (void)energy;
  return 0;
}
