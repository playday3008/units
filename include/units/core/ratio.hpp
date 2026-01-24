// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include <concepts>
#include <limits>
#include <type_traits>

namespace units {

  // =============================================================================
  // Extended integer type support
  // =============================================================================

  // Detect __int128 support (GCC/Clang on 64-bit platforms)
  // NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if defined(__SIZEOF_INT128__) && !defined(UNITS_NO_INT128)
#  define UNITS_HAS_INT128 1
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
  using ratio_int_t = __int128;
#  pragma GCC diagnostic pop
#else
#  define UNITS_HAS_INT128 0
  using ratio_int_t = std::intmax_t;
#endif
  // NOLINTEND(cppcoreguidelines-macro-usage)

  // =============================================================================
  // Compile-time arithmetic utilities
  // =============================================================================

  namespace detail {

    // Numeric limits for ratio_int_t (std::numeric_limits doesn't support __int128)
    template<typename T>
    struct ratio_limits;

    template<>
    struct ratio_limits<std::intmax_t> {
        static constexpr auto max() noexcept -> std::intmax_t {
          return std::numeric_limits<std::intmax_t>::max();
        }
        static constexpr auto min() noexcept -> std::intmax_t {
          return std::numeric_limits<std::intmax_t>::min();
        }
    };

#if UNITS_HAS_INT128
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
    template<>
    struct ratio_limits<__int128> {
        static constexpr auto max() noexcept -> __int128 {
          return std::numeric_limits<__int128>::max();
        }
        static constexpr auto min() noexcept -> __int128 {
          return std::numeric_limits<__int128>::min();
        }
    };
#  pragma GCC diagnostic pop
#endif

    /// Compile-time absolute value
    consteval auto abs(ratio_int_t value) noexcept -> ratio_int_t {
      return value < 0 ? -value : value;
    }

    /// Compile-time sign: returns -1, 0, or 1
    consteval auto sign(ratio_int_t value) noexcept -> ratio_int_t {
      return static_cast<ratio_int_t>(value > 0) - static_cast<ratio_int_t>(value < 0);
    }

    /// Compile-time GCD using Euclidean algorithm
    consteval auto gcd(ratio_int_t a, ratio_int_t b) noexcept -> ratio_int_t {
      a = abs(a);
      b = abs(b);
      while (b != 0) {
        const auto temp = b;
        b               = a % b;
        a               = temp;
      }
      return a;
    }

    /// Compile-time LCM
    consteval auto lcm(ratio_int_t a, ratio_int_t b) noexcept -> ratio_int_t {
      if (a == 0 || b == 0) {
        return 0;
      }
      return abs(a) / gcd(a, b) * abs(b);
    }

    /// Check if multiplication would overflow
    consteval auto mul_overflow(ratio_int_t a, ratio_int_t b) noexcept -> bool {
      if (a == 0 || b == 0) {
        return false;
      }

      constexpr auto max_val = ratio_limits<ratio_int_t>::max();
      constexpr auto min_val = ratio_limits<ratio_int_t>::min();

      if (a > 0) {
        if (b > 0) {
          return a > max_val / b;
        }
        return b < min_val / a;
      }
      if (b > 0) {
        return a < min_val / b;
      }
      return a != 0 && b < max_val / a;
    }

    /// Check if addition would overflow
    consteval auto add_overflow(ratio_int_t a, ratio_int_t b) noexcept -> bool {
      constexpr auto max_val = ratio_limits<ratio_int_t>::max();
      constexpr auto min_val = ratio_limits<ratio_int_t>::min();

      if (b > 0 && a > max_val - b) {
        return true;
      }
      if (b < 0 && a < min_val - b) {
        return true;
      }
      return false;
    }

    // NOTE: The following consteval functions use `throw "string"` for compile-time error
    // reporting. These are never thrown at runtime - they cause compilation to fail with
    // a descriptive error message when overflow is detected during constant evaluation.
    // NOLINTBEGIN(hicpp-exception-baseclass)

    /// Safe multiplication with overflow check (consteval context only)
    consteval auto safe_mul(ratio_int_t a, ratio_int_t b) -> ratio_int_t {
      if (mul_overflow(a, b)) {
        throw "Overflow in ratio multiplication";
      }
      return a * b;
    }

    /// Safe addition with overflow check (consteval context only)
    consteval auto safe_add(ratio_int_t a, ratio_int_t b) -> ratio_int_t {
      if (add_overflow(a, b)) {
        throw "Overflow in ratio addition";
      }
      return a + b;
    }

    /// Compile-time integer power
    consteval auto power(ratio_int_t base, ratio_int_t exp) -> ratio_int_t {
      if (exp < 0) {
        throw "Negative exponent not supported for integer power";
      }
      if (exp == 0) {
        return 1;
      }

      ratio_int_t result = 1;
      // NOLINTBEGIN(hicpp-signed-bitwise)
      while (exp > 0) {
        if ((exp & 1) != 0) {
          result = safe_mul(result, base);
        }
        if (exp > 1) {
          base = safe_mul(base, base);
        }
        exp >>= 1;
      }
      // NOLINTEND(hicpp-signed-bitwise)
      return result;
    }

    // NOLINTEND(hicpp-exception-baseclass)

  } // namespace detail

  // =============================================================================
  // ratio - Compile-time rational number
  // =============================================================================

  /// A compile-time rational number with automatic GCD normalization and
  /// sign normalization (denominator is always positive).
  ///
  /// Improves on std::ratio with better overflow handling via `if consteval`.
  /// Uses __int128 on GCC/Clang for full SI prefix support (up to 10^30).
  template<ratio_int_t Num, ratio_int_t Den = 1>
  struct ratio {
      static_assert(Den != 0, "Denominator cannot be zero");

    private:
      static constexpr auto gcd_val  = detail::gcd(Num, Den);
      static constexpr auto sign_val = detail::sign(Den);

    public:
      /// Numerator after GCD reduction and sign normalization
      static constexpr ratio_int_t num = sign_val * Num / gcd_val;

      /// Denominator after GCD reduction (always positive)
      static constexpr ratio_int_t den = detail::abs(Den) / gcd_val;

      /// The normalized ratio type
      using type = ratio<num, den>;

      /// Convert to floating point value
      template<typename T = double>
        requires std::is_floating_point_v<T>
      static consteval auto value() noexcept -> T {
        return static_cast<T>(num) / static_cast<T>(den);
      }

      /// Equality comparison at type level
      template<ratio_int_t N2, ratio_int_t D2>
      static consteval auto equals(ratio<N2, D2> /*other*/) noexcept -> bool {
        return num == ratio<N2, D2>::num && den == ratio<N2, D2>::den;
      }
  };

  // =============================================================================
  // ratio type aliases for common values
  // =============================================================================

  using ratio_zero = ratio<0, 1>;
  using ratio_one  = ratio<1, 1>;
  using ratio_half = ratio<1, 2>;

  // =============================================================================
  // ratio comparison type traits
  // =============================================================================

  /// Check if two ratios are equal (after normalization)
  template<typename R1, typename R2>
  struct ratio_equal : std::false_type {};

  template<ratio_int_t N1, ratio_int_t D1, ratio_int_t N2, ratio_int_t D2>
  struct ratio_equal<ratio<N1, D1>, ratio<N2, D2>>
      : std::bool_constant<ratio<N1, D1>::num == ratio<N2, D2>::num
                           && ratio<N1, D1>::den == ratio<N2, D2>::den> {};

  template<typename R1, typename R2>
  inline constexpr bool ratio_equal_v = ratio_equal<R1, R2>::value;

  /// Check if R1 < R2 (cross multiplication to avoid overflow where possible)
  template<typename R1, typename R2>
  struct ratio_less;

  template<ratio_int_t N1, ratio_int_t D1, ratio_int_t N2, ratio_int_t D2>
  struct ratio_less<ratio<N1, D1>, ratio<N2, D2>> {
    private:
      // Use normalized values
      using r1 = typename ratio<N1, D1>::type;
      using r2 = typename ratio<N2, D2>::type;

      // Cross multiply: N1/D1 < N2/D2 iff N1*D2 < N2*D1
      // Use GCD to reduce overflow risk
      static constexpr auto g = detail::gcd(r1::den, r2::den);

      // Compute lhs = r1::num * (r2::den / g)
      // Compute rhs = r2::num * (r1::den / g)
      static constexpr auto lhs = r1::num * (r2::den / g);
      static constexpr auto rhs = r2::num * (r1::den / g);

    public:
      static constexpr bool value = lhs < rhs;
  };

  template<typename R1, typename R2>
  inline constexpr bool ratio_less_v = ratio_less<R1, R2>::value;

  /// Check if R1 <= R2
  template<typename R1, typename R2>
  inline constexpr bool ratio_less_equal_v = ratio_less_v<R1, R2> || ratio_equal_v<R1, R2>;

  /// Check if R1 > R2
  template<typename R1, typename R2>
  inline constexpr bool ratio_greater_v = ratio_less_v<R2, R1>;

  /// Check if R1 >= R2
  template<typename R1, typename R2>
  inline constexpr bool ratio_greater_equal_v = ratio_greater_v<R1, R2> || ratio_equal_v<R1, R2>;

  /// Check if R1 != R2
  template<typename R1, typename R2>
  inline constexpr bool ratio_not_equal_v = !ratio_equal_v<R1, R2>;

  // =============================================================================
  // ratio arithmetic operations
  // =============================================================================

  namespace detail {

    /// Implementation of ratio_add with overflow protection
    template<typename R1, typename R2>
    struct ratio_add_impl {
      private:
        static constexpr auto g = gcd(R1::den, R2::den);

        // Scale denominators
        static constexpr auto d1_scaled = R1::den / g;
        static constexpr auto d2_scaled = R2::den / g;

        // Calculate numerator terms (with overflow check in consteval context)
        static consteval auto calc_num() -> ratio_int_t {
          const auto t1 = safe_mul(R1::num, d2_scaled);
          const auto t2 = safe_mul(R2::num, d1_scaled);
          return safe_add(t1, t2);
        }

        static consteval auto calc_den() -> ratio_int_t {
          return safe_mul(R1::den, d2_scaled);
        }

      public:
        static constexpr auto num = calc_num();
        static constexpr auto den = calc_den();
        using type                = ratio<num, den>;
    };

    /// Implementation of ratio_subtract
    template<typename R1, typename R2>
    struct ratio_subtract_impl {
        using neg_r2 = ratio<-R2::num, R2::den>;
        using type   = typename ratio_add_impl<R1, neg_r2>::type;
    };

    /// Implementation of ratio_multiply with overflow protection
    template<typename R1, typename R2>
    struct ratio_multiply_impl {
      private:
        // Cross-reduce to minimize overflow risk
        static constexpr auto g1 = gcd(R1::num, R2::den);
        static constexpr auto g2 = gcd(R2::num, R1::den);

        static consteval auto calc_num() -> ratio_int_t {
          return safe_mul(R1::num / g1, R2::num / g2);
        }

        static consteval auto calc_den() -> ratio_int_t {
          return safe_mul(R1::den / g2, R2::den / g1);
        }

      public:
        static constexpr auto num = calc_num();
        static constexpr auto den = calc_den();
        using type                = ratio<num, den>;
    };

    /// Implementation of ratio_divide
    template<typename R1, typename R2>
    struct ratio_divide_impl {
        static_assert(R2::num != 0, "Division by zero ratio");
        using inv_r2 = ratio<R2::den, R2::num>;
        using type   = typename ratio_multiply_impl<R1, inv_r2>::type;
    };

    /// Implementation of ratio_power (positive integer exponents)
    template<typename R, ratio_int_t Exp>
    struct ratio_power_impl;

    template<ratio_int_t N, ratio_int_t D, ratio_int_t Exp>
    struct ratio_power_impl<ratio<N, D>, Exp> {
      private:
        static_assert(Exp >= 0 || N != 0, "Cannot raise zero to negative power");

        static consteval auto calc_num() -> ratio_int_t {
          if constexpr (Exp >= 0) {
            return power(N, Exp);
          }
          else {
            return power(D, -Exp);
          }
        }

        static consteval auto calc_den() -> ratio_int_t {
          if constexpr (Exp >= 0) {
            return power(D, Exp);
          }
          else {
            return power(N, -Exp);
          }
        }

      public:
        using type = ratio<calc_num(), calc_den()>;
    };

    // Specialization for Exp = 0
    template<ratio_int_t N, ratio_int_t D>
    struct ratio_power_impl<ratio<N, D>, 0> {
        using type = ratio_one;
    };

    // Specialization for Exp = 1
    template<ratio_int_t N, ratio_int_t D>
    struct ratio_power_impl<ratio<N, D>, 1> {
        using type = typename ratio<N, D>::type;
    };

    /// Implementation of ratio_negate
    template<typename R>
    struct ratio_negate_impl {
        using type = ratio<-R::num, R::den>;
    };

    /// Implementation of ratio_abs
    template<typename R>
    struct ratio_abs_impl {
        using type = ratio<abs(R::num), R::den>;
    };

    /// Implementation of ratio_inverse
    template<typename R>
    struct ratio_inverse_impl {
        static_assert(R::num != 0, "Cannot invert zero ratio");
        using type = ratio<R::den, R::num>;
    };

  } // namespace detail

  // =============================================================================
  // Public ratio arithmetic type aliases
  // =============================================================================

  /// Add two ratios: R1 + R2
  template<typename R1, typename R2>
  using ratio_add = typename detail::ratio_add_impl<typename R1::type, typename R2::type>::type;

  /// Subtract two ratios: R1 - R2
  template<typename R1, typename R2>
  using ratio_subtract =
      typename detail::ratio_subtract_impl<typename R1::type, typename R2::type>::type;

  /// Multiply two ratios: R1 * R2
  template<typename R1, typename R2>
  using ratio_multiply =
      typename detail::ratio_multiply_impl<typename R1::type, typename R2::type>::type;

  /// Divide two ratios: R1 / R2
  template<typename R1, typename R2>
  using ratio_divide =
      typename detail::ratio_divide_impl<typename R1::type, typename R2::type>::type;

  /// Raise ratio to integer power: R^Exp
  template<typename R, ratio_int_t Exp>
  using ratio_power = typename detail::ratio_power_impl<typename R::type, Exp>::type;

  /// Negate a ratio: -R
  template<typename R>
  using ratio_negate = typename detail::ratio_negate_impl<typename R::type>::type;

  /// Absolute value of ratio: |R|
  template<typename R>
  using ratio_abs = typename detail::ratio_abs_impl<typename R::type>::type;

  /// Inverse of ratio: 1/R
  template<typename R>
  using ratio_inverse = typename detail::ratio_inverse_impl<typename R::type>::type;

  // =============================================================================
  // Concepts for ratio types
  // =============================================================================

  /// Concept for types that are ratio specializations
  template<typename T>
  concept Ratio = requires {
    { T::num } -> std::convertible_to<ratio_int_t>;
    { T::den } -> std::convertible_to<ratio_int_t>;
    requires T::den > 0;                       // After normalization, den is always positive
    requires detail::gcd(T::num, T::den) == 1; // Must be fully reduced
  };

  /// Concept for ratio with value zero
  template<typename T>
  concept RatioZero = Ratio<T> && T::num == 0;

  /// Concept for ratio with positive value
  template<typename T>
  concept RatioPositive = Ratio<T> && T::num > 0;

  /// Concept for ratio with negative value
  template<typename T>
  concept RatioNegative = Ratio<T> && T::num < 0;

  /// Concept for ratio representing an integer (den == 1)
  template<typename T>
  concept RatioInteger = Ratio<T> && T::den == 1;

  // =============================================================================
  // SI prefixes as ratios
  // =============================================================================

  // Helper to compute 10^N at compile time
  namespace detail {
    template<int N>
    struct power_of_10 {
        static constexpr ratio_int_t value = 10 * power_of_10<N - 1>::value;
    };
    template<>
    struct power_of_10<0> {
        static constexpr ratio_int_t value = 1;
    };
    template<int N>
    inline constexpr ratio_int_t power_of_10_v = power_of_10<N>::value;
  } // namespace detail

  // Large prefixes
#if UNITS_HAS_INT128
  // With __int128, we can represent all SI prefixes exactly
  using quetta = ratio<detail::power_of_10_v<30>, 1>; // 10^30
  using ronna  = ratio<detail::power_of_10_v<27>, 1>; // 10^27
  using yotta  = ratio<detail::power_of_10_v<24>, 1>; // 10^24
  using zetta  = ratio<detail::power_of_10_v<21>, 1>; // 10^21
#else
  // Without __int128, clamp to max representable (10^18)
  using quetta = ratio<detail::power_of_10_v<18>, 1>; // Clamped from 10^30
  using ronna  = ratio<detail::power_of_10_v<18>, 1>; // Clamped from 10^27
  using yotta  = ratio<detail::power_of_10_v<18>, 1>; // Clamped from 10^24
  using zetta  = ratio<detail::power_of_10_v<18>, 1>; // Clamped from 10^21
#endif
  using exa   = ratio<detail::power_of_10_v<18>, 1>; // 10^18
  using peta  = ratio<detail::power_of_10_v<15>, 1>; // 10^15
  using tera  = ratio<detail::power_of_10_v<12>, 1>; // 10^12
  using giga  = ratio<detail::power_of_10_v<9>, 1>;  // 10^9
  using mega  = ratio<detail::power_of_10_v<6>, 1>;  // 10^6
  using kilo  = ratio<detail::power_of_10_v<3>, 1>;  // 10^3
  using hecto = ratio<detail::power_of_10_v<2>, 1>;  // 10^2
  using deca  = ratio<detail::power_of_10_v<1>, 1>;  // 10^1

  // Unity
  using one = ratio<1, 1>;

  // Small prefixes
  using deci  = ratio<1, detail::power_of_10_v<1>>;  // 10^-1
  using centi = ratio<1, detail::power_of_10_v<2>>;  // 10^-2
  using milli = ratio<1, detail::power_of_10_v<3>>;  // 10^-3
  using micro = ratio<1, detail::power_of_10_v<6>>;  // 10^-6
  using nano  = ratio<1, detail::power_of_10_v<9>>;  // 10^-9
  using pico  = ratio<1, detail::power_of_10_v<12>>; // 10^-12
  using femto = ratio<1, detail::power_of_10_v<15>>; // 10^-15
  using atto  = ratio<1, detail::power_of_10_v<18>>; // 10^-18
#if UNITS_HAS_INT128
  // With __int128, we can represent all SI prefixes exactly
  using zepto  = ratio<1, detail::power_of_10_v<21>>; // 10^-21
  using yocto  = ratio<1, detail::power_of_10_v<24>>; // 10^-24
  using ronto  = ratio<1, detail::power_of_10_v<27>>; // 10^-27
  using quecto = ratio<1, detail::power_of_10_v<30>>; // 10^-30
#else
  // Without __int128, clamp to max representable (10^-18)
  using zepto  = ratio<1, detail::power_of_10_v<18>>; // Clamped from 10^-21
  using yocto  = ratio<1, detail::power_of_10_v<18>>; // Clamped from 10^-24
  using ronto  = ratio<1, detail::power_of_10_v<18>>; // Clamped from 10^-27
  using quecto = ratio<1, detail::power_of_10_v<18>>; // Clamped from 10^-30
#endif

} // namespace units
