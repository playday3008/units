// SPDX-License-Identifier: MIT

// Compile-fail test: Torque should NOT implicitly convert to energy
// Even though they have the same dimension (M*L^2*T^-2), they are different
// physical quantities and should not be interchangeable.
//
// This test MUST fail to compile. If it compiles successfully, the type
// safety check is broken.

#include <units/systems/isq/mechanics.hpp>

// Function that requires an energy type
template<typename T>
  requires units::implicitly_convertible_v<T, units::isq::energy_t>
static void requires_energy(T /*qty*/) {}

auto main() -> int {
  // This should fail: torque is not implicitly convertible to energy
  requires_energy(units::isq::torque_t {}); // MUST NOT COMPILE

  return 0;
}
