// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <type_traits>

#include <units/core/quantity_spec.hpp>
#include <units/core/unit.hpp>

namespace units {

  // =============================================================================
  // Reference - Combines quantity specification with unit
  // =============================================================================

  /// A reference combines a quantity specification with a unit, providing
  /// complete metadata for a quantity value except the numeric representation.
  ///
  /// For example: reference<isq::length_t, si::metre_t> represents "length
  /// measured in metres".
  ///
  /// @tparam QSpec The quantity specification (e.g., isq::length_t)
  /// @tparam UnitType The unit (e.g., si::metre_t)
  template<QuantitySpec QSpec, Unit UnitType>
  struct reference {
      using quantity_spec = QSpec;
      using unit          = UnitType;

      /// Get the magnitude from the unit
      using magnitude_type = typename UnitType::magnitude_type;

      /// Multiply two references (creates derived reference)
      template<typename Self, QuantitySpec OtherSpec, Unit OtherUnit>
      constexpr auto operator*(this Self /*self*/, reference<OtherSpec, OtherUnit> /*other*/) {
        // Multiply quantity specs
        using new_spec = decltype(QSpec {} * OtherSpec {});
        // Multiply units
        using new_unit = decltype(UnitType {} * OtherUnit {});
        return reference<new_spec, std::remove_cvref_t<new_unit>> {};
      }

      /// Divide two references (creates derived reference)
      template<typename Self, QuantitySpec OtherSpec, Unit OtherUnit>
      constexpr auto operator/(this Self /*self*/, reference<OtherSpec, OtherUnit> /*other*/) {
        // Divide quantity specs
        using new_spec = decltype(QSpec {} / OtherSpec {});
        // Divide units
        using new_unit = decltype(UnitType {} / OtherUnit {});
        return reference<new_spec, std::remove_cvref_t<new_unit>> {};
      }
  };

  // =============================================================================
  // Reference type trait
  // =============================================================================

  namespace detail {

    template<typename T, typename = void>
    struct has_reference_interface : std::false_type {};

    template<typename T>
    struct has_reference_interface<
        T,
        std::void_t<typename T::quantity_spec, typename T::unit, typename T::magnitude_type>>
        : std::bool_constant<QuantitySpec<typename T::quantity_spec> && Unit<typename T::unit>> {};

  } // namespace detail

  /// Type trait to check if T is a reference
  template<typename T>
  struct is_reference : detail::has_reference_interface<std::remove_cvref_t<T>> {};

  template<typename T>
  inline constexpr bool is_reference_v = is_reference<T>::value;

  /// Concept for reference types
  template<typename T>
  concept Reference = is_reference_v<T>;

  // =============================================================================
  // Reference accessors
  // =============================================================================

  /// Get the quantity specification from a reference
  template<Reference Ref>
  using get_quantity_spec = typename Ref::quantity_spec;

  /// Get the unit from a reference
  template<Reference Ref>
  using get_unit = typename Ref::unit;

  /// Get the magnitude from a reference
  template<Reference Ref>
  using get_reference_magnitude = typename Ref::magnitude_type;

  // =============================================================================
  // Reference factory
  // =============================================================================

  /// Create a reference from a quantity specification and a unit.
  ///
  /// @param spec The quantity specification (value unused, type matters)
  /// @param unit The unit (value unused, type matters)
  /// @return A reference type combining the spec and unit
  template<QuantitySpec QSpec, Unit UnitType>
  [[nodiscard]]
  consteval auto make_reference(QSpec /*spec*/, UnitType /*unit*/) -> reference<QSpec, UnitType> {
    return {};
  }

  // =============================================================================
  // Reference compatibility predicates
  // =============================================================================

  /// Check if two references are compatible (can be converted between).
  /// References are compatible if they have the same dimension.
  template<Reference Ref1, Reference Ref2>
  inline constexpr bool references_compatible_v =
      // For now, require exact quantity spec match or hierarchy relationship
      std::same_as<get_quantity_spec<Ref1>, get_quantity_spec<Ref2>>
      || implicitly_convertible_v<get_quantity_spec<Ref1>, get_quantity_spec<Ref2>>
      || implicitly_convertible_v<get_quantity_spec<Ref2>, get_quantity_spec<Ref1>>;

  // =============================================================================
  // Unit-to-reference conversions
  // =============================================================================

  namespace detail {

    /// Extract quantity spec from a unit's quantity_kind
    template<typename QtyKind>
    struct extract_quantity_spec {
        using type = QtyKind;
    };

    template<QuantitySpec QSpec>
    struct extract_quantity_spec<kind_of<QSpec>> {
        using type = QSpec;
    };

  } // namespace detail

  /// Create a reference from just a unit (uses the unit's associated quantity kind)
  template<Unit UnitType>
    requires requires { typename UnitType::quantity_kind; }
  [[nodiscard]]
  consteval auto make_reference(UnitType /*unit*/) {
    using qty_kind = typename UnitType::quantity_kind;
    using qty_spec = typename detail::extract_quantity_spec<qty_kind>::type;
    return reference<qty_spec, UnitType> {};
  }

  /// Type alias to create a reference type from a unit
  template<Unit UnitType>
    requires requires { typename UnitType::quantity_kind; }
  using reference_from_unit = decltype(make_reference(UnitType {}));

  // =============================================================================
  // Conversion factor between references
  // =============================================================================

  /// Compute the conversion factor from one reference to another.
  /// The references must be compatible (same quantity spec or related).
  ///
  /// @tparam FromRef The source reference
  /// @tparam ToRef The target reference
  /// @return The conversion factor as a floating-point value
  template<Reference FromRef, Reference ToRef, typename T = double>
    requires std::is_floating_point_v<T>
  [[nodiscard]]
  consteval auto reference_conversion_factor() -> T {
    // Conversion factor is the ratio of magnitudes
    return conversion_factor<get_unit<FromRef>, get_unit<ToRef>, T>();
  }

} // namespace units
