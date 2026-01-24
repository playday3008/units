// SPDX-License-Identifier: MIT
// README example: Formatting

#include <units/format.hpp>
#include <units/units.hpp>

#include <format>

auto main() -> int {
  using namespace units::si::literals;

  auto speed = 25.5_m / 1.0_s;

  auto s1 = std::format("{}", speed);     // "25.5 m/s"
  auto s2 = std::format("{:.2f}", speed); // "25.50 m/s"
  auto s3 = std::format("{:n}", speed);   // "25.5" (no unit)

  (void)s1;
  (void)s2;
  (void)s3;
  return 0;
}
