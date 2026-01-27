// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>
#include <cstdint>

#include <concepts>
#include <numbers>
#include <type_traits>

#include <units/core/quantity.hpp>
#include <units/core/reference.hpp>
#include <units/core/unit.hpp>

namespace units {

  // =============================================================================
  // Basic mathematical functions that preserve units
  // =============================================================================

  /// Absolute value of a quantity
  template<Reference Ref, typename Rep>
  [[nodiscard]]
  constexpr auto abs(const quantity<Ref, Rep> &q) noexcept -> quantity<Ref, Rep> {
    if consteval {
      return quantity<Ref, Rep> { q.value() < Rep { 0 } ? -q.value() : q.value() };
    }
    else {
      return quantity<Ref, Rep> { std::abs(q.value()) };
    }
  }

  /// Floor of a quantity (rounds toward negative infinity)
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto floor(const quantity<Ref, Rep> &q) noexcept -> quantity<Ref, Rep> {
    if consteval {
      // Compile-time floor approximation
      auto val       = q.value();
      auto truncated = static_cast<Rep>(static_cast<std::int64_t>(val));
      return quantity<Ref, Rep> { val < truncated ? truncated - Rep { 1 } : truncated };
    }
    else {
      return quantity<Ref, Rep> { std::floor(q.value()) };
    }
  }

  /// Ceiling of a quantity (rounds toward positive infinity)
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto ceil(const quantity<Ref, Rep> &q) noexcept -> quantity<Ref, Rep> {
    if consteval {
      // Compile-time ceil approximation
      auto val       = q.value();
      auto truncated = static_cast<Rep>(static_cast<std::int64_t>(val));
      return quantity<Ref, Rep> { val > truncated ? truncated + Rep { 1 } : truncated };
    }
    else {
      return quantity<Ref, Rep> { std::ceil(q.value()) };
    }
  }

  /// Round a quantity to nearest integer value
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto round(const quantity<Ref, Rep> &q) noexcept -> quantity<Ref, Rep> {
    if consteval {
      // Compile-time round approximation (round half away from zero)
      auto val       = q.value();
      auto truncated = static_cast<Rep>(static_cast<std::int64_t>(val));
      auto frac      = val - truncated;
      if (frac >= Rep { 0.5 }) {
        return quantity<Ref, Rep> { truncated + Rep { 1 } };
      }
      else if (frac <= Rep { -0.5 }) {
        return quantity<Ref, Rep> { truncated - Rep { 1 } };
      }
      return quantity<Ref, Rep> { truncated };
    }
    else {
      return quantity<Ref, Rep> { std::round(q.value()) };
    }
  }

  /// Truncate a quantity (rounds toward zero)
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto trunc(const quantity<Ref, Rep> &q) noexcept -> quantity<Ref, Rep> {
    if consteval {
      return quantity<Ref, Rep> { static_cast<Rep>(static_cast<std::int64_t>(q.value())) };
    }
    else {
      return quantity<Ref, Rep> { std::trunc(q.value()) };
    }
  }

  /// Floating-point remainder of quantity division
  template<Reference Ref, typename Rep1, typename Rep2>
    requires std::floating_point<Rep1> && std::floating_point<Rep2>
  [[nodiscard]]
  constexpr auto fmod(const quantity<Ref, Rep1> &x, const quantity<Ref, Rep2> &y) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    if consteval {
      auto xv   = static_cast<common_rep>(x.value());
      auto yv   = static_cast<common_rep>(y.value());
      auto quot = static_cast<std::int64_t>(xv / yv);
      return quantity<Ref, common_rep> { xv - static_cast<common_rep>(quot) * yv };
    }
    else {
      return quantity<Ref, common_rep> { std::fmod(static_cast<common_rep>(x.value()),
                                                   static_cast<common_rep>(y.value())) };
    }
  }

  // =============================================================================
  // Power and root functions that transform dimensions
  // =============================================================================

  namespace detail {

    /// Transform unit term power by multiplying
    template<typename Term, std::intmax_t Factor>
    struct scale_unit_term_power {
        using type = unit_term<typename Term::unit, Term::power * Factor>;
    };

    /// Transform all terms in a unit_numerator
    template<typename Num, std::intmax_t Factor>
    struct scale_numerator_powers;

    template<typename... Terms, std::intmax_t Factor>
    struct scale_numerator_powers<unit_numerator<Terms...>, Factor> {
        using type = unit_numerator<typename scale_unit_term_power<Terms, Factor>::type...>;
    };

    /// Transform all terms in a unit_denominator
    template<typename Den, std::intmax_t Factor>
    struct scale_denominator_powers;

    template<typename... Terms, std::intmax_t Factor>
    struct scale_denominator_powers<unit_denominator<Terms...>, Factor> {
        using type = unit_denominator<typename scale_unit_term_power<Terms, Factor>::type...>;
    };

    /// Create a powered derived unit from a base unit
    template<Unit U, std::intmax_t N>
    struct powered_unit_impl {
        // For a simple unit, create derived_unit with single term raised to power N
        using flat    = flatten_unit<U>;
        using new_num = typename scale_numerator_powers<typename flat::num, N>::type;
        using new_den = typename scale_denominator_powers<typename flat::den, N>::type;
        using type    = derived_unit<new_num, new_den>;
    };

    /// Get the powered version of a unit
    template<Unit U, std::intmax_t N>
    using powered_unit = typename powered_unit_impl<U, N>::type;

    /// Check if a derived unit's powers are all divisible by a factor
    template<typename Num, typename Den, std::intmax_t Factor>
    struct powers_divisible_by;

    template<std::intmax_t Factor>
    struct powers_divisible_by<unit_numerator<>, unit_denominator<>, Factor> {
        static constexpr bool value = true;
    };

    template<typename First, typename... RestNum, typename... DenTerms, std::intmax_t Factor>
    struct powers_divisible_by<unit_numerator<First, RestNum...>,
                               unit_denominator<DenTerms...>,
                               Factor> {
        static constexpr bool value = (First::power % Factor == 0)
                                   && powers_divisible_by<unit_numerator<RestNum...>,
                                                          unit_denominator<DenTerms...>,
                                                          Factor>::value;
    };

    template<typename First, typename... RestDen, std::intmax_t Factor>
    struct powers_divisible_by<unit_numerator<>, unit_denominator<First, RestDen...>, Factor> {
        static constexpr bool value
            = (First::power % Factor == 0)
           && powers_divisible_by<unit_numerator<>, unit_denominator<RestDen...>, Factor>::value;
    };

    /// Divide unit term power
    template<typename Term, std::intmax_t Divisor>
    struct divide_unit_term_power {
        using type = unit_term<typename Term::unit, Term::power / Divisor>;
    };

    /// Divide all term powers in numerator
    template<typename Num, std::intmax_t Divisor>
    struct divide_numerator_powers;

    template<typename... Terms, std::intmax_t Divisor>
    struct divide_numerator_powers<unit_numerator<Terms...>, Divisor> {
        using type = unit_numerator<typename divide_unit_term_power<Terms, Divisor>::type...>;
    };

    /// Divide all term powers in denominator
    template<typename Den, std::intmax_t Divisor>
    struct divide_denominator_powers;

    template<typename... Terms, std::intmax_t Divisor>
    struct divide_denominator_powers<unit_denominator<Terms...>, Divisor> {
        using type = unit_denominator<typename divide_unit_term_power<Terms, Divisor>::type...>;
    };

    /// Create a root derived unit
    template<Unit U, std::intmax_t N>
    struct root_unit_impl {
        using flat    = flatten_unit<U>;
        using new_num = typename divide_numerator_powers<typename flat::num, N>::type;
        using new_den = typename divide_denominator_powers<typename flat::den, N>::type;
        using type    = derived_unit<new_num, new_den>;
    };

    template<Unit U, std::intmax_t N>
    using root_unit = typename root_unit_impl<U, N>::type;

    /// Check if a unit can be square-rooted (all powers even)
    template<Unit U>
    struct is_sqrt_compatible {
        using flat = flatten_unit<U>;
        static constexpr bool value
            = powers_divisible_by<typename flat::num, typename flat::den, 2>::value;
    };

    /// Check if a unit can be cube-rooted (all powers divisible by 3)
    template<Unit U>
    struct is_cbrt_compatible {
        using flat = flatten_unit<U>;
        static constexpr bool value
            = powers_divisible_by<typename flat::num, typename flat::den, 3>::value;
    };

  } // namespace detail

  /// Raise a quantity to an integer power
  /// pow<2>(5_m) == 25_m²
  /// pow<0>(5_m) returns dimensionless quantity with value 1
  /// pow<-1>(5_m) returns quantity with inverse unit
  template<std::intmax_t N, Reference Ref, typename Rep>
  [[nodiscard]]
  constexpr auto pow(const quantity<Ref, Rep> &q) noexcept {
    using unit = get_unit<Ref>;
    using spec = get_quantity_spec<Ref>;

    // For N=0, return dimensionless quantity with value 1
    if constexpr (N == 0) {
      using new_unit = detail::powered_unit<unit, 0>;
      using new_ref  = reference<spec, new_unit>;
      return quantity<new_ref, Rep> { Rep { 1 } };
    }
    // For N=1, return the same quantity
    else if constexpr (N == 1) {
      return q;
    }
    // For N=2, use multiplication for better precision
    else if constexpr (N == 2) {
      return q * q;
    }
    // For N=-1, return 1/q with proper inverse unit
    else if constexpr (N == -1) {
      using new_unit = detail::powered_unit<unit, -1>;
      using new_ref  = reference<spec, new_unit>;
      return quantity<new_ref, Rep> { Rep { 1 } / q.value() };
    }
    // General case (N >= 3 or N <= -2)
    // Note: Unit powers are correctly computed, but quantity_spec is preserved
    // as-is. For full dimensional type tracking, use repeated multiplication.
    else {
      using new_unit = detail::powered_unit<unit, N>;
      using new_ref  = reference<spec, new_unit>;

      if consteval {
        // Compile-time power calculation
        Rep            result = Rep { 1 };
        Rep            base   = q.value();
        constexpr auto abs_n  = N < 0 ? -N : N;

        for (std::intmax_t i = 0; i < abs_n; ++i) {
          result *= base;
        }

        if constexpr (N < 0) {
          result = Rep { 1 } / result;
        }

        return quantity<new_ref, Rep> { result };
      }
      else {
        return quantity<new_ref, Rep> { std::pow(q.value(), static_cast<Rep>(N)) };
      }
    }
  }

  /// Square root of a quantity
  /// sqrt(25_m2) == 5_m
  /// Only valid when all dimension powers are even
  template<Reference Ref, typename Rep>
    requires detail::is_sqrt_compatible<get_unit<Ref>>::value
  [[nodiscard]]
  constexpr auto sqrt(const quantity<Ref, Rep> &q) noexcept {
    using unit     = get_unit<Ref>;
    using spec     = get_quantity_spec<Ref>;
    using new_unit = detail::root_unit<unit, 2>;
    using new_ref  = reference<spec, new_unit>; // Simplified

    if consteval {
      // Newton-Raphson square root at compile time
      Rep x = q.value();
      if (x <= Rep { 0 }) {
        return quantity<new_ref, Rep> { Rep { 0 } };
      }

      Rep guess = x;
      for (int i = 0; i < 50; ++i) { // Usually converges much faster
        Rep next = (guess + x / guess) / Rep { 2 };
        if (next == guess) {
          break;
        }
        guess = next;
      }
      return quantity<new_ref, Rep> { guess };
    }
    else {
      return quantity<new_ref, Rep> { std::sqrt(q.value()) };
    }
  }

  /// Cube root of a quantity
  /// cbrt(27_m3) == 3_m
  /// cbrt(-8_m3) == -2_m (negative inputs are valid for cube root)
  /// Only valid when all dimension powers are divisible by 3
  template<Reference Ref, typename Rep>
    requires detail::is_cbrt_compatible<get_unit<Ref>>::value
  [[nodiscard]]
  constexpr auto cbrt(const quantity<Ref, Rep> &q) noexcept {
    using unit     = get_unit<Ref>;
    using spec     = get_quantity_spec<Ref>;
    using new_unit = detail::root_unit<unit, 3>;
    using new_ref  = reference<spec, new_unit>;

    if consteval {
      // Newton-Raphson cube root at compile time
      Rep x = q.value();
      if (x == Rep { 0 }) {
        return quantity<new_ref, Rep> { Rep { 0 } };
      }

      // Handle negative inputs: cbrt(-x) = -cbrt(x)
      bool is_negative = x < Rep { 0 };
      if (is_negative) {
        x = -x;
      }

      Rep guess = x;
      for (int i = 0; i < 50; ++i) {
        Rep next = (Rep { 2 } * guess + x / (guess * guess)) / Rep { 3 };
        if (next == guess) {
          break;
        }
        guess = next;
      }

      return quantity<new_ref, Rep> { is_negative ? -guess : guess };
    }
    else {
      return quantity<new_ref, Rep> { std::cbrt(q.value()) };
    }
  }

  // =============================================================================
  // Trigonometric functions for angular quantities
  // =============================================================================

  namespace detail {

    /// Check if a quantity is dimensionless
    template<typename Ref>
    struct is_dimensionless : std::false_type {};

    // A unit is dimensionless if its magnitude is 1 and it has no dimension
    // For now, we'll check by looking at the unit structure
    // This is a simplified check

  } // namespace detail

  namespace detail {

    /// Compile-time fmod implementation for angle normalization
    template<typename Rep>
    constexpr auto ct_fmod(Rep x, Rep y) noexcept -> Rep {
      auto quot = static_cast<std::int64_t>(x / y);
      return x - static_cast<Rep>(quot) * y;
    }

    /// Normalize angle to [-pi, pi] range efficiently
    template<typename Rep>
    constexpr auto normalize_angle(Rep x) noexcept -> Rep {
      constexpr Rep pi     = Rep { std::numbers::pi };
      constexpr Rep two_pi = Rep { 2 } * pi;

      // Use fmod-style reduction for efficiency (avoids potential infinite loop)
      x = ct_fmod(x, two_pi);

      // Adjust to [-pi, pi]
      if (x > pi) {
        x -= two_pi;
      }
      else if (x < -pi) {
        x += two_pi;
      }
      return x;
    }

  } // namespace detail

  /// Sine of an angle (assumes input is in radians)
  /// Returns a dimensionless value
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto sin(const quantity<Ref, Rep> &angle) noexcept -> Rep {
    if consteval {
      // Taylor series for sin at compile time
      Rep x = detail::normalize_angle(angle.value());

      Rep result = x;
      Rep term   = x;
      for (int n = 1; n < 20; ++n) {
        term   *= -x * x / static_cast<Rep>((2 * n) * (2 * n + 1));
        result += term;
      }
      return result;
    }
    else {
      return std::sin(angle.value());
    }
  }

  /// Cosine of an angle (assumes input is in radians)
  /// Returns a dimensionless value
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto cos(const quantity<Ref, Rep> &angle) noexcept -> Rep {
    if consteval {
      // Taylor series for cos at compile time
      Rep x = detail::normalize_angle(angle.value());

      Rep result = Rep { 1 };
      Rep term   = Rep { 1 };
      for (int n = 1; n < 20; ++n) {
        term   *= -x * x / static_cast<Rep>((2 * n - 1) * (2 * n));
        result += term;
      }
      return result;
    }
    else {
      return std::cos(angle.value());
    }
  }

  /// Tangent of an angle (assumes input is in radians)
  /// Returns a dimensionless value
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto tan(const quantity<Ref, Rep> &angle) noexcept -> Rep {
    if consteval {
      return sin(angle) / cos(angle);
    }
    else {
      return std::tan(angle.value());
    }
  }

  /// Arc sine - returns angle in radians (as raw value)
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto asin(Rep x) noexcept -> Rep {
    if consteval {
      // Taylor series approximation for asin
      // asin(x) = x + x³/6 + 3x⁵/40 + 15x⁷/336 + ...
      if (x < Rep { -1 } || x > Rep { 1 }) {
        return Rep { 0 }; // Domain error
      }
      Rep result = x;
      Rep term   = x;
      Rep x2     = x * x;
      for (int n = 1; n < 25; ++n) {
        term *= x2 * static_cast<Rep>((2 * n - 1) * (2 * n - 1))
              / static_cast<Rep>((2 * n) * (2 * n + 1));
        result += term;
      }
      return result;
    }
    else {
      return std::asin(x);
    }
  }

  /// Arc cosine - returns angle in radians (as raw value)
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto acos(Rep x) noexcept -> Rep {
    if consteval {
      constexpr Rep pi_2 = Rep { std::numbers::pi / 2 };
      return pi_2 - asin(x);
    }
    else {
      return std::acos(x);
    }
  }

  /// Arc tangent - returns angle in radians (as raw value)
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto atan(Rep x) noexcept -> Rep {
    if consteval {
      // For |x| <= 1, use Taylor series
      // atan(x) = x - x³/3 + x⁵/5 - x⁷/7 + ...
      if (x >= Rep { -1 } && x <= Rep { 1 }) {
        Rep result = x;
        Rep term   = x;
        Rep x2     = x * x;
        for (int n = 1; n < 50; ++n) {
          term   *= -x2;
          result += term / static_cast<Rep>((2 * n) + 1);
        }
        return result;
      }
      else {
        // Use atan(x) = pi/2 - atan(1/x) for |x| > 1
        constexpr Rep pi_2 = Rep { std::numbers::pi / 2 };
        if (x > Rep { 1 }) {
          return pi_2 - atan(Rep { 1 } / x);
        }
        else {
          return -pi_2 - atan(Rep { 1 } / x);
        }
      }
    }
    else {
      return std::atan(x);
    }
  }

  /// Arc tangent of y/x with proper quadrant handling
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto atan2(Rep y, Rep x) noexcept -> Rep {
    if consteval {
      constexpr Rep pi   = Rep { std::numbers::pi };
      constexpr Rep pi_2 = pi / Rep { 2 };

      if (x > Rep { 0 }) {
        return atan(y / x);
      }
      else if (x < Rep { 0 } && y >= Rep { 0 }) {
        return atan(y / x) + pi;
      }
      else if (x < Rep { 0 } && y < Rep { 0 }) {
        return atan(y / x) - pi;
      }
      else if (x == Rep { 0 } && y > Rep { 0 }) {
        return pi_2;
      }
      else if (x == Rep { 0 } && y < Rep { 0 }) {
        return -pi_2;
      }
      return Rep { 0 }; // x == 0 && y == 0
    }
    else {
      return std::atan2(y, x);
    }
  }

  // =============================================================================
  // Hyperbolic functions
  // =============================================================================

  /// Hyperbolic sine
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto sinh(const quantity<Ref, Rep> &x) noexcept -> Rep {
    if consteval {
      // sinh(x) = (e^x - e^-x) / 2
      // Use Taylor series: x + x³/3! + x⁵/5! + ...
      Rep val    = x.value();
      Rep result = val;
      Rep term   = val;
      for (int n = 1; n < 20; ++n) {
        term   *= val * val / static_cast<Rep>((2 * n) * (2 * n + 1));
        result += term;
      }
      return result;
    }
    else {
      return std::sinh(x.value());
    }
  }

  /// Hyperbolic cosine
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto cosh(const quantity<Ref, Rep> &x) noexcept -> Rep {
    if consteval {
      // cosh(x) = (e^x + e^-x) / 2
      // Use Taylor series: 1 + x²/2! + x⁴/4! + ...
      Rep val    = x.value();
      Rep result = Rep { 1 };
      Rep term   = Rep { 1 };
      for (int n = 1; n < 20; ++n) {
        term   *= val * val / static_cast<Rep>((2 * n - 1) * (2 * n));
        result += term;
      }
      return result;
    }
    else {
      return std::cosh(x.value());
    }
  }

  /// Hyperbolic tangent
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto tanh(const quantity<Ref, Rep> &x) noexcept -> Rep {
    if consteval {
      return sinh(x) / cosh(x);
    }
    else {
      return std::tanh(x.value());
    }
  }

  // =============================================================================
  // Exponential and logarithmic functions (for dimensionless quantities)
  // =============================================================================

  /// Natural exponential (e^x) - for dimensionless values
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto exp(Rep x) noexcept -> Rep {
    if consteval {
      // Taylor series: 1 + x + x²/2! + x³/3! + ...
      Rep result = Rep { 1 };
      Rep term   = Rep { 1 };
      for (int n = 1; n < 30; ++n) {
        term   *= x / static_cast<Rep>(n);
        result += term;
        if (term < Rep { 1e-15 } && term > Rep { -1e-15 }) {
          break;
        }
      }
      return result;
    }
    else {
      return std::exp(x);
    }
  }

  /// Natural logarithm - for dimensionless values
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto log(Rep x) noexcept -> Rep {
    if consteval {
      if (x <= Rep { 0 }) {
        return Rep { 0 }; // Domain error
      }

      // Use ln(x) = 2 * arctanh((x-1)/(x+1)) for x > 0
      // Or reduce to ln(x) = ln(m * 2^e) = ln(m) + e*ln(2)
      Rep result = Rep { 0 };

      // Normalize x to [1, 2) and track exponent
      int exp_count = 0;
      while (x >= Rep { 2 }) {
        x /= Rep { 2 };
        ++exp_count;
      }
      while (x < Rep { 1 }) {
        x *= Rep { 2 };
        --exp_count;
      }

      // Now x is in [1, 2), use Taylor series around 1
      // ln(1+y) = y - y²/2 + y³/3 - ...
      Rep y    = x - Rep { 1 };
      Rep term = y;
      result   = y;
      for (int n = 2; n < 50; ++n) {
        term   *= -y;
        result += term / static_cast<Rep>(n);
      }

      // Add back the exponent part
      constexpr Rep ln2  = Rep { std::numbers::ln2 };
      result            += static_cast<Rep>(exp_count) * ln2;

      return result;
    }
    else {
      return std::log(x);
    }
  }

  /// Base-10 logarithm - for dimensionless values
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto log10(Rep x) noexcept -> Rep {
    if consteval {
      constexpr Rep ln10 = Rep { std::numbers::ln10 };
      return log(x) / ln10;
    }
    else {
      return std::log10(x);
    }
  }

  /// Base-2 logarithm - for dimensionless values
  template<typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto log2(Rep x) noexcept -> Rep {
    if consteval {
      constexpr Rep ln2 = Rep { std::numbers::ln2 };
      return log(x) / ln2;
    }
    else {
      return std::log2(x);
    }
  }

  // =============================================================================
  // Min/Max functions
  // =============================================================================

  /// Minimum of two quantities
  template<Reference Ref, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto min(const quantity<Ref, Rep1> &a, const quantity<Ref, Rep2> &b) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    return quantity<Ref, common_rep> { a.value() < b.value() ? static_cast<common_rep>(a.value())
                                                             : static_cast<common_rep>(b.value()) };
  }

  /// Maximum of two quantities
  template<Reference Ref, typename Rep1, typename Rep2>
  [[nodiscard]]
  constexpr auto max(const quantity<Ref, Rep1> &a, const quantity<Ref, Rep2> &b) noexcept {
    using common_rep = std::common_type_t<Rep1, Rep2>;
    return quantity<Ref, common_rep> { a.value() > b.value() ? static_cast<common_rep>(a.value())
                                                             : static_cast<common_rep>(b.value()) };
  }

  /// Clamp a quantity to a range
  template<Reference Ref, typename Rep>
  [[nodiscard]]
  constexpr auto clamp(const quantity<Ref, Rep> &val,
                       const quantity<Ref, Rep> &lo,
                       const quantity<Ref, Rep> &hi) noexcept -> quantity<Ref, Rep> {
    if (val.value() < lo.value()) {
      return lo;
    }
    if (val.value() > hi.value()) {
      return hi;
    }
    return val;
  }

  // =============================================================================
  // Comparison helpers
  // =============================================================================

  /// Check if a quantity is approximately zero
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto is_zero(const quantity<Ref, Rep> &q, Rep epsilon = Rep { 1e-10 }) noexcept
      -> bool {
    return q.value() > -epsilon && q.value() < epsilon;
  }

  /// Check if two quantities are approximately equal
  template<Reference Ref, typename Rep>
    requires std::floating_point<Rep>
  [[nodiscard]]
  constexpr auto approx_equal(const quantity<Ref, Rep> &a,
                              const quantity<Ref, Rep> &b,
                              Rep                       rel_epsilon = Rep { 1e-9 },
                              Rep abs_epsilon = Rep { 1e-12 }) noexcept -> bool {
    auto diff    = abs(a - b).value();
    auto max_val = max(abs(a), abs(b)).value();
    return diff <= abs_epsilon || diff <= max_val * rel_epsilon;
  }

} // namespace units
