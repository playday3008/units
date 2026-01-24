// SPDX-License-Identifier: MIT

// Compile-fail test: Implicit narrowing conversions should not compile.
// Converting from a higher-precision representation (double) to a
// lower-precision representation (float) requires explicit conversion.

#include <units/units.hpp>

auto main() -> int {
  using namespace units::si;
  using namespace units::si::literals;

  // Create a quantity with double precision
  auto distance_double = 100.0_m; // quantity<..., double>

  // This should FAIL to compile:
  // Implicit narrowing from double to float is not allowed
  [[maybe_unused]]
  units::quantity<units::reference<units::isq::length_t, metre_t>, float> distance_float
      = distance_double; // ERROR: narrowing conversion

  return 0;
}
