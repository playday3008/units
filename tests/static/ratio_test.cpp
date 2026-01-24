// SPDX-License-Identifier: MIT

// Static assertion tests for compile-time rational arithmetic
// All tests execute at compile time via static_assert

#include <units/core/ratio.hpp>

namespace units::test {

  // =============================================================================
  // Basic ratio construction and normalization
  // =============================================================================

  // Simple ratios should store correct values
  static_assert(ratio<1, 2>::num == 1);
  static_assert(ratio<1, 2>::den == 2);

  static_assert(ratio<3, 4>::num == 3);
  static_assert(ratio<3, 4>::den == 4);

  // Zero numerator
  static_assert(ratio<0, 5>::num == 0);
  static_assert(ratio<0, 5>::den == 1); // Normalized to 0/1

  // Whole numbers (den = 1)
  static_assert(ratio<42>::num == 42);
  static_assert(ratio<42>::den == 1);

  static_assert(ratio<-7>::num == -7);
  static_assert(ratio<-7>::den == 1);

  // =============================================================================
  // GCD normalization
  // =============================================================================

  // 2/4 should reduce to 1/2
  static_assert(ratio<2, 4>::num == 1);
  static_assert(ratio<2, 4>::den == 2);

  // 6/9 should reduce to 2/3
  static_assert(ratio<6, 9>::num == 2);
  static_assert(ratio<6, 9>::den == 3);

  // 12/8 should reduce to 3/2
  static_assert(ratio<12, 8>::num == 3);
  static_assert(ratio<12, 8>::den == 2);

  // 100/25 should reduce to 4/1
  static_assert(ratio<100, 25>::num == 4);
  static_assert(ratio<100, 25>::den == 1);

  // Large numbers with common factor
  static_assert(ratio<100'0000, 1000>::num == 1000);
  static_assert(ratio<100'0000, 1000>::den == 1);

  // =============================================================================
  // Sign normalization (denominator always positive)
  // =============================================================================

  // Positive/Positive = Positive
  static_assert(ratio<3, 4>::num == 3);
  static_assert(ratio<3, 4>::den == 4);

  // Negative/Positive = Negative numerator
  static_assert(ratio<-3, 4>::num == -3);
  static_assert(ratio<-3, 4>::den == 4);

  // Positive/Negative = Negative numerator (sign moves to numerator)
  static_assert(ratio<3, -4>::num == -3);
  static_assert(ratio<3, -4>::den == 4);

  // Negative/Negative = Positive
  static_assert(ratio<-3, -4>::num == 3);
  static_assert(ratio<-3, -4>::den == 4);

  // Combined with GCD reduction
  static_assert(ratio<-6, -9>::num == 2);
  static_assert(ratio<-6, -9>::den == 3);

  static_assert(ratio<6, -9>::num == -2);
  static_assert(ratio<6, -9>::den == 3);

  // =============================================================================
  // ratio_equal comparison
  // =============================================================================

  // Same values are equal
  static_assert(ratio_equal_v<ratio<1, 2>, ratio<1, 2>>);
  static_assert(ratio_equal_v<ratio<3, 4>, ratio<3, 4>>);

  // Equivalent ratios are equal after normalization
  static_assert(ratio_equal_v<ratio<2, 4>, ratio<1, 2>>);
  static_assert(ratio_equal_v<ratio<6, 9>, ratio<2, 3>>);
  static_assert(ratio_equal_v<ratio<-2, 4>, ratio<1, -2>>);

  // Different ratios are not equal
  static_assert(!ratio_equal_v<ratio<1, 2>, ratio<1, 3>>);
  static_assert(!ratio_equal_v<ratio<2, 3>, ratio<3, 4>>);

  // Zero is zero
  static_assert(ratio_equal_v<ratio<0, 1>, ratio<0, 100>>);
  static_assert(ratio_equal_v<ratio<0, 5>, ratio_zero>);

  // =============================================================================
  // ratio_less comparison
  // =============================================================================

  // Basic ordering
  static_assert(ratio_less_v<ratio<1, 4>, ratio<1, 2>>);
  static_assert(ratio_less_v<ratio<1, 3>, ratio<1, 2>>);
  static_assert(ratio_less_v<ratio<2, 5>, ratio<1, 2>>);

  // Not less
  static_assert(!ratio_less_v<ratio<1, 2>, ratio<1, 4>>);
  static_assert(!ratio_less_v<ratio<1, 2>, ratio<1, 2>>); // Equal, not less

  // Negative numbers
  static_assert(ratio_less_v<ratio<-1, 2>, ratio<1, 2>>);
  static_assert(ratio_less_v<ratio<-3, 4>, ratio<-1, 4>>);
  static_assert(!ratio_less_v<ratio<1, 2>, ratio<-1, 2>>);

  // Zero comparisons
  static_assert(ratio_less_v<ratio<-1, 2>, ratio<0>>);
  static_assert(ratio_less_v<ratio<0>, ratio<1, 2>>);
  static_assert(!ratio_less_v<ratio<0>, ratio<0>>);

  // =============================================================================
  // Other comparisons (derived from equal and less)
  // =============================================================================

  static_assert(ratio_less_equal_v<ratio<1, 4>, ratio<1, 2>>);
  static_assert(ratio_less_equal_v<ratio<1, 2>, ratio<1, 2>>);
  static_assert(!ratio_less_equal_v<ratio<3, 4>, ratio<1, 2>>);

  static_assert(ratio_greater_v<ratio<3, 4>, ratio<1, 2>>);
  static_assert(!ratio_greater_v<ratio<1, 4>, ratio<1, 2>>);

  static_assert(ratio_greater_equal_v<ratio<3, 4>, ratio<1, 2>>);
  static_assert(ratio_greater_equal_v<ratio<1, 2>, ratio<1, 2>>);

  static_assert(ratio_not_equal_v<ratio<1, 2>, ratio<1, 3>>);
  static_assert(!ratio_not_equal_v<ratio<1, 2>, ratio<2, 4>>);

  // =============================================================================
  // ratio_add
  // =============================================================================

  // Simple addition
  static_assert(ratio_equal_v<ratio_add<ratio<1, 2>, ratio<1, 2>>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_add<ratio<1, 4>, ratio<1, 4>>, ratio<1, 2>>);

  // Different denominators
  static_assert(ratio_equal_v<ratio_add<ratio<1, 2>, ratio<1, 3>>, ratio<5, 6>>);
  static_assert(ratio_equal_v<ratio_add<ratio<1, 4>, ratio<1, 6>>, ratio<5, 12>>);

  // Adding zero
  static_assert(ratio_equal_v<ratio_add<ratio<3, 4>, ratio<0>>, ratio<3, 4>>);
  static_assert(ratio_equal_v<ratio_add<ratio<0>, ratio<5, 7>>, ratio<5, 7>>);

  // Negative numbers
  static_assert(ratio_equal_v<ratio_add<ratio<1, 2>, ratio<-1, 4>>, ratio<1, 4>>);
  static_assert(ratio_equal_v<ratio_add<ratio<-1, 2>, ratio<-1, 2>>, ratio<-1>>);

  // Results that reduce
  static_assert(ratio_equal_v<ratio_add<ratio<1, 6>, ratio<1, 6>>, ratio<1, 3>>);

  // =============================================================================
  // ratio_subtract
  // =============================================================================

  // Simple subtraction
  static_assert(ratio_equal_v<ratio_subtract<ratio<3, 4>, ratio<1, 4>>, ratio<1, 2>>);
  static_assert(ratio_equal_v<ratio_subtract<ratio<1>, ratio<1, 2>>, ratio<1, 2>>);

  // Result is zero
  static_assert(ratio_equal_v<ratio_subtract<ratio<1, 2>, ratio<1, 2>>, ratio<0>>);

  // Negative result
  static_assert(ratio_equal_v<ratio_subtract<ratio<1, 4>, ratio<1, 2>>, ratio<-1, 4>>);

  // Subtracting negative (adding)
  static_assert(ratio_equal_v<ratio_subtract<ratio<1, 2>, ratio<-1, 2>>, ratio<1>>);

  // Different denominators
  static_assert(ratio_equal_v<ratio_subtract<ratio<1, 2>, ratio<1, 3>>, ratio<1, 6>>);

  // =============================================================================
  // ratio_multiply
  // =============================================================================

  // Simple multiplication
  static_assert(ratio_equal_v<ratio_multiply<ratio<2>, ratio<3>>, ratio<6>>);
  static_assert(ratio_equal_v<ratio_multiply<ratio<1, 2>, ratio<1, 2>>, ratio<1, 4>>);

  // Cross reduction
  static_assert(ratio_equal_v<ratio_multiply<ratio<2, 3>, ratio<3, 4>>, ratio<1, 2>>);
  static_assert(ratio_equal_v<ratio_multiply<ratio<4, 5>, ratio<5, 6>>, ratio<2, 3>>);

  // Multiply by one
  static_assert(ratio_equal_v<ratio_multiply<ratio<7, 11>, ratio<1>>, ratio<7, 11>>);

  // Multiply by zero
  static_assert(ratio_equal_v<ratio_multiply<ratio<7, 11>, ratio<0>>, ratio<0>>);

  // Sign handling
  static_assert(ratio_equal_v<ratio_multiply<ratio<-2, 3>, ratio<3, 4>>, ratio<-1, 2>>);
  static_assert(ratio_equal_v<ratio_multiply<ratio<-2, 3>, ratio<-3, 4>>, ratio<1, 2>>);

  // =============================================================================
  // ratio_divide
  // =============================================================================

  // Simple division
  static_assert(ratio_equal_v<ratio_divide<ratio<1, 2>, ratio<1, 2>>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_divide<ratio<3, 4>, ratio<1, 2>>, ratio<3, 2>>);

  // Division reduces
  static_assert(ratio_equal_v<ratio_divide<ratio<2, 3>, ratio<4, 3>>, ratio<1, 2>>);

  // Divide by one
  static_assert(ratio_equal_v<ratio_divide<ratio<5, 7>, ratio<1>>, ratio<5, 7>>);

  // Sign handling
  static_assert(ratio_equal_v<ratio_divide<ratio<-1, 2>, ratio<1, 4>>, ratio<-2>>);
  static_assert(ratio_equal_v<ratio_divide<ratio<-1, 2>, ratio<-1, 4>>, ratio<2>>);

  // =============================================================================
  // ratio_power
  // =============================================================================

  // Power of 0 is 1
  static_assert(ratio_equal_v<ratio_power<ratio<5, 3>, 0>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_power<ratio<-7, 11>, 0>, ratio<1>>);

  // Power of 1 is identity
  static_assert(ratio_equal_v<ratio_power<ratio<5, 3>, 1>, ratio<5, 3>>);
  static_assert(ratio_equal_v<ratio_power<ratio<-2, 7>, 1>, ratio<-2, 7>>);

  // Power of 2
  static_assert(ratio_equal_v<ratio_power<ratio<2, 3>, 2>, ratio<4, 9>>);
  static_assert(ratio_equal_v<ratio_power<ratio<1, 2>, 2>, ratio<1, 4>>);
  static_assert(ratio_equal_v<ratio_power<ratio<-3, 2>, 2>, ratio<9, 4>>);

  // Power of 3
  static_assert(ratio_equal_v<ratio_power<ratio<2, 3>, 3>, ratio<8, 27>>);
  static_assert(ratio_equal_v<ratio_power<ratio<-2, 3>, 3>, ratio<-8, 27>>);

  // Higher powers
  static_assert(ratio_equal_v<ratio_power<ratio<2>, 10>, ratio<1024>>);
  static_assert(ratio_equal_v<ratio_power<ratio<1, 2>, 4>, ratio<1, 16>>);

  // Negative exponents (invert first)
  static_assert(ratio_equal_v<ratio_power<ratio<2>, -1>, ratio<1, 2>>);
  static_assert(ratio_equal_v<ratio_power<ratio<2, 3>, -2>, ratio<9, 4>>);
  static_assert(ratio_equal_v<ratio_power<ratio<1, 2>, -3>, ratio<8>>);

  // =============================================================================
  // ratio_negate
  // =============================================================================

  static_assert(ratio_equal_v<ratio_negate<ratio<3, 4>>, ratio<-3, 4>>);
  static_assert(ratio_equal_v<ratio_negate<ratio<-3, 4>>, ratio<3, 4>>);
  static_assert(ratio_equal_v<ratio_negate<ratio<0>>, ratio<0>>);

  // =============================================================================
  // ratio_abs
  // =============================================================================

  static_assert(ratio_equal_v<ratio_abs<ratio<3, 4>>, ratio<3, 4>>);
  static_assert(ratio_equal_v<ratio_abs<ratio<-3, 4>>, ratio<3, 4>>);
  static_assert(ratio_equal_v<ratio_abs<ratio<0>>, ratio<0>>);

  // =============================================================================
  // ratio_inverse
  // =============================================================================

  static_assert(ratio_equal_v<ratio_inverse<ratio<2, 3>>, ratio<3, 2>>);
  static_assert(ratio_equal_v<ratio_inverse<ratio<-2, 3>>, ratio<-3, 2>>);
  static_assert(ratio_equal_v<ratio_inverse<ratio<1>>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_inverse<ratio<5>>, ratio<1, 5>>);

  // =============================================================================
  // Ratio concepts
  // =============================================================================

  // Ratio concept
  static_assert(Ratio<ratio<1, 2>>);
  static_assert(Ratio<ratio<0>>);
  static_assert(Ratio<ratio<-5, 7>>);
  static_assert(Ratio<ratio<100'0000, 1>>);

  // RatioZero concept
  static_assert(RatioZero<ratio<0>>);
  static_assert(RatioZero<ratio<0, 5>>);
  static_assert(!RatioZero<ratio<1, 2>>);

  // RatioPositive concept
  static_assert(RatioPositive<ratio<1, 2>>);
  static_assert(RatioPositive<ratio<3>>);
  static_assert(!RatioPositive<ratio<0>>);
  static_assert(!RatioPositive<ratio<-1, 2>>);

  // RatioNegative concept
  static_assert(RatioNegative<ratio<-1, 2>>);
  static_assert(RatioNegative<ratio<-3>>);
  static_assert(!RatioNegative<ratio<0>>);
  static_assert(!RatioNegative<ratio<1, 2>>);

  // RatioInteger concept
  static_assert(RatioInteger<ratio<5>>);
  static_assert(RatioInteger<ratio<-3>>);
  static_assert(RatioInteger<ratio<0>>);
  static_assert(RatioInteger<ratio<100, 25>>); // Reduces to 4/1
  static_assert(!RatioInteger<ratio<1, 2>>);
  static_assert(!RatioInteger<ratio<3, 2>>);

  // =============================================================================
  // SI prefix values
  // =============================================================================

  // Large prefixes
  static_assert(ratio_equal_v<kilo, ratio<1000>>);
  static_assert(ratio_equal_v<mega, ratio<100'0000>>);
  static_assert(ratio_equal_v<giga, ratio<10'0000'0000>>);
  static_assert(ratio_equal_v<tera, ratio<1'0000'0000'0000>>);

  // Small prefixes
  static_assert(ratio_equal_v<milli, ratio<1, 1000>>);
  static_assert(ratio_equal_v<micro, ratio<1, 100'0000>>);
  static_assert(ratio_equal_v<nano, ratio<1, 10'0000'0000>>);
  static_assert(ratio_equal_v<pico, ratio<1, 1'0000'0000'0000>>);

  // Prefix relationships
  static_assert(ratio_equal_v<ratio_multiply<kilo, milli>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_multiply<mega, micro>, ratio<1>>);
  static_assert(ratio_equal_v<ratio_divide<kilo, milli>, mega>);
  static_assert(ratio_equal_v<ratio_power<kilo, 2>, mega>);
  static_assert(ratio_equal_v<ratio_power<milli, 2>, micro>);

  // =============================================================================
  // Common type aliases
  // =============================================================================

  static_assert(ratio_equal_v<ratio_zero, ratio<0>>);
  static_assert(ratio_equal_v<ratio_one, ratio<1>>);
  static_assert(ratio_equal_v<ratio_half, ratio<1, 2>>);

  // =============================================================================
  // Floating point conversion
  // =============================================================================

  static_assert(ratio<1, 2>::value() == 0.5);
  static_assert(ratio<3, 4>::value() == 0.75);
  static_assert(ratio<1, 3>::value<float>() > 0.333F);
  static_assert(ratio<1, 3>::value<float>() < 0.334F);
  static_assert(ratio<-1, 4>::value() == -0.25);

  // =============================================================================
  // Complex expression tests
  // =============================================================================

  // (1/2 + 1/3) * 6 = 5
  static_assert(
      ratio_equal_v<ratio_multiply<ratio_add<ratio<1, 2>, ratio<1, 3>>, ratio<6>>, ratio<5>>);

  // ((2/3)^2 - 1/9) / (1/3) = 1
  static_assert(ratio_equal_v<
                ratio_divide<ratio_subtract<ratio_power<ratio<2, 3>, 2>, ratio<1, 9>>, ratio<1, 3>>,
                ratio<1>>);

  // 1/(1/2) = 2
  static_assert(ratio_equal_v<ratio_inverse<ratio<1, 2>>, ratio<2>>);

  // |(-3/4)| + 1/4 = 1
  static_assert(ratio_equal_v<ratio_add<ratio_abs<ratio<-3, 4>>, ratio<1, 4>>, ratio<1>>);

  // kilo^3 = giga
  static_assert(ratio_equal_v<ratio_power<kilo, 3>, giga>);

  // milli^-1 = kilo
  static_assert(ratio_equal_v<ratio_power<milli, -1>, kilo>);

} // namespace units::test

// Main function - tests are all compile-time, so just need to compile successfully
auto main() -> int {
  return 0;
}
