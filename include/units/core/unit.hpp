// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include <algorithm>
#include <concepts>
#include <type_traits>

#include <units/core/quantity_spec.hpp>
#include <units/core/ratio.hpp>

namespace units {

  // =============================================================================
  // fixed_string - Compile-time string for unit symbols
  // =============================================================================

  /// A compile-time fixed-length string for storing unit symbols.
  /// Supports string literal construction via CTAD.
  ///
  /// NOTE: This is an NTTP (non-type template parameter) structural type.
  /// Structural types require: public members, no user-declared copy/move operations.
  /// The C-style array and implicit constructor are intentional for CTAD with string literals.
  template<std::size_t N>
  struct fixed_string {
      // NOLINTBEGIN(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays)
      // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
      char data[N] {}; // C-style array required for structural type NTTP usage
      // NOLINTEND(misc-non-private-member-variables-in-classes)

      consteval fixed_string() = default;

      // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
      consteval fixed_string(const char (&str)[N]) { // Implicit for CTAD: fixed_string{"m"}
        std::copy_n(str, N, data);
      }
      // NOLINTEND(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays)

      [[nodiscard]]
      consteval auto size() const noexcept -> std::size_t {
        return N - 1; // Exclude null terminator
      }

      [[nodiscard]]
      consteval auto operator[](std::size_t i) const -> char {
        return data[i];
      }

      template<std::size_t M>
      [[nodiscard]]
      consteval auto operator==(const fixed_string<M> &other) const -> bool {
        if constexpr (N != M) {
          return false;
        }
        else {
          for (std::size_t i = 0; i < N; ++i) {
            if (data[i] != other.data[i]) {
              return false;
            }
          }
          return true;
        }
      }
  };

  // NOLINTBEGIN(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays)

  // CTAD guide
  template<std::size_t N>
  fixed_string(const char (&)[N]) -> fixed_string<N>;

  // NOLINTEND(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays)

  // =============================================================================
  // Magnitude - Unit scaling factor
  // =============================================================================

  /// A magnitude represents the scaling factor of a unit relative to its
  /// base unit. For most units this is a simple ratio. More complex magnitudes
  /// involving irrational numbers (pi, sqrt) can be added later.
  ///
  /// @tparam R The rational scaling factor (must satisfy Ratio concept)
  template<typename R>
    requires Ratio<R>
  struct magnitude {
      using ratio = typename R::type;

      static constexpr auto num = ratio::num;
      static constexpr auto den = ratio::den;

      /// Get the magnitude as a floating point value
      template<typename T = double>
        requires std::is_floating_point_v<T>
      [[nodiscard]]
      static consteval auto value() noexcept -> T {
        return ratio::template value<T>();
      }
  };

  /// Type alias for common magnitude of 1
  using mag_one = magnitude<ratio_one>;

  /// Create a magnitude from a compile-time integer
  template<std::intmax_t N>
  using mag = magnitude<ratio<N>>;

  /// Create a magnitude from a ratio
  template<std::intmax_t Num, std::intmax_t Den>
  using mag_ratio = magnitude<ratio<Num, Den>>;

  // =============================================================================
  // Magnitude arithmetic
  // =============================================================================

  namespace detail {

    template<typename M1, typename M2>
    struct mag_multiply_impl {
        using type = magnitude<ratio_multiply<typename M1::ratio, typename M2::ratio>>;
    };

    template<typename M1, typename M2>
    struct mag_divide_impl {
        using type = magnitude<ratio_divide<typename M1::ratio, typename M2::ratio>>;
    };

    template<typename M, std::intmax_t Exp>
    struct mag_power_impl {
        using type = magnitude<ratio_power<typename M::ratio, Exp>>;
    };

  } // namespace detail

  /// Multiply two magnitudes
  template<typename M1, typename M2>
  using mag_multiply = typename detail::mag_multiply_impl<M1, M2>::type;

  /// Divide two magnitudes
  template<typename M1, typename M2>
  using mag_divide = typename detail::mag_divide_impl<M1, M2>::type;

  /// Raise magnitude to an integer power
  template<typename M, std::intmax_t Exp>
  using mag_power = typename detail::mag_power_impl<M, Exp>::type;

  /// Check if two magnitudes are equal
  template<typename M1, typename M2>
  inline constexpr bool mag_equal_v = ratio_equal_v<typename M1::ratio, typename M2::ratio>;

  // =============================================================================
  // Forward declarations
  // =============================================================================

  template<typename T>
  struct is_unit;

  template<typename T>
  concept Unit = is_unit<std::remove_cvref_t<T>>::value;

  // =============================================================================
  // named_unit - A unit with a name, symbol, and associated quantity
  // =============================================================================

  /// A named unit represents a fundamental or coherent derived unit with
  /// a human-readable symbol and an associated quantity kind.
  ///
  /// @tparam Symbol The unit symbol as a fixed_string (e.g., "m", "kg", "s")
  /// @tparam QuantityKind The associated quantity specification or kind_of<>
  /// @tparam Mag The magnitude (scaling factor), defaults to 1
  template<fixed_string Symbol, typename QuantityKind, typename Mag = mag_one>
  struct named_unit {
      static constexpr auto symbol = Symbol;
      using quantity_kind          = QuantityKind;
      using magnitude_type         = Mag;

      /// Multiply two units
      template<typename Self, Unit Other>
      constexpr auto operator*(this Self /*self*/, Other /*other*/);

      /// Divide two units
      template<typename Self, Unit Other>
      constexpr auto operator/(this Self /*self*/, Other /*other*/);
  };

  // =============================================================================
  // scaled_unit - A unit with a different magnitude
  // =============================================================================

  /// A unit derived from another by scaling its magnitude.
  /// Used for prefixed units (kilometre = 1000 * metre) and other scaled units.
  ///
  /// @tparam Symbol The unit symbol
  /// @tparam BaseUnit The unit being scaled
  /// @tparam ScaleMag The additional scaling magnitude
  template<fixed_string Symbol, Unit BaseUnit, typename ScaleMag>
  struct scaled_unit {
      static constexpr auto symbol = Symbol;
      using base_unit              = BaseUnit;
      using quantity_kind          = typename BaseUnit::quantity_kind;
      using magnitude_type         = mag_multiply<ScaleMag, typename BaseUnit::magnitude_type>;

      /// Multiply two units
      template<typename Self, Unit Other>
      constexpr auto operator*(this Self /*self*/, Other /*other*/);

      /// Divide two units
      template<typename Self, Unit Other>
      constexpr auto operator/(this Self /*self*/, Other /*other*/);
  };

  /// Convenience alias for prefixed units using SI prefix ratios
  template<fixed_string Symbol, typename Prefix, Unit BaseUnit>
    requires Ratio<Prefix>
  using prefixed_unit = scaled_unit<Symbol, BaseUnit, magnitude<Prefix>>;

  // =============================================================================
  // derived_unit - Compound units from unit expressions
  // =============================================================================

  // Forward declaration for derived_unit (must be in units namespace, not detail)
  template<typename Num, typename Den>
  struct derived_unit;

  namespace detail {

    /// A single term in a unit expression: Unit raised to Power
    template<Unit U, std::intmax_t Power>
    struct unit_term {
        using unit                           = U;
        static constexpr std::intmax_t power = Power;
    };

    /// Numerator terms in a unit expression
    template<typename... Terms>
    struct unit_numerator {
        using types = unit_numerator;
    };

    /// Denominator terms in a unit expression
    template<typename... Terms>
    struct unit_denominator {
        using types = unit_denominator;
    };

    /// Flatten a unit into numerator/denominator terms
    template<typename U>
    struct flatten_unit {
        using num = unit_numerator<unit_term<U, 1>>;
        using den = unit_denominator<>;
    };

    // Flatten derived_unit (uses units::derived_unit from forward declaration above)
    template<typename... NumTerms, typename... DenTerms>
    struct flatten_unit<
        ::units::derived_unit<unit_numerator<NumTerms...>, unit_denominator<DenTerms...>>> {
        using num = unit_numerator<NumTerms...>;
        using den = unit_denominator<DenTerms...>;
    };

    /// Merge two unit_numerator lists
    template<typename N1, typename N2>
    struct merge_unit_numerators;

    template<typename... T1s, typename... T2s>
    struct merge_unit_numerators<unit_numerator<T1s...>, unit_numerator<T2s...>> {
        using type = unit_numerator<T1s..., T2s...>;
    };

    /// Merge two unit_denominator lists
    template<typename D1, typename D2>
    struct merge_unit_denominators;

    template<typename... T1s, typename... T2s>
    struct merge_unit_denominators<unit_denominator<T1s...>, unit_denominator<T2s...>> {
        using type = unit_denominator<T1s..., T2s...>;
    };

    /// Convert numerator to denominator (for division)
    template<typename N>
    struct unit_num_to_den;

    template<typename... Terms>
    struct unit_num_to_den<unit_numerator<Terms...>> {
        using type = unit_denominator<Terms...>;
    };

    /// Convert denominator to numerator (for division)
    template<typename D>
    struct unit_den_to_num;

    template<typename... Terms>
    struct unit_den_to_num<unit_denominator<Terms...>> {
        using type = unit_numerator<Terms...>;
    };

    /// Calculate combined magnitude from unit terms
    template<typename... Terms>
    struct combined_magnitude;

    template<>
    struct combined_magnitude<> {
        using type = mag_one;
    };

    template<typename First, typename... Rest>
    struct combined_magnitude<First, Rest...> {
        using unit_mag = mag_power<typename First::unit::magnitude_type, First::power>;
        using rest_mag = typename combined_magnitude<Rest...>::type;
        using type     = mag_multiply<unit_mag, rest_mag>;
    };

    /// Calculate magnitude for numerator terms (positive powers)
    template<typename Num>
    struct numerator_magnitude;

    template<typename... Terms>
    struct numerator_magnitude<unit_numerator<Terms...>> {
        using type = typename combined_magnitude<Terms...>::type;
    };

    /// Calculate magnitude for denominator terms (need to invert)
    template<typename Den>
    struct denominator_magnitude;

    template<typename... Terms>
    struct denominator_magnitude<unit_denominator<Terms...>> {
        // Denominator terms effectively have negative powers for magnitude
        using num_mag = typename combined_magnitude<Terms...>::type;
        using type    = mag_divide<mag_one, num_mag>;
    };

  } // namespace detail

  /// A derived unit represents a compound unit formed from unit expressions.
  /// For example: m/s, m/s², kg·m/s², etc.
  ///
  /// @tparam Num Unit numerator terms (unit_numerator<unit_term<...>...>)
  /// @tparam Den Unit denominator terms (unit_denominator<unit_term<...>...>)
  template<typename Num, typename Den>
  struct derived_unit {
      using numerator_terms   = Num;
      using denominator_terms = Den;

      // Combined magnitude from all terms
      using magnitude_type = mag_multiply<typename detail::numerator_magnitude<Num>::type,
                                          typename detail::denominator_magnitude<Den>::type>;

      /// Multiply two units
      template<typename Self, Unit Other>
      constexpr auto operator*(this Self /*self*/, Other /*other*/) {
        using self_flat  = detail::flatten_unit<std::remove_cvref_t<Self>>;
        using other_flat = detail::flatten_unit<std::remove_cvref_t<Other>>;

        using new_num = typename detail::merge_unit_numerators<typename self_flat::num,
                                                               typename other_flat::num>::type;

        using new_den = typename detail::merge_unit_denominators<typename self_flat::den,
                                                                 typename other_flat::den>::type;

        return derived_unit<new_num, new_den> {};
      }

      /// Divide two units
      template<typename Self, Unit Other>
      constexpr auto operator/(this Self /*self*/, Other /*other*/) {
        using self_flat  = detail::flatten_unit<std::remove_cvref_t<Self>>;
        using other_flat = detail::flatten_unit<std::remove_cvref_t<Other>>;

        // Division: multiply by inverse
        using new_num = typename detail::merge_unit_numerators<
            typename self_flat::num,
            typename detail::unit_den_to_num<typename other_flat::den>::type>::type;

        using new_den = typename detail::merge_unit_denominators<
            typename self_flat::den,
            typename detail::unit_num_to_den<typename other_flat::num>::type>::type;

        return derived_unit<new_num, new_den> {};
      }
  };

  // =============================================================================
  // Unit operator implementations for named_unit and scaled_unit
  // =============================================================================

  template<fixed_string Symbol, typename QuantityKind, typename Mag>
  template<typename Self, Unit Other>
  constexpr auto named_unit<Symbol, QuantityKind, Mag>::operator*(this Self /*self*/,
                                                                  Other /*other*/) {
    using self_flat  = detail::flatten_unit<std::remove_cvref_t<Self>>;
    using other_flat = detail::flatten_unit<std::remove_cvref_t<Other>>;

    using new_num = typename detail::merge_unit_numerators<typename self_flat::num,
                                                           typename other_flat::num>::type;

    using new_den = typename detail::merge_unit_denominators<typename self_flat::den,
                                                             typename other_flat::den>::type;

    return derived_unit<new_num, new_den> {};
  }

  template<fixed_string Symbol, typename QuantityKind, typename Mag>
  template<typename Self, Unit Other>
  constexpr auto named_unit<Symbol, QuantityKind, Mag>::operator/(this Self /*self*/,
                                                                  Other /*other*/) {
    using self_flat  = detail::flatten_unit<std::remove_cvref_t<Self>>;
    using other_flat = detail::flatten_unit<std::remove_cvref_t<Other>>;

    using new_num = typename detail::merge_unit_numerators<
        typename self_flat::num,
        typename detail::unit_den_to_num<typename other_flat::den>::type>::type;

    using new_den = typename detail::merge_unit_denominators<
        typename self_flat::den,
        typename detail::unit_num_to_den<typename other_flat::num>::type>::type;

    return derived_unit<new_num, new_den> {};
  }

  template<fixed_string Symbol, Unit BaseUnit, typename ScaleMag>
  template<typename Self, Unit Other>
  constexpr auto scaled_unit<Symbol, BaseUnit, ScaleMag>::operator*(this Self /*self*/,
                                                                    Other /*other*/) {
    using self_flat  = detail::flatten_unit<std::remove_cvref_t<Self>>;
    using other_flat = detail::flatten_unit<std::remove_cvref_t<Other>>;

    using new_num = typename detail::merge_unit_numerators<typename self_flat::num,
                                                           typename other_flat::num>::type;

    using new_den = typename detail::merge_unit_denominators<typename self_flat::den,
                                                             typename other_flat::den>::type;

    return derived_unit<new_num, new_den> {};
  }

  template<fixed_string Symbol, Unit BaseUnit, typename ScaleMag>
  template<typename Self, Unit Other>
  constexpr auto scaled_unit<Symbol, BaseUnit, ScaleMag>::operator/(this Self /*self*/,
                                                                    Other /*other*/) {
    using self_flat  = detail::flatten_unit<std::remove_cvref_t<Self>>;
    using other_flat = detail::flatten_unit<std::remove_cvref_t<Other>>;

    using new_num = typename detail::merge_unit_numerators<
        typename self_flat::num,
        typename detail::unit_den_to_num<typename other_flat::den>::type>::type;

    using new_den = typename detail::merge_unit_denominators<
        typename self_flat::den,
        typename detail::unit_num_to_den<typename other_flat::num>::type>::type;

    return derived_unit<new_num, new_den> {};
  }

  // =============================================================================
  // Unit type trait
  // =============================================================================

  namespace detail {

    /// Check if T has a magnitude_type (essential for any unit)
    template<typename T, typename = void>
    struct has_magnitude_type : std::false_type {};

    template<typename T>
    struct has_magnitude_type<T, std::void_t<typename T::magnitude_type>> : std::true_type {};

    /// Check if T is a derived_unit (has numerator/denominator terms)
    template<typename T, typename = void>
    struct is_derived_unit_impl : std::false_type {};

    template<typename T>
    struct is_derived_unit_impl<
        T,
        std::void_t<typename T::numerator_terms, typename T::denominator_terms>>
        : std::true_type {};

    /// Check if T is a named unit (has symbol)
    template<typename T, typename = void>
    struct has_symbol : std::false_type {};

    template<typename T>
    struct has_symbol<T, std::void_t<decltype(T::symbol)>> : std::true_type {};

  } // namespace detail

  /// Type trait to check if T is a unit
  /// A type is a unit if it has a magnitude_type (and is either named or derived)
  template<typename T>
  struct is_unit
      : std::bool_constant<detail::has_magnitude_type<std::remove_cvref_t<T>>::value
                           && (detail::has_symbol<std::remove_cvref_t<T>>::value
                               || detail::is_derived_unit_impl<std::remove_cvref_t<T>>::value)> {};

  template<typename T>
  inline constexpr bool is_unit_v = is_unit<T>::value;

  // =============================================================================
  // Unit concepts
  // =============================================================================

  /// Concept for named units (have a symbol and quantity_kind)
  template<typename T>
  concept NamedUnit = Unit<T> && requires {
    { T::symbol } -> std::convertible_to<decltype(T::symbol)>;
    typename T::quantity_kind;
  };

  /// Concept for derived units (compound expressions)
  template<typename T>
  concept DerivedUnit = Unit<T> && requires {
    typename T::numerator_terms;
    typename T::denominator_terms;
  };

  /// Concept for scaled units
  template<typename T>
  concept ScaledUnit = Unit<T> && requires { typename T::base_unit; };

  // =============================================================================
  // Unit accessors
  // =============================================================================

  /// Get the magnitude of a unit
  template<Unit U>
  using get_magnitude = typename U::magnitude_type;

  /// Get the magnitude value as a floating point
  template<Unit U, typename T = double>
    requires std::is_floating_point_v<T>
  [[nodiscard]]
  consteval auto magnitude_value() -> T {
    return get_magnitude<U>::template value<T>();
  }

  // =============================================================================
  // Conversion factor computation
  // =============================================================================

  /// Compute the conversion factor from one unit to another.
  /// The result is the multiplier to convert a value in FromUnit to ToUnit.
  /// Example: conversion_factor<kilometre_t, metre_t>() == 1000.0
  ///
  /// @tparam FromUnit The source unit
  /// @tparam ToUnit The target unit
  /// @return The conversion factor as a floating-point value
  template<Unit FromUnit, Unit ToUnit, typename T = double>
    requires std::is_floating_point_v<T>
  [[nodiscard]]
  consteval auto conversion_factor() -> T {
    // Factor = from_magnitude / to_magnitude
    using from_mag   = get_magnitude<FromUnit>;
    using to_mag     = get_magnitude<ToUnit>;
    using factor_mag = mag_divide<from_mag, to_mag>;
    return factor_mag::template value<T>();
  }

  /// Compute the conversion factor as a ratio type (for exact arithmetic)
  template<Unit FromUnit, Unit ToUnit>
  using conversion_factor_t = mag_divide<get_magnitude<FromUnit>, get_magnitude<ToUnit>>;

// =============================================================================
// Convenience macro for defining named units
// =============================================================================

/// Helper macro for defining named units with type and constexpr instance.
#define UNITS_NAMED_UNIT(name, symbol_str, qty_kind)                    \
  struct name##_t final : ::units::named_unit<symbol_str, qty_kind> {}; \
  inline constexpr name##_t name {}

/// Helper macro for defining named units with custom magnitude.
#define UNITS_NAMED_UNIT_MAG(name, symbol_str, qty_kind, mag_type)                \
  struct name##_t final : ::units::named_unit<symbol_str, qty_kind, mag_type> {}; \
  inline constexpr name##_t name {}

/// Helper macro for defining scaled units.
#define UNITS_SCALED_UNIT(name, symbol_str, base_unit_type, scale_mag)                    \
  struct name##_t final : ::units::scaled_unit<symbol_str, base_unit_type, scale_mag> {}; \
  inline constexpr name##_t name {}

/// Helper macro for defining prefixed units.
#define UNITS_PREFIXED_UNIT(name, symbol_str, prefix, base_unit_type)                    \
  struct name##_t final : ::units::prefixed_unit<symbol_str, prefix, base_unit_type> {}; \
  inline constexpr name##_t name {}

} // namespace units
