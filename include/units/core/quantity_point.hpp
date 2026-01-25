// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <type_traits>

#include <units/core/quantity.hpp>
#include <units/core/quantity_spec.hpp>
#include <units/core/reference.hpp>
#include <units/core/unit.hpp>

namespace units {

  // =============================================================================
  // Point origin concepts and types
  // =============================================================================

  // Forward declarations
  template<typename T>
  struct is_point_origin;

  template<typename T>
  concept PointOrigin = is_point_origin<std::remove_cvref_t<T>>::value;

  // =============================================================================
  // absolute_point_origin - Defines the zero point for a quantity spec
  // =============================================================================

  /// An absolute point origin defines the "zero" for a particular quantity
  /// specification. For example, absolute zero for thermodynamic temperature.
  ///
  /// @tparam QSpec The quantity specification this origin is for
  template<QuantitySpec QSpec>
  struct absolute_point_origin {
      using quantity_spec = QSpec;

      /// Tag to identify this as an absolute origin
      static constexpr bool is_absolute = true;
  };

  // =============================================================================
  // relative_point_origin - Offset from another origin
  // =============================================================================

  /// A relative point origin is defined as an offset from another origin.
  /// For example, the ice point (0°C) is 273.15 K above absolute zero.
  ///
  /// @tparam Origin The base origin
  /// @tparam OffsetRef The reference type for the offset quantity
  /// @tparam OffsetRep The representation type for the offset
  /// @tparam OffsetVal The offset value (as a compile-time parameter)
  template<PointOrigin Origin, Reference OffsetRef, typename OffsetRep, OffsetRep OffsetVal>
  struct relative_point_origin {
      using base_origin                       = Origin;
      using offset_reference                  = OffsetRef;
      using offset_rep                        = OffsetRep;
      static constexpr OffsetRep offset_value = OffsetVal;

      /// Tag to identify this as a relative origin
      static constexpr bool is_absolute = false;

      /// Get the offset as a quantity
      [[nodiscard]]
      static constexpr auto offset() noexcept {
        return quantity<OffsetRef, OffsetRep> { OffsetVal };
      }
  };

  // =============================================================================
  // Point origin type traits
  // =============================================================================

  namespace detail {

    template<typename T, typename = void>
    struct has_point_origin_interface : std::false_type {};

    template<typename T>
    struct has_point_origin_interface<T, std::void_t<decltype(T::is_absolute)>> : std::true_type {};

    /// Check if T is an absolute point origin
    template<typename T, typename = void>
    struct is_absolute_origin_impl : std::false_type {};

    template<typename T>
    struct is_absolute_origin_impl<
        T,
        std::enable_if_t<has_point_origin_interface<T>::value && T::is_absolute>>
        : std::true_type {};

    /// Check if T is a relative point origin
    template<typename T, typename = void>
    struct is_relative_origin_impl : std::false_type {};

    template<typename T>
    struct is_relative_origin_impl<
        T,
        std::enable_if_t<has_point_origin_interface<T>::value && !T::is_absolute>>
        : std::true_type {};

  } // namespace detail

  /// Type trait to check if T is a point origin
  template<typename T>
  struct is_point_origin : detail::has_point_origin_interface<std::remove_cvref_t<T>> {};

  template<typename T>
  inline constexpr bool is_point_origin_v = is_point_origin<T>::value;

  /// Type trait to check if T is an absolute origin
  template<typename T>
  inline constexpr bool is_absolute_origin_v
      = detail::is_absolute_origin_impl<std::remove_cvref_t<T>>::value;

  /// Type trait to check if T is a relative origin
  template<typename T>
  inline constexpr bool is_relative_origin_v
      = detail::is_relative_origin_impl<std::remove_cvref_t<T>>::value;

  // =============================================================================
  // Origin utilities
  // =============================================================================

  namespace detail {

    /// Get the root (absolute) origin from any origin
    template<PointOrigin Origin>
    struct root_origin_impl {
        using type = Origin; // Absolute origin is its own root
    };

    template<PointOrigin Base, Reference Ref, typename Rep, Rep Val>
    struct root_origin_impl<relative_point_origin<Base, Ref, Rep, Val>> {
        using type = typename root_origin_impl<Base>::type;
    };

    /// Get the quantity spec from an origin
    template<PointOrigin Origin>
    struct origin_quantity_spec_impl;

    template<QuantitySpec QSpec>
    struct origin_quantity_spec_impl<absolute_point_origin<QSpec>> {
        using type = QSpec;
    };

    template<PointOrigin Base, Reference Ref, typename Rep, Rep Val>
    struct origin_quantity_spec_impl<relative_point_origin<Base, Ref, Rep, Val>> {
        using type = typename origin_quantity_spec_impl<Base>::type;
    };

  } // namespace detail

  /// Get the root (absolute) origin from any origin
  template<PointOrigin Origin>
  using root_origin = typename detail::root_origin_impl<Origin>::type;

  /// Get the quantity spec associated with an origin
  template<PointOrigin Origin>
  using origin_quantity_spec = typename detail::origin_quantity_spec_impl<Origin>::type;

  // =============================================================================
  // Default point origin for a reference
  // =============================================================================

  namespace detail {

    /// Default origin is an absolute origin for the quantity spec
    template<Reference Ref>
    struct default_point_origin_impl {
        using type = absolute_point_origin<get_quantity_spec<Ref>>;
    };

  } // namespace detail

  /// Get the default point origin for a reference
  template<Reference Ref>
  using default_point_origin = typename detail::default_point_origin_impl<Ref>::type;

  // =============================================================================
  // quantity_point - An affine quantity with an origin
  // =============================================================================

  // Forward declaration
  template<Reference Ref, PointOrigin Origin, typename Rep>
    requires Representation<Rep>
  class quantity_point;

  /// Type trait to check if T is a quantity_point
  template<typename T>
  struct is_quantity_point : std::false_type {};

  template<Reference Ref, PointOrigin Origin, typename Rep>
  struct is_quantity_point<quantity_point<Ref, Origin, Rep>> : std::true_type {};

  template<typename T>
  inline constexpr bool is_quantity_point_v = is_quantity_point<std::remove_cvref_t<T>>::value;

  template<typename T>
  concept QuantityPoint = is_quantity_point_v<T>;

  /// A quantity_point represents a specific "position" in a quantity space,
  /// defined relative to an origin. Unlike plain quantities (which are
  /// displacements/differences), quantity_points have absolute meaning.
  ///
  /// For example:
  /// - A temperature of 300 K is a quantity_point (absolute position)
  /// - A temperature difference of 10 K is a quantity (displacement)
  ///
  /// Operations:
  /// - point - point = quantity (displacement between two points)
  /// - point + quantity = point (offset a point by a displacement)
  /// - point - quantity = point (offset a point by a negative displacement)
  /// - point + point is DELETED (adding two absolute values makes no sense)
  ///
  /// @tparam Ref The reference type (quantity_spec + unit)
  /// @tparam Origin The point origin type
  /// @tparam Rep The numeric representation type
  template<Reference Ref, PointOrigin Origin = default_point_origin<Ref>, typename Rep = double>
    requires Representation<Rep>
  class quantity_point {
      quantity<Ref, Rep> displacement_;

    public:
      // -------------------------------------------------------------------------
      // Type aliases
      // -------------------------------------------------------------------------

      using reference_type = Ref;
      using origin_type    = Origin;
      using rep            = Rep;
      using quantity_spec  = get_quantity_spec<Ref>;
      using unit_type      = get_unit<Ref>;
      using quantity_type  = quantity<Ref, Rep>;

      // -------------------------------------------------------------------------
      // Constructors
      // -------------------------------------------------------------------------

      /// Default constructor is deleted - points must have values
      constexpr quantity_point() = delete;

      /// Construct from a quantity (displacement from origin)
      constexpr explicit quantity_point(quantity<Ref, Rep> qty) noexcept : displacement_(qty) {}

      /// Construct from a raw value (displacement from origin)
      constexpr explicit quantity_point(Rep val) noexcept
          : displacement_(quantity<Ref, Rep> { val }) {}

      /// Copy constructor
      constexpr quantity_point(const quantity_point &) noexcept = default;

      /// Move constructor
      constexpr quantity_point(quantity_point &&) noexcept = default;

      /// Converting constructor from compatible quantity_point
      /// Handles conversion between different origins and units
      template<Reference OtherRef, PointOrigin OtherOrigin, typename OtherRep>
        requires(!std::same_as<quantity_point, quantity_point<OtherRef, OtherOrigin, OtherRep>>)
             && std::convertible_to<OtherRep, Rep>
      constexpr explicit quantity_point(
          const quantity_point<OtherRef, OtherOrigin, OtherRep> &other) noexcept;

      // -------------------------------------------------------------------------
      // Destructor
      // -------------------------------------------------------------------------

      constexpr ~quantity_point() noexcept = default;

      // -------------------------------------------------------------------------
      // Assignment
      // -------------------------------------------------------------------------

      constexpr auto operator=(const quantity_point &) noexcept -> quantity_point & = default;
      constexpr auto operator=(quantity_point &&) noexcept -> quantity_point &      = default;

      // -------------------------------------------------------------------------
      // Accessors
      // -------------------------------------------------------------------------

      /// Get the displacement from the origin as a quantity
      [[nodiscard]]
      constexpr auto quantity_from_origin() const noexcept -> quantity_type {
        return displacement_;
      }

      /// Get the raw numeric value (displacement from origin)
      [[nodiscard]]
      constexpr auto value() const noexcept -> Rep {
        return displacement_.value();
      }

      /// Get the numeric value in a specific unit
      template<Unit ToUnit>
      [[nodiscard]]
      constexpr auto numerical_value_in(ToUnit unit) const noexcept {
        return displacement_.numerical_value_in(unit);
      }

      // -------------------------------------------------------------------------
      // Conversion methods
      // -------------------------------------------------------------------------

      /// Convert to a different unit (same origin)
      template<Unit ToUnit>
      [[nodiscard]]
      constexpr auto in(ToUnit /*unit*/) const noexcept {
        using new_ref = reference<quantity_spec, ToUnit>;
        auto new_qty  = displacement_.in(ToUnit {});
        return quantity_point<new_ref, Origin, Rep> { new_qty };
      }

      // -------------------------------------------------------------------------
      // Point arithmetic - FORBIDDEN operations
      // -------------------------------------------------------------------------

      /// Adding two points is meaningless - DELETED
      constexpr auto operator+(quantity_point) const = delete;

      // -------------------------------------------------------------------------
      // Point arithmetic - valid operations
      // -------------------------------------------------------------------------

      /// Subtract two points = displacement (quantity)
      /// point - point = quantity
      [[nodiscard]]
      constexpr auto operator-(const quantity_point &other) const noexcept -> quantity_type {
        return displacement_ - other.displacement_;
      }

      /// Add a quantity to a point = new point
      /// point + quantity = point
      [[nodiscard]]
      constexpr auto operator+(const quantity_type &delta) const noexcept -> quantity_point {
        return quantity_point { displacement_ + delta };
      }

      /// Subtract a quantity from a point = new point
      /// point - quantity = point
      [[nodiscard]]
      constexpr auto operator-(const quantity_type &delta) const noexcept -> quantity_point {
        return quantity_point { displacement_ - delta };
      }

      // -------------------------------------------------------------------------
      // Compound assignment
      // -------------------------------------------------------------------------

      constexpr auto operator+=(const quantity_type &delta) noexcept -> quantity_point & {
        displacement_ += delta;
        return *this;
      }

      constexpr auto operator-=(const quantity_type &delta) noexcept -> quantity_point & {
        displacement_ -= delta;
        return *this;
      }
  };

  // =============================================================================
  // Comparison operators
  // =============================================================================

  /// Equality comparison for quantity_points with the same reference and origin
  template<Reference Ref, PointOrigin Origin, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator==(const quantity_point<Ref, Origin, Rep1> &lhs,
                            const quantity_point<Ref, Origin, Rep2> &rhs) noexcept -> bool {
    return lhs.quantity_from_origin() == rhs.quantity_from_origin();
  }

  /// Three-way comparison for quantity_points with the same reference and origin
  template<Reference Ref, PointOrigin Origin, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator<=>(const quantity_point<Ref, Origin, Rep1> &lhs,
                             const quantity_point<Ref, Origin, Rep2> &rhs) noexcept {
    return lhs.quantity_from_origin() <=> rhs.quantity_from_origin();
  }

  // =============================================================================
  // Origin offset calculation
  // =============================================================================

  namespace detail {

    /// Calculate the offset from one origin to another (both as kelvin quantities)
    template<PointOrigin FromOrigin, PointOrigin ToOrigin, Reference Ref, typename Rep>
    struct origin_offset_impl;

    /// Same origin - no offset
    template<PointOrigin Origin, Reference Ref, typename Rep>
    struct origin_offset_impl<Origin, Origin, Ref, Rep> {
        static constexpr auto value() noexcept {
          return quantity<Ref, Rep> { Rep { 0 } };
        }
    };

    /// From absolute to absolute (same underlying quantity spec) - no offset
    template<QuantitySpec QSpec1, QuantitySpec QSpec2, Reference Ref, typename Rep>
      requires std::same_as<QSpec1, QSpec2>
    struct origin_offset_impl<absolute_point_origin<QSpec1>,
                              absolute_point_origin<QSpec2>,
                              Ref,
                              Rep> {
        static constexpr auto value() noexcept {
          return quantity<Ref, Rep> { Rep { 0 } };
        }
    };

    /// From relative origin to its base - negative of the offset
    template<PointOrigin Base,
             Reference   OffsetRef,
             typename OffsetRep,
             OffsetRep OffsetVal,
             Reference Ref,
             typename Rep>
    struct origin_offset_impl<relative_point_origin<Base, OffsetRef, OffsetRep, OffsetVal>,
                              Base,
                              Ref,
                              Rep> {
        static constexpr auto value() noexcept {
          // The offset from relative to base is negative of the defined offset
          auto offset = relative_point_origin<Base, OffsetRef, OffsetRep, OffsetVal>::offset();
          return quantity<Ref, Rep> { static_cast<Rep>(
              -offset.value() * reference_conversion_factor<OffsetRef, Ref, Rep>()) };
        }
    };

    /// From base origin to relative origin - the defined offset
    template<PointOrigin Base,
             Reference   OffsetRef,
             typename OffsetRep,
             OffsetRep OffsetVal,
             Reference Ref,
             typename Rep>
    struct origin_offset_impl<Base,
                              relative_point_origin<Base, OffsetRef, OffsetRep, OffsetVal>,
                              Ref,
                              Rep> {
        static constexpr auto value() noexcept {
          auto offset = relative_point_origin<Base, OffsetRef, OffsetRep, OffsetVal>::offset();
          return quantity<Ref, Rep> { static_cast<Rep>(
              offset.value() * reference_conversion_factor<OffsetRef, Ref, Rep>()) };
        }
    };

  } // namespace detail

  /// Get the offset from one origin to another
  template<PointOrigin FromOrigin, PointOrigin ToOrigin, Reference Ref, typename Rep = double>
  [[nodiscard]]
  constexpr auto origin_offset() noexcept {
    return detail::origin_offset_impl<FromOrigin, ToOrigin, Ref, Rep>::value();
  }

  // =============================================================================
  // Converting constructor implementation
  // =============================================================================

  template<Reference Ref, PointOrigin Origin, typename Rep>
    requires Representation<Rep>
  template<Reference OtherRef, PointOrigin OtherOrigin, typename OtherRep>
    requires(!std::same_as<quantity_point<Ref, Origin, Rep>,
                           quantity_point<OtherRef, OtherOrigin, OtherRep>>)
         && std::convertible_to<OtherRep, Rep>
  constexpr quantity_point<Ref, Origin, Rep>::quantity_point(
      const quantity_point<OtherRef, OtherOrigin, OtherRep> &other) noexcept {
    // Convert the other point's displacement to our reference
    auto converted_qty = quantity<Ref, Rep> { static_cast<Rep>(
        other.value() * reference_conversion_factor<OtherRef, Ref, Rep>()) };

    // If origins differ, we need to adjust for the origin offset
    if constexpr (!std::same_as<Origin, OtherOrigin>) {
      // Add the offset from OtherOrigin to Origin
      auto offset   = origin_offset<OtherOrigin, Origin, Ref, Rep>();
      displacement_ = converted_qty + offset;
    }
    else {
      displacement_ = converted_qty;
    }
  }

  // =============================================================================
  // Factory function
  // =============================================================================

  /// Create a quantity_point from a quantity
  template<PointOrigin Origin, Reference Ref, typename Rep>
  [[nodiscard]]
  constexpr auto make_quantity_point(quantity<Ref, Rep> qty) noexcept {
    return quantity_point<Ref, Origin, Rep> { qty };
  }

  /// Create a quantity_point from a value and unit
  template<PointOrigin Origin, Unit UnitType, typename Rep = double>
    requires requires { typename UnitType::quantity_kind; }
  [[nodiscard]]
  constexpr auto make_quantity_point(Rep value, UnitType /*unit*/) noexcept {
    using ref = reference_from_unit<UnitType>;
    return quantity_point<ref, Origin, Rep> { quantity<ref, Rep> { value } };
  }

  // =============================================================================
  // Zero-overhead guarantees
  // =============================================================================

  namespace detail {

    template<typename QSpec, typename UnitT, typename Origin, typename Rep>
    struct quantity_point_zero_overhead_check {
        using ref = reference<QSpec, UnitT>;
        using qp  = quantity_point<ref, Origin, Rep>;

        static_assert(sizeof(qp) == sizeof(Rep),
                      "quantity_point should have the same size as its representation");

        static_assert(std::is_trivially_copyable_v<qp>,
                      "quantity_point should be trivially copyable");

        static_assert(std::is_standard_layout_v<qp>, "quantity_point should have standard layout");
    };

  } // namespace detail

} // namespace units
