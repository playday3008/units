// SPDX-License-Identifier: MIT

// Compile-fail test: Adding two quantity_points should not compile.
// Adding two absolute positions makes no physical sense - you can only
// subtract them (to get displacement) or add a displacement to a point.

#include <units/systems/si/temperature.hpp>

auto main() -> int {
  using namespace units::si;

  // Create two temperature points
  auto temp1 = celsius_pt(20.0);
  auto temp2 = celsius_pt(25.0);

  // This should FAIL to compile:
  // Adding two points is meaningless (point + point = ???)
  [[maybe_unused]]
  auto invalid
      = temp1 + temp2; // ERROR: deleted function

  return 0;
}
