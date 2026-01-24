// SPDX-License-Identifier: MIT
// README Compiler Explorer Example 7: Mathematical Functions (Extended)

#include <units/math.hpp>
#include <units/units.hpp>

#include <numbers>
#include <print>

auto main() -> int {
  using namespace units::si::literals;
  using namespace units;

  // Absolute value
  auto negative = make_quantity(-5.0, si::metre);
  auto positive = abs(negative);
  std::println("abs(-5 m) = {} m", positive.value());

  // Min/max
  auto a = 3.0_m;
  auto b = 7.0_m;
  std::println("min({}, {}) = {} m", a.value(), b.value(), min(a, b).value());
  std::println("max({}, {}) = {} m", a.value(), b.value(), max(a, b).value());

  // Rounding
  auto x = make_quantity(3.7, si::metre);
  std::println("floor(3.7 m) = {} m", floor(x).value());
  std::println("ceil(3.7 m) = {} m", ceil(x).value());
  std::println("round(3.7 m) = {} m", round(x).value());

  // Trigonometry (returns raw values for angles)
  auto angle = make_quantity(std::numbers::pi / 4.0, si::radian);
  std::println("sin(pi/4) = {:.4f}", sin(angle));
  std::println("cos(pi/4) = {:.4f}", cos(angle));

  return 0;
}
