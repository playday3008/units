// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <type_traits>

#include <units/core/reference.hpp>
#include <units/core/unit.hpp>

namespace units {

  // =============================================================================
  // Representation concepts
  // =============================================================================

  /// Concept for types that can be used as quantity representations
  template<typename T>
  concept Representation = std::regular<T> && requires(T lhs, T rhs) {
    { lhs + rhs } -> std::convertible_to<T>;
    { lhs - rhs } -> std::convertible_to<T>;
    { lhs * rhs } -> std::convertible_to<T>;
    { lhs / rhs } -> std::convertible_to<T>;
    { -lhs } -> std::convertible_to<T>;
  };

  // =============================================================================
  // Forward declarations
  // =============================================================================

  template<Reference Ref, typename Rep = double>
    requires Representation<Rep>
  class quantity;

  template<typename T>
  struct is_quantity : std::false_type {};

  template<Reference Ref, typename Rep>
  struct is_quantity<quantity<Ref, Rep>> : std::true_type {};

  template<typename T>
  inline constexpr bool is_quantity_v = is_quantity<std::remove_cvref_t<T>>::value;

  template<typename T>
  concept Quantity = is_quantity_v<T>;

  // =============================================================================
  // Common reference computation
  // =============================================================================

  namespace detail {

    /// Compute common reference for two quantities
    template<Reference Ref1, Reference Ref2>
    struct common_reference_impl {
        // Use the common quantity spec
        using common_spec = common_quantity_spec<get_quantity_spec<Ref1>, get_quantity_spec<Ref2>>;

        // For units, prefer the first unit (or could use common base)
        // For now, use Ref1's unit - caller handles conversions
        using type = reference<common_spec, get_unit<Ref1>>;
    };

    template<Reference Ref1, Reference Ref2>
    using common_reference = typename common_reference_impl<Ref1, Ref2>::type;

    /// Check if two references are the same (ignoring cv-qualifiers)
    template<typename Ref1, typename Ref2>
    inline constexpr bool same_reference_v
        = std::same_as<std::remove_cvref_t<Ref1>, std::remove_cvref_t<Ref2>>;

  } // namespace detail

  // =============================================================================
  // quantity - The primary user-facing type
  // =============================================================================

  /// A physical quantity combining a numeric value with a reference (unit + spec).
  ///
  /// Design goals:
  /// - Zero overhead: sizeof(quantity) == sizeof(Rep)
  /// - Type safety: Can only combine compatible quantities
  /// - Compile-time validation: Invalid operations fail at compile time
  ///
  /// @tparam Ref The reference type (combines quantity_spec with unit)
  /// @tparam Rep The numeric representation type (default: double)
  template<Reference Ref, typename Rep>
    requires Representation<Rep>
  class quantity {
      Rep value_;

    public:
      // -------------------------------------------------------------------------
      // Destructor
      // -------------------------------------------------------------------------

      /// Defaulted destructor
      constexpr ~quantity() noexcept = default;

      // -------------------------------------------------------------------------
      // Type aliases
      // -------------------------------------------------------------------------

      using reference_type = Ref;
      using rep            = Rep;
      using quantity_spec  = get_quantity_spec<Ref>;
      using unit_type      = get_unit<Ref>;

      // -------------------------------------------------------------------------
      // Constructors
      // -------------------------------------------------------------------------

      /// Default constructor is deleted - quantities must have values
      constexpr quantity() = delete;

      /// Construct from a numeric value
      constexpr explicit quantity(Rep v) noexcept : value_(v) {}

      /// Copy constructor
      constexpr quantity(const quantity &) noexcept = default;

      /// Move constructor
      constexpr quantity(quantity &&) noexcept = default;

      // NOLINTBEGIN(google-explicit-constructor,hicpp-explicit-conversions)

      /// Converting constructor from compatible quantity (implicit)
      /// Allows width -> length conversion, and unit conversions
      template<Reference OtherRef, typename OtherRep>
        requires(!detail::same_reference_v<Ref, OtherRef>)
             && implicitly_convertible_v<get_quantity_spec<OtherRef>, quantity_spec>
             && std::convertible_to<OtherRep, Rep>
      constexpr quantity(const quantity<OtherRef, OtherRep> &other) noexcept
          : value_(static_cast<Rep>(other.value()
                                    * reference_conversion_factor<OtherRef, Ref, Rep>())) {}

      // NOLINTEND(google-explicit-constructor,hicpp-explicit-conversions)

      // -------------------------------------------------------------------------
      // Assignment
      // -------------------------------------------------------------------------

      constexpr auto operator=(const quantity &) noexcept -> quantity & = default;
      constexpr auto operator=(quantity &&) noexcept -> quantity &      = default;

      // -------------------------------------------------------------------------
      // Accessors
      // -------------------------------------------------------------------------

      /// Get the raw numeric value
      [[nodiscard]]
      constexpr auto value() const noexcept -> Rep {
        return value_;
      }

      /// Get the numeric value in a specific unit
      template<Unit ToUnit>
      [[nodiscard]]
      constexpr auto numerical_value_in(ToUnit /*unit*/) const noexcept {
        constexpr auto factor = conversion_factor<unit_type, ToUnit, Rep>();
        return value_ * factor;
      }

      // -------------------------------------------------------------------------
      // Conversion methods
      // -------------------------------------------------------------------------

      /// Convert to a different unit (returns new quantity)
      /// The target unit must be compatible with this quantity's unit.
      template<Unit ToUnit>
      [[nodiscard]]
      constexpr auto in(ToUnit /*unit*/) const noexcept {
        using new_ref         = reference<quantity_spec, ToUnit>;
        constexpr auto factor = conversion_factor<unit_type, ToUnit, Rep>();
        return quantity<new_ref, Rep> { static_cast<Rep>(value_ * factor) };
      }

      /// Force conversion to a different unit (potentially lossy)
      /// Use when you know the conversion may lose precision.
      template<Unit ToUnit>
      [[nodiscard]]
      constexpr auto force_in(ToUnit /*unit*/) const noexcept {
        using new_ref         = reference<quantity_spec, ToUnit>;
        constexpr auto factor = conversion_factor<unit_type, ToUnit, Rep>();
        return quantity<new_ref, Rep> { static_cast<Rep>(value_ * factor) };
      }

      // -------------------------------------------------------------------------
      // Unary operators
      // -------------------------------------------------------------------------

      /// Unary plus (identity)
      [[nodiscard]]
      constexpr auto operator+() const noexcept -> quantity {
        return *this;
      }

      /// Unary minus (negation)
      [[nodiscard]]
      constexpr auto operator-() const noexcept -> quantity {
        return quantity { -value_ };
      }

      // -------------------------------------------------------------------------
      // Compound assignment - same reference
      // -------------------------------------------------------------------------

      constexpr auto operator+=(const quantity &rhs) noexcept -> quantity & {
        value_ += rhs.value_;
        return *this;
      }

      constexpr auto operator-=(const quantity &rhs) noexcept -> quantity & {
        value_ -= rhs.value_;
        return *this;
      }

      /// Multiply by scalar
      template<typename T>
        requires std::convertible_to<T, Rep> && (!Quantity<T>)
      constexpr auto operator*=(T scalar) noexcept -> quantity & {
        value_ *= static_cast<Rep>(scalar);
        return *this;
      }

      /// Divide by scalar
      template<typename T>
        requires std::convertible_to<T, Rep> && (!Quantity<T>)
      constexpr auto operator/=(T scalar) noexcept -> quantity & {
        value_ /= static_cast<Rep>(scalar);
        return *this;
      }
  };

  // =============================================================================
  // Binary arithmetic operators - same reference
  // =============================================================================

  /// Add two quantities with the same reference
  template<Reference Ref, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator+(const quantity<Ref, Rep1> &lhs,
                           const quantity<Ref, Rep2> &rhs) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    return quantity<Ref, common_rep> { static_cast<common_rep>(lhs.value())
                                       + static_cast<common_rep>(rhs.value()) };
  }

  /// Subtract two quantities with the same reference
  template<Reference Ref, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator-(const quantity<Ref, Rep1> &lhs,
                           const quantity<Ref, Rep2> &rhs) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    return quantity<Ref, common_rep> { static_cast<common_rep>(lhs.value())
                                       - static_cast<common_rep>(rhs.value()) };
  }

  // =============================================================================
  // Binary arithmetic operators - different compatible references
  // =============================================================================

  /// Add two quantities with different but compatible references
  /// Result uses the first quantity's unit, converts the second
  template<Reference Ref1, Reference Ref2, typename Rep1, typename Rep2>
    requires(!std::same_as<Ref1, Ref2>) && references_compatible_v<Ref1, Ref2>
  [[nodiscard]]
  constexpr auto operator+(const quantity<Ref1, Rep1> &lhs,
                           const quantity<Ref2, Rep2> &rhs) noexcept {
    using common_rep  = std::common_type_t<Rep1, Rep2>;
    using common_spec = common_quantity_spec<get_quantity_spec<Ref1>, get_quantity_spec<Ref2>>;
    using result_ref  = reference<common_spec, get_unit<Ref1>>;

    constexpr auto factor = reference_conversion_factor<Ref2, Ref1, common_rep>();
    return quantity<result_ref, common_rep> { static_cast<common_rep>(lhs.value())
                                              + static_cast<common_rep>(rhs.value()) * factor };
  }

  /// Subtract two quantities with different but compatible references
  template<Reference Ref1, Reference Ref2, typename Rep1, typename Rep2>
    requires(!std::same_as<Ref1, Ref2>) && references_compatible_v<Ref1, Ref2>
  [[nodiscard]]
  constexpr auto operator-(const quantity<Ref1, Rep1> &lhs,
                           const quantity<Ref2, Rep2> &rhs) noexcept {
    using common_rep  = std::common_type_t<Rep1, Rep2>;
    using common_spec = common_quantity_spec<get_quantity_spec<Ref1>, get_quantity_spec<Ref2>>;
    using result_ref  = reference<common_spec, get_unit<Ref1>>;

    constexpr auto factor = reference_conversion_factor<Ref2, Ref1, common_rep>();
    return quantity<result_ref, common_rep> { static_cast<common_rep>(lhs.value())
                                              - static_cast<common_rep>(rhs.value()) * factor };
  }

  // =============================================================================
  // Multiplication and division - creates derived quantities
  // =============================================================================

  /// Multiply two quantities - creates derived reference
  template<Reference Ref1, Reference Ref2, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator*(const quantity<Ref1, Rep1> &lhs,
                           const quantity<Ref2, Rep2> &rhs) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    using result_ref = decltype(Ref1 {} * Ref2 {});
    return quantity<std::remove_cvref_t<result_ref>, common_rep> {
      static_cast<common_rep>(lhs.value()) * static_cast<common_rep>(rhs.value())
    };
  }

  /// Divide two quantities - creates derived reference
  template<Reference Ref1, Reference Ref2, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator/(const quantity<Ref1, Rep1> &lhs,
                           const quantity<Ref2, Rep2> &rhs) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    using result_ref = decltype(Ref1 {} / Ref2 {});
    return quantity<std::remove_cvref_t<result_ref>, common_rep> {
      static_cast<common_rep>(lhs.value()) / static_cast<common_rep>(rhs.value())
    };
  }

  // =============================================================================
  // Scalar multiplication and division
  // =============================================================================

  /// Multiply quantity by scalar (quantity * scalar)
  template<Reference Ref, typename Rep, typename Scalar>
    requires std::convertible_to<Scalar, Rep> && (!Quantity<Scalar>)
  [[nodiscard]]
  constexpr auto operator*(const quantity<Ref, Rep> &q, Scalar scalar) noexcept {
    return quantity<Ref, Rep> { q.value() * static_cast<Rep>(scalar) };
  }

  /// Multiply quantity by scalar (scalar * quantity)
  template<Reference Ref, typename Rep, typename Scalar>
    requires std::convertible_to<Scalar, Rep> && (!Quantity<Scalar>)
  [[nodiscard]]
  constexpr auto operator*(Scalar scalar, const quantity<Ref, Rep> &q) noexcept {
    return quantity<Ref, Rep> { static_cast<Rep>(scalar) * q.value() };
  }

  /// Divide quantity by scalar
  template<Reference Ref, typename Rep, typename Scalar>
    requires std::convertible_to<Scalar, Rep> && (!Quantity<Scalar>)
  [[nodiscard]]
  constexpr auto operator/(const quantity<Ref, Rep> &q, Scalar scalar) noexcept {
    return quantity<Ref, Rep> { q.value() / static_cast<Rep>(scalar) };
  }

  // =============================================================================
  // Comparison operators
  // =============================================================================

  /// Equality comparison - same reference
  template<Reference Ref, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator==(const quantity<Ref, Rep1> &lhs, const quantity<Ref, Rep2> &rhs) noexcept
      -> bool {
    return lhs.value() == rhs.value();
  }

  /// Equality comparison - different compatible references
  template<Reference Ref1, Reference Ref2, typename Rep1, typename Rep2>
    requires(!std::same_as<Ref1, Ref2>) && references_compatible_v<Ref1, Ref2>
  [[nodiscard]]
  constexpr auto operator==(const quantity<Ref1, Rep1> &lhs,
                            const quantity<Ref2, Rep2> &rhs) noexcept -> bool {
    using common_rep      = std::common_type_t<Rep1, Rep2>;
    constexpr auto factor = reference_conversion_factor<Ref2, Ref1, common_rep>();
    return static_cast<common_rep>(lhs.value()) == static_cast<common_rep>(rhs.value()) * factor;
  }

  /// Three-way comparison - same reference
  template<Reference Ref, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto operator<=>(const quantity<Ref, Rep1> &lhs,
                             const quantity<Ref, Rep2> &rhs) noexcept {
    return lhs.value() <=> rhs.value();
  }

  /// Three-way comparison - different compatible references
  template<Reference Ref1, Reference Ref2, typename Rep1, typename Rep2>
    requires(!std::same_as<Ref1, Ref2>) && references_compatible_v<Ref1, Ref2>
  [[nodiscard]]
  constexpr auto operator<=>(const quantity<Ref1, Rep1> &lhs,
                             const quantity<Ref2, Rep2> &rhs) noexcept {
    using common_rep      = std::common_type_t<Rep1, Rep2>;
    constexpr auto factor = reference_conversion_factor<Ref2, Ref1, common_rep>();
    return static_cast<common_rep>(lhs.value()) <=> static_cast<common_rep>(rhs.value()) * factor;
  }

  // =============================================================================
  // Quantity factory function
  // =============================================================================

  /// Create a quantity from a value and a unit
  template<Unit UnitType, typename Rep = double>
    requires requires { typename UnitType::quantity_kind; }
  [[nodiscard]]
  constexpr auto make_quantity(Rep value, UnitType /*unit*/) noexcept {
    using ref = reference_from_unit<UnitType>;
    return quantity<ref, Rep> { value };
  }

  // =============================================================================
  // Zero-overhead guarantees
  // =============================================================================

  // These static assertions verify the zero-overhead principle
  namespace detail {

    // Helper to test zero overhead without including SI headers
    template<typename QSpec, typename UnitT, typename Rep>
    struct zero_overhead_check {
        using ref = reference<QSpec, UnitT>;
        using qty = quantity<ref, Rep>;

        static_assert(sizeof(qty) == sizeof(Rep),
                      "quantity should have the same size as its representation");

        static_assert(std::is_trivially_copyable_v<qty>, "quantity should be trivially copyable");

        static_assert(std::is_standard_layout_v<qty>, "quantity should have standard layout");

        static_assert(std::is_nothrow_copy_constructible_v<qty>,
                      "quantity should be nothrow copy constructible");

        static_assert(std::is_nothrow_move_constructible_v<qty>,
                      "quantity should be nothrow move constructible");
    };

  } // namespace detail

} // namespace units
