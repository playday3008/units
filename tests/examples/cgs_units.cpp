// SPDX-License-Identifier: MIT
// README example: CGS Units

#include <units/units.hpp>

auto main() -> int {
  using namespace units::cgs;
  using namespace units;

  auto force_dyne = make_quantity(100.0, dyne);
  auto energy_erg = make_quantity(500.0, erg);

  // Convert to SI
  auto force_N = force_dyne.in(units::si::newton); // 0.001 N

  (void)energy_erg;
  (void)force_N;
  return 0;
}
