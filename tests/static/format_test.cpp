// SPDX-License-Identifier: MIT

/// @file format_test.cpp
/// @brief Static tests for units formatting functionality.
///
/// Note: Most formatting tests are runtime tests because std::string
/// and std::format are not fully constexpr. This file tests that the
/// formatting infrastructure compiles correctly.

#include <units/core/quantity.hpp>
#include <units/core/reference.hpp>
#include <units/format.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/literals.hpp>
#include <units/systems/si/prefixes.hpp>

#include <format>
#include <string>
#include <type_traits>

namespace units::test {

  using namespace si::literals;

  // =============================================================================
  // Compile-time type trait verification
  // =============================================================================

  // Verify that std::formatter is specialized for quantity types
  static_assert(std::is_default_constructible_v<
                std::formatter<quantity<reference<isq::length_t, si::metre_t>, double>>>);

  static_assert(std::is_default_constructible_v<
                std::formatter<quantity<reference<isq::mass_t, si::kilogram_t>, double>>>);

  static_assert(std::is_default_constructible_v<
                std::formatter<quantity<reference<isq::time_t, si::second_t>, double>>>);

  // =============================================================================
  // Runtime formatting tests
  // =============================================================================

  static inline auto test_basic_format() -> bool {
    auto q = 5.0_m;
    auto s = std::format("{}", q);
    // Should contain "5" and "m"
    return s.find('5') != std::string::npos && s.find('m') != std::string::npos;
  }

  static inline auto test_precision_format() -> bool {
    auto q = 5.123456_m;
    auto s = std::format("{:.2f}", q);
    // Should be "5.12 m"
    return s.find("5.12") != std::string::npos;
  }

  static inline auto test_no_unit_format() -> bool {
    auto q = 5.0_m;
    auto s = std::format("{:n}", q);
    // Should not contain "m" (unit symbol)
    return s.find('m') == std::string::npos && s.find('5') != std::string::npos;
  }

  static inline auto test_width_format() -> bool {
    auto q = 5.0_m;
    auto s = std::format("{:10}", q);
    // Should be padded
    return s.length() >= 10;
  }

  // =============================================================================
  // Unit symbol tests (runtime, since std::string is not constexpr)
  // =============================================================================

  static inline auto test_named_unit_symbols() -> bool {
    return unit_symbol<si::metre_t>() == "m" && unit_symbol<si::kilogram_t>() == "kg"
        && unit_symbol<si::second_t>() == "s" && unit_symbol<si::ampere_t>() == "A"
        && unit_symbol<si::kelvin_t>() == "K" && unit_symbol<si::mole_t>() == "mol"
        && unit_symbol<si::candela_t>() == "cd";
  }

  static inline auto test_prefixed_unit_symbols() -> bool {
    return unit_symbol<si::kilometre_t>() == "km" && unit_symbol<si::centimetre_t>() == "cm"
        && unit_symbol<si::millimetre_t>() == "mm" && unit_symbol<si::millisecond_t>() == "ms";
  }

  static inline auto test_derived_unit_symbols() -> bool {
    return unit_symbol<si::newton_t>() == "N" && unit_symbol<si::joule_t>() == "J"
        && unit_symbol<si::watt_t>() == "W" && unit_symbol<si::pascal_t>() == "Pa"
        && unit_symbol<si::hertz_t>() == "Hz" && unit_symbol<si::volt_t>() == "V"
        && unit_symbol<si::ohm_t>() == "Ohm";
  }

  // =============================================================================
  // to_string function tests
  // =============================================================================

  static inline auto test_to_string() -> bool {
    auto q = 5.0_m;
    auto s = to_string(q);
    return s.find('5') != std::string::npos && s.find('m') != std::string::npos;
  }

  static inline auto test_to_string_with_precision() -> bool {
    auto q = 5.123456_m;
    auto s = to_string(q, 2);
    return s.find("5.12") != std::string::npos;
  }

  static inline auto test_value_to_string() -> bool {
    auto q = 5.0_m;
    auto s = value_to_string(q);
    return s.find('5') != std::string::npos && s.find('m') == std::string::npos;
  }

  // =============================================================================
  // reference_symbol tests
  // =============================================================================

  static inline auto test_reference_symbols() -> bool {
    using length_ref = reference<isq::length_t, si::metre_t>;
    using mass_ref   = reference<isq::mass_t, si::kilogram_t>;
    using time_ref   = reference<isq::time_t, si::second_t>;

    return reference_symbol<length_ref>() == "m" && reference_symbol<mass_ref>() == "kg"
        && reference_symbol<time_ref>() == "s";
  }

} // namespace units::test

auto main() noexcept(false) -> int {
  // Run all tests
  bool all_passed = true;

  all_passed &= units::test::test_basic_format();
  all_passed &= units::test::test_precision_format();
  all_passed &= units::test::test_no_unit_format();
  all_passed &= units::test::test_width_format();
  all_passed &= units::test::test_named_unit_symbols();
  all_passed &= units::test::test_prefixed_unit_symbols();
  all_passed &= units::test::test_derived_unit_symbols();
  all_passed &= units::test::test_to_string();
  all_passed &= units::test::test_to_string_with_precision();
  all_passed &= units::test::test_value_to_string();
  all_passed &= units::test::test_reference_symbols();

  return all_passed ? 0 : 1;
}
