// SPDX-License-Identifier: MIT
// README Compiler Explorer Example 4: Temperature Conversions

#include <units/units.hpp>

#include <print>

auto main() -> int {
  using namespace units::si;

  // Create temperature points
  auto boiling_c = celsius_pt(100.0);        // 100 degrees Celsius
  auto boiling_k = to_kelvin(boiling_c);     // Convert to Kelvin
  auto boiling_f = to_fahrenheit(boiling_c); // Convert to Fahrenheit

  std::println("Boiling point of water:");
  std::println("  {} C = {} K = {} F", boiling_c.value(), boiling_k.value(), boiling_f.value());

  // Absolute zero
  auto abs_zero_k = kelvin_pt(0.0);
  auto abs_zero_c = to_celsius(abs_zero_k);
  auto abs_zero_f = to_fahrenheit(abs_zero_k);

  std::println("Absolute zero:");
  std::println("  {} K = {} C = {} F", abs_zero_k.value(), abs_zero_c.value(), abs_zero_f.value());
  return 0;
}
