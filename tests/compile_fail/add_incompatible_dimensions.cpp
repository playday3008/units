// SPDX-License-Identifier: MIT

// Compile-fail test: Adding quantities with incompatible dimensions
// should not compile. You cannot add metres to seconds - they represent
// fundamentally different physical quantities.

#include <units/units.hpp>

auto main() -> int {
  using namespace units::si::literals;

  auto distance = 100.0_m;
  auto time     = 10.0_s;

  // This should FAIL to compile:
  // Adding length to time is dimensionally invalid
  [[maybe_unused]]
  auto invalid
      = distance + time; // ERROR: incompatible dimensions

  return 0;
}
