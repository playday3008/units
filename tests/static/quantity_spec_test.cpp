// SPDX-License-Identifier: MIT

// Static assertion tests for quantity specifications
// All tests execute at compile time via static_assert

#include <units/core/quantity_spec.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/isq/mechanics.hpp>

namespace units::test {

  // =============================================================================
  // Quantity spec concept tests
  // =============================================================================

  // Base quantities satisfy QuantitySpec concept
  static_assert(QuantitySpec<isq::length_t>);
  static_assert(QuantitySpec<isq::mass_t>);
  static_assert(QuantitySpec<isq::time_t>);
  static_assert(QuantitySpec<isq::electric_current_t>);
  static_assert(QuantitySpec<isq::thermodynamic_temperature_t>);
  static_assert(QuantitySpec<isq::amount_of_substance_t>);
  static_assert(QuantitySpec<isq::luminous_intensity_t>);

  // Derived quantities satisfy QuantitySpec concept
  static_assert(QuantitySpec<isq::speed_t>);
  static_assert(QuantitySpec<isq::velocity_t>);
  static_assert(QuantitySpec<isq::acceleration_t>);
  static_assert(QuantitySpec<isq::force_t>);
  static_assert(QuantitySpec<isq::energy_t>);
  static_assert(QuantitySpec<isq::torque_t>);

  // Subtypes satisfy QuantitySpec concept
  static_assert(QuantitySpec<isq::width_t>);
  static_assert(QuantitySpec<isq::height_t>);
  static_assert(QuantitySpec<isq::radius_t>);
  static_assert(QuantitySpec<isq::work_t>);
  static_assert(QuantitySpec<isq::kinetic_energy_t>);

  // =============================================================================
  // Quantity character tests
  // =============================================================================

  // Scalar quantities
  static_assert(ScalarQuantitySpec<isq::length_t>);
  static_assert(ScalarQuantitySpec<isq::mass_t>);
  static_assert(ScalarQuantitySpec<isq::time_t>);
  static_assert(ScalarQuantitySpec<isq::energy_t>);
  static_assert(ScalarQuantitySpec<isq::speed_t>);
  static_assert(ScalarQuantitySpec<isq::torque_t>);

  // Vector quantities
  static_assert(VectorQuantitySpec<isq::velocity_t>);
  static_assert(VectorQuantitySpec<isq::acceleration_t>);
  static_assert(VectorQuantitySpec<isq::force_t>);
  static_assert(VectorQuantitySpec<isq::momentum_t>);
  static_assert(VectorQuantitySpec<isq::position_vector_t>);
  static_assert(VectorQuantitySpec<isq::displacement_t>);

  // Not the wrong character
  static_assert(!VectorQuantitySpec<isq::length_t>);
  static_assert(!ScalarQuantitySpec<isq::velocity_t>);

  // =============================================================================
  // Hierarchy (is_child_of_v) tests
  // =============================================================================

  // Self is always child of self
  static_assert(is_child_of_v<isq::length_t, isq::length_t>);
  static_assert(is_child_of_v<isq::width_t, isq::width_t>);

  // Direct children
  static_assert(is_child_of_v<isq::width_t, isq::length_t>);
  static_assert(is_child_of_v<isq::height_t, isq::length_t>);
  static_assert(is_child_of_v<isq::radius_t, isq::length_t>);
  static_assert(is_child_of_v<isq::diameter_t, isq::length_t>);
  static_assert(is_child_of_v<isq::depth_t, isq::length_t>);
  static_assert(is_child_of_v<isq::thickness_t, isq::length_t>);
  static_assert(is_child_of_v<isq::distance_t, isq::length_t>);
  static_assert(is_child_of_v<isq::wavelength_t, isq::length_t>);

  // Grandchildren (transitive)
  static_assert(is_child_of_v<isq::period_t, isq::duration_t>);
  static_assert(is_child_of_v<isq::period_t, isq::time_t>);

  // Energy hierarchy
  static_assert(is_child_of_v<isq::work_t, isq::energy_t>);
  static_assert(is_child_of_v<isq::kinetic_energy_t, isq::energy_t>);
  static_assert(is_child_of_v<isq::potential_energy_t, isq::energy_t>);
  static_assert(is_child_of_v<isq::mechanical_energy_t, isq::energy_t>);
  static_assert(is_child_of_v<isq::heat_t, isq::energy_t>);

  // Force hierarchy
  static_assert(is_child_of_v<isq::weight_t, isq::force_t>);

  // Momentum hierarchy
  static_assert(is_child_of_v<isq::impulse_t, isq::momentum_t>);

  // Pressure/stress hierarchy
  static_assert(is_child_of_v<isq::stress_t, isq::pressure_t>);

  // Negative tests - siblings are not children of each other
  static_assert(!is_child_of_v<isq::width_t, isq::height_t>);
  static_assert(!is_child_of_v<isq::height_t, isq::width_t>);
  static_assert(!is_child_of_v<isq::work_t, isq::kinetic_energy_t>);

  // Parent is not child of child
  static_assert(!is_child_of_v<isq::length_t, isq::width_t>);
  static_assert(!is_child_of_v<isq::energy_t, isq::work_t>);

  // Unrelated types
  static_assert(!is_child_of_v<isq::length_t, isq::mass_t>);
  static_assert(!is_child_of_v<isq::energy_t, isq::torque_t>);
  static_assert(!is_child_of_v<isq::torque_t, isq::energy_t>);
  static_assert(!is_child_of_v<isq::speed_t, isq::velocity_t>);

  // =============================================================================
  // QuantitySpecOf concept tests
  // =============================================================================

  // QuantitySpecOf checks both QuantitySpec and is_child_of
  static_assert(QuantitySpecOf<isq::width_t, isq::length_t>);
  static_assert(QuantitySpecOf<isq::work_t, isq::energy_t>);
  static_assert(QuantitySpecOf<isq::length_t, isq::length_t>);

  static_assert(!QuantitySpecOf<isq::length_t, isq::width_t>);
  static_assert(!QuantitySpecOf<isq::torque_t, isq::energy_t>);

  // =============================================================================
  // Implicit conversion tests
  // =============================================================================

  // Same type - always implicitly convertible
  static_assert(implicitly_convertible_v<isq::length_t, isq::length_t>);
  static_assert(implicitly_convertible_v<isq::energy_t, isq::energy_t>);

  // Child to parent - implicitly convertible (upcasting)
  static_assert(implicitly_convertible_v<isq::width_t, isq::length_t>);
  static_assert(implicitly_convertible_v<isq::height_t, isq::length_t>);
  static_assert(implicitly_convertible_v<isq::work_t, isq::energy_t>);
  static_assert(implicitly_convertible_v<isq::kinetic_energy_t, isq::energy_t>);
  static_assert(implicitly_convertible_v<isq::weight_t, isq::force_t>);

  // Parent to child - NOT implicitly convertible (requires explicit cast)
  static_assert(!implicitly_convertible_v<isq::length_t, isq::width_t>);
  static_assert(!implicitly_convertible_v<isq::energy_t, isq::work_t>);
  static_assert(!implicitly_convertible_v<isq::force_t, isq::weight_t>);

  // Siblings - NOT implicitly convertible
  static_assert(!implicitly_convertible_v<isq::width_t, isq::height_t>);
  static_assert(!implicitly_convertible_v<isq::height_t, isq::width_t>);
  static_assert(!implicitly_convertible_v<isq::work_t, isq::kinetic_energy_t>);
  static_assert(!implicitly_convertible_v<isq::kinetic_energy_t, isq::work_t>);

  // Unrelated - NOT implicitly convertible
  static_assert(!implicitly_convertible_v<isq::length_t, isq::mass_t>);
  static_assert(!implicitly_convertible_v<isq::energy_t, isq::torque_t>);
  static_assert(!implicitly_convertible_v<isq::torque_t, isq::energy_t>);

  // =============================================================================
  // Explicit conversion tests
  // =============================================================================

  // Same type - always explicitly convertible
  static_assert(explicitly_convertible_v<isq::length_t, isq::length_t>);
  static_assert(explicitly_convertible_v<isq::energy_t, isq::energy_t>);

  // Child to parent - explicitly convertible
  static_assert(explicitly_convertible_v<isq::width_t, isq::length_t>);
  static_assert(explicitly_convertible_v<isq::work_t, isq::energy_t>);

  // Parent to child - explicitly convertible (downcasting)
  static_assert(explicitly_convertible_v<isq::length_t, isq::width_t>);
  static_assert(explicitly_convertible_v<isq::energy_t, isq::work_t>);

  // Siblings - NOT explicitly convertible (no hierarchy relationship)
  static_assert(!explicitly_convertible_v<isq::width_t, isq::height_t>);
  static_assert(!explicitly_convertible_v<isq::work_t, isq::kinetic_energy_t>);

  // Unrelated - NOT explicitly convertible
  static_assert(!explicitly_convertible_v<isq::length_t, isq::mass_t>);
  static_assert(!explicitly_convertible_v<isq::energy_t, isq::torque_t>);
  static_assert(!explicitly_convertible_v<isq::torque_t, isq::energy_t>);

  // =============================================================================
  // common_quantity_spec tests
  // =============================================================================

  // Same type - common is the type itself
  static_assert(std::same_as<common_quantity_spec<isq::length_t, isq::length_t>, isq::length_t>);
  static_assert(std::same_as<common_quantity_spec<isq::width_t, isq::width_t>, isq::width_t>);
  static_assert(std::same_as<common_quantity_spec<isq::energy_t, isq::energy_t>, isq::energy_t>);

  // Child and parent - common is parent
  static_assert(std::same_as<common_quantity_spec<isq::width_t, isq::length_t>, isq::length_t>);
  static_assert(std::same_as<common_quantity_spec<isq::length_t, isq::width_t>, isq::length_t>);
  static_assert(std::same_as<common_quantity_spec<isq::work_t, isq::energy_t>, isq::energy_t>);

  // Siblings - common is their shared parent
  static_assert(std::same_as<common_quantity_spec<isq::width_t, isq::height_t>, isq::length_t>);
  static_assert(std::same_as<common_quantity_spec<isq::height_t, isq::width_t>, isq::length_t>);
  static_assert(std::same_as<common_quantity_spec<isq::radius_t, isq::diameter_t>, isq::length_t>);
  static_assert(
      std::same_as<common_quantity_spec<isq::work_t, isq::kinetic_energy_t>, isq::energy_t>);
  static_assert(std::same_as<common_quantity_spec<isq::kinetic_energy_t, isq::potential_energy_t>,
                             isq::energy_t>);

  // Grandchild and parent
  static_assert(std::same_as<common_quantity_spec<isq::period_t, isq::time_t>, isq::time_t>);
  static_assert(
      std::same_as<common_quantity_spec<isq::period_t, isq::duration_t>, isq::duration_t>);

  // =============================================================================
  // Root quantity spec tests
  // =============================================================================

  // Base quantities are roots
  static_assert(is_root_quantity_spec_v<isq::length_t>);
  static_assert(is_root_quantity_spec_v<isq::mass_t>);
  static_assert(is_root_quantity_spec_v<isq::time_t>);
  static_assert(is_root_quantity_spec_v<isq::energy_t>);
  static_assert(is_root_quantity_spec_v<isq::torque_t>);
  static_assert(is_root_quantity_spec_v<isq::speed_t>);

  // Derived quantities (children) are not roots
  static_assert(!is_root_quantity_spec_v<isq::width_t>);
  static_assert(!is_root_quantity_spec_v<isq::height_t>);
  static_assert(!is_root_quantity_spec_v<isq::work_t>);
  static_assert(!is_root_quantity_spec_v<isq::kinetic_energy_t>);
  static_assert(!is_root_quantity_spec_v<isq::period_t>);
  static_assert(!is_root_quantity_spec_v<isq::duration_t>);

  // =============================================================================
  // Derived quantity spec (expression) tests
  // =============================================================================

  // Test that derived_quantity_spec satisfies QuantitySpec
  using speed_expr = decltype(isq::length / isq::time);
  static_assert(QuantitySpec<speed_expr>);
  static_assert(DerivedQuantitySpec<speed_expr>);

  using accel_expr = decltype(isq::length / isq::time / isq::time);
  static_assert(QuantitySpec<accel_expr>);
  static_assert(DerivedQuantitySpec<accel_expr>);

  using force_expr = decltype(isq::mass * isq::length / isq::time / isq::time);
  static_assert(QuantitySpec<force_expr>);
  static_assert(DerivedQuantitySpec<force_expr>);

  using energy_expr = decltype(isq::mass * isq::length * isq::length / isq::time / isq::time);
  static_assert(QuantitySpec<energy_expr>);
  static_assert(DerivedQuantitySpec<energy_expr>);

  // Named specs are not derived (they're named)
  static_assert(!DerivedQuantitySpec<isq::length_t>);
  static_assert(!DerivedQuantitySpec<isq::energy_t>);
  static_assert(!DerivedQuantitySpec<isq::speed_t>);

  // =============================================================================
  // kind_of wrapper tests
  // =============================================================================

  using length_kind = kind_of<isq::length_t>;
  using energy_kind = kind_of<isq::energy_t>;

  static_assert(is_kind_of_v<length_kind>);
  static_assert(is_kind_of_v<energy_kind>);
  static_assert(!is_kind_of_v<isq::length_t>);
  static_assert(!is_kind_of_v<isq::energy_t>);

  // kind_of preserves character
  static_assert(length_kind::character == quantity_character::scalar);
  static_assert(kind_of<isq::velocity_t>::character == quantity_character::vector);

  // =============================================================================
  // Critical safety test: Torque vs Energy
  // =============================================================================
  // This is the key type safety feature - torque and energy have the same
  // dimension (M*L^2*T^-2) but are different physical quantities.

  // They are both root quantity specs (not related)
  static_assert(is_root_quantity_spec_v<isq::energy_t>);
  static_assert(is_root_quantity_spec_v<isq::torque_t>);

  // Neither is a child of the other
  static_assert(!is_child_of_v<isq::torque_t, isq::energy_t>);
  static_assert(!is_child_of_v<isq::energy_t, isq::torque_t>);

  // NOT implicitly convertible
  static_assert(!implicitly_convertible_v<isq::torque_t, isq::energy_t>);
  static_assert(!implicitly_convertible_v<isq::energy_t, isq::torque_t>);

  // NOT explicitly convertible (no hierarchy relationship)
  static_assert(!explicitly_convertible_v<isq::torque_t, isq::energy_t>);
  static_assert(!explicitly_convertible_v<isq::energy_t, isq::torque_t>);

  // Cannot find common_quantity_spec (would fail to compile if attempted)
  // static_assert(std::same_as<common_quantity_spec<isq::torque_t, isq::energy_t>, ???>);

} // namespace units::test

// Main function - tests are all compile-time, so just need to compile successfully
auto main() -> int {
  return 0;
}
