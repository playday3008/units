// SPDX-License-Identifier: MIT

#pragma once

/// @file units.hpp
/// @brief Convenience header that includes all commonly needed units library components.
///
/// This header provides a one-stop include for the units library. It includes:
/// - Core components (quantity, quantity_point, reference, unit)
/// - SI system (base units, derived units, prefixes, temperatures, constants)
/// - ISQ system (quantity specifications)
/// - Formatting support (std::formatter specialization)
/// - Mathematical functions (abs, sqrt, pow, trig functions)
///
/// Usage:
/// @code
/// #include <units/units.hpp>
///
/// int main() {
///     using namespace units::si::literals;
///
///     auto distance = 100_m;
///     auto time = 9.58_s;
///     auto speed = distance / time;  // ~10.44 m/s
///
///     std::cout << std::format("Speed: {:.2f}", speed) << std::endl;
/// }
/// @endcode

// =============================================================================
// Core components
// =============================================================================

#include <units/core/quantity.hpp>
#include <units/core/quantity_point.hpp>
#include <units/core/quantity_spec.hpp>
#include <units/core/ratio.hpp>
#include <units/core/reference.hpp>
#include <units/core/unit.hpp>

// =============================================================================
// ISQ quantity specifications
// =============================================================================

#include <units/systems/isq/base.hpp>
#include <units/systems/isq/electromagnetism.hpp>
#include <units/systems/isq/mechanics.hpp>
#include <units/systems/isq/thermodynamics.hpp>

// =============================================================================
// SI system
// =============================================================================

#include <units/systems/si/base.hpp>
#include <units/systems/si/constants.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/literals.hpp>
#include <units/systems/si/prefixes.hpp>
#include <units/systems/si/temperature.hpp>

// =============================================================================
// Additional unit systems
// =============================================================================

#include <units/systems/cgs.hpp>
#include <units/systems/imperial.hpp>
#include <units/systems/natural.hpp>

// =============================================================================
// Formatting and mathematical functions
// =============================================================================

#include <units/format.hpp>
#include <units/math.hpp>

// =============================================================================
// Optional: Bring SI literals into a convenient namespace
// =============================================================================

/// Inline namespace for convenient access to SI literals.
/// Usage:
/// @code
/// using namespace units::literals;
/// auto d = 5_m;
/// @endcode
namespace units::inline literals {
  using namespace si::literals;
  using namespace natural::literals;
} // namespace units::inline literals
