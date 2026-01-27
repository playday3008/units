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

#include <units/core/quantity.hpp>       // IWYU pragma: export
#include <units/core/quantity_point.hpp> // IWYU pragma: export
#include <units/core/quantity_spec.hpp>  // IWYU pragma: export
#include <units/core/ratio.hpp>          // IWYU pragma: export
#include <units/core/reference.hpp>      // IWYU pragma: export
#include <units/core/unit.hpp>           // IWYU pragma: export

// =============================================================================
// ISQ quantity specifications
// =============================================================================

#include <units/systems/isq/base.hpp>             // IWYU pragma: export
#include <units/systems/isq/electromagnetism.hpp> // IWYU pragma: export
#include <units/systems/isq/mechanics.hpp>        // IWYU pragma: export
#include <units/systems/isq/thermodynamics.hpp>   // IWYU pragma: export

// =============================================================================
// SI system
// =============================================================================

#include <units/systems/si/base.hpp>        // IWYU pragma: export
#include <units/systems/si/constants.hpp>   // IWYU pragma: export
#include <units/systems/si/derived.hpp>     // IWYU pragma: export
#include <units/systems/si/literals.hpp>    // IWYU pragma: export
#include <units/systems/si/prefixes.hpp>    // IWYU pragma: export
#include <units/systems/si/temperature.hpp> // IWYU pragma: export

// =============================================================================
// Additional unit systems
// =============================================================================

#include <units/systems/cgs.hpp>      // IWYU pragma: export
#include <units/systems/imperial.hpp> // IWYU pragma: export
#include <units/systems/natural.hpp>  // IWYU pragma: export

// =============================================================================
// Formatting and mathematical functions
// =============================================================================

#include <units/format.hpp> // IWYU pragma: export
#include <units/math.hpp>   // IWYU pragma: export

// =============================================================================
// Optional: Bring SI literals into a convenient namespace
// =============================================================================

/// Inline namespace for convenient access to all unit system literals.
/// Usage:
/// @code
/// using namespace units::literals;
/// auto d = 5_m;      // SI
/// auto e = 100_erg;  // CGS
/// auto l = 10_ft;    // Imperial
/// @endcode
namespace units::inline literals {
  using namespace si::literals;
  using namespace natural::literals;
  using namespace cgs::literals;
  using namespace imperial::literals;
} // namespace units::inline literals
