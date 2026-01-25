// SPDX-License-Identifier: MIT
// README Compiler Explorer Example 5: Zero Overhead Verification

#include <units/units.hpp>

using namespace units::si::literals;
using namespace units::si;

// Compile with -O2 and inspect assembly
static auto compute_speed_raw(double distance, double time) -> double {
  return distance / time;
}

static auto compute_speed_units(double distance, double time) {
  auto d = make_quantity(distance, metre);
  auto t = make_quantity(time, second);
  return (d / t).value();
}

// Both functions generate identical assembly!
auto main() -> int {
  const volatile double d = 100.0;
  const volatile double t = 9.58;

  auto raw  = compute_speed_raw(d, t);
  auto safe = compute_speed_units(d, t);

  return (raw == safe) ? 0 : 1;
}
