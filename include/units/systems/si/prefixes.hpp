// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/ratio.hpp>
#include <units/core/unit.hpp>
#include <units/systems/si/base.hpp>

namespace units::si {

  // =============================================================================
  // SI Prefixes (BIPM, Resolution 4 of the 2022 CGPM)
  // =============================================================================
  // All 24 SI prefixes from 10^-30 (quecto) to 10^30 (quetta).
  //
  // Note: std::intmax_t (typically 64-bit) can represent values up to ~9.2×10^18,
  // so the largest prefixes (10^21 and above) and smallest (10^-21 and below)
  // cannot be represented exactly. We provide the best approximation possible.
  //
  // For prefixes within range, the ratio is exact.

  // =============================================================================
  // Large prefixes (multipliers)
  // =============================================================================

  // Note: For prefixes within the range of __int128 (or std::intmax_t), the ratio
  // representation is exact. For extreme prefixes (quetta through zetta, and
  // zepto through quecto), the ratio is clamped to the maximum representable
  // power of 10 when __int128 is not available. Use ::units::quetta etc. from
  // ratio.hpp for proper __int128 support.

  /// exa - 10^18
  /// Symbol: E
  using exa = ::units::exa;

  /// peta - 10^15
  /// Symbol: P
  using peta = ::units::peta;

  /// tera - 10^12
  /// Symbol: T
  using tera = ::units::tera;

  /// giga - 10^9
  /// Symbol: G
  using giga = ::units::giga;

  /// mega - 10^6
  /// Symbol: M
  using mega = ::units::mega;

  /// kilo - 10^3
  /// Symbol: k
  using kilo = ::units::kilo;

  /// hecto - 10^2
  /// Symbol: h
  using hecto = ::units::hecto;

  /// deca - 10^1
  /// Symbol: da
  using deca = ::units::deca;

  // =============================================================================
  // Small prefixes (submultiples)
  // =============================================================================

  /// deci - 10^-1
  /// Symbol: d
  using deci = ::units::deci;

  /// centi - 10^-2
  /// Symbol: c
  using centi = ::units::centi;

  /// milli - 10^-3
  /// Symbol: m
  using milli = ::units::milli;

  /// micro - 10^-6
  /// Symbol: μ (or u in ASCII)
  using micro = ::units::micro;

  /// nano - 10^-9
  /// Symbol: n
  using nano = ::units::nano;

  /// pico - 10^-12
  /// Symbol: p
  using pico = ::units::pico;

  /// femto - 10^-15
  /// Symbol: f
  using femto = ::units::femto;

  /// atto - 10^-18
  /// Symbol: a
  using atto = ::units::atto;

  // =============================================================================
  // Common prefixed units for length (metre)
  // =============================================================================

  /// kilometre - 1000 metres
  struct kilometre_t final : prefixed_unit<"km", kilo, metre_t> {};
  inline constexpr kilometre_t kilometre {};
  inline constexpr kilometre_t kilometer = kilometre; // American spelling

  /// hectometre - 100 metres
  struct hectometre_t final : prefixed_unit<"hm", hecto, metre_t> {};
  inline constexpr hectometre_t hectometre {};

  /// decametre - 10 metres
  struct decametre_t final : prefixed_unit<"dam", deca, metre_t> {};
  inline constexpr decametre_t decametre {};

  /// decimetre - 0.1 metres
  struct decimetre_t final : prefixed_unit<"dm", deci, metre_t> {};
  inline constexpr decimetre_t decimetre {};

  /// centimetre - 0.01 metres
  struct centimetre_t final : prefixed_unit<"cm", centi, metre_t> {};
  inline constexpr centimetre_t centimetre {};
  inline constexpr centimetre_t centimeter = centimetre; // American spelling

  /// millimetre - 0.001 metres
  struct millimetre_t final : prefixed_unit<"mm", milli, metre_t> {};
  inline constexpr millimetre_t millimetre {};
  inline constexpr millimetre_t millimeter = millimetre; // American spelling

  /// micrometre - 10^-6 metres
  struct micrometre_t final : prefixed_unit<"um", micro, metre_t> {};
  inline constexpr micrometre_t micrometre {};
  inline constexpr micrometre_t micrometer = micrometre; // American spelling

  /// nanometre - 10^-9 metres
  struct nanometre_t final : prefixed_unit<"nm", nano, metre_t> {};
  inline constexpr nanometre_t nanometre {};
  inline constexpr nanometre_t nanometer = nanometre; // American spelling

  /// picometre - 10^-12 metres
  struct picometre_t final : prefixed_unit<"pm", pico, metre_t> {};
  inline constexpr picometre_t picometre {};

  /// femtometre - 10^-15 metres
  struct femtometre_t final : prefixed_unit<"fm", femto, metre_t> {};
  inline constexpr femtometre_t femtometre {};

  // =============================================================================
  // Common prefixed units for mass (gram, not kilogram for prefixes)
  // =============================================================================

  /// milligram - 10^-3 grams = 10^-6 kg
  struct milligram_t final : prefixed_unit<"mg", milli, gram_t> {};
  inline constexpr milligram_t milligram {};

  /// microgram - 10^-6 grams = 10^-9 kg
  struct microgram_t final : prefixed_unit<"ug", micro, gram_t> {};
  inline constexpr microgram_t microgram {};

  /// nanogram - 10^-9 grams = 10^-12 kg
  struct nanogram_t final : prefixed_unit<"ng", nano, gram_t> {};
  inline constexpr nanogram_t nanogram {};

  /// megagram (tonne) - 10^6 grams = 10^3 kg
  struct megagram_t final : prefixed_unit<"Mg", mega, gram_t> {};
  inline constexpr megagram_t megagram {};

  // =============================================================================
  // Common prefixed units for time (second)
  // =============================================================================

  /// millisecond - 10^-3 seconds
  struct millisecond_t final : prefixed_unit<"ms", milli, second_t> {};
  inline constexpr millisecond_t millisecond {};

  /// microsecond - 10^-6 seconds
  struct microsecond_t final : prefixed_unit<"us", micro, second_t> {};
  inline constexpr microsecond_t microsecond {};

  /// nanosecond - 10^-9 seconds
  struct nanosecond_t final : prefixed_unit<"ns", nano, second_t> {};
  inline constexpr nanosecond_t nanosecond {};

  /// picosecond - 10^-12 seconds
  struct picosecond_t final : prefixed_unit<"ps", pico, second_t> {};
  inline constexpr picosecond_t picosecond {};

  /// femtosecond - 10^-15 seconds
  struct femtosecond_t final : prefixed_unit<"fs", femto, second_t> {};
  inline constexpr femtosecond_t femtosecond {};

  // =============================================================================
  // Static assertions to verify prefix values
  // =============================================================================

  // Verify prefix ratios are correct
  static_assert(ratio_equal_v<kilo, ::units::ratio<1000>>);
  static_assert(ratio_equal_v<mega, ::units::ratio<100'0000>>);
  static_assert(ratio_equal_v<giga, ::units::ratio<10'0000'0000>>);

  static_assert(ratio_equal_v<milli, ::units::ratio<1, 1000>>);
  static_assert(ratio_equal_v<micro, ::units::ratio<1, 100'0000>>);
  static_assert(ratio_equal_v<nano, ::units::ratio<1, 10'0000'0000>>);

  // Verify prefix relationships
  static_assert(ratio_equal_v<ratio_multiply<kilo, milli>, ::units::ratio<1>>);
  static_assert(ratio_equal_v<ratio_multiply<mega, micro>, ::units::ratio<1>>);
  static_assert(ratio_equal_v<ratio_multiply<giga, nano>, ::units::ratio<1>>);

  // Verify prefixed unit magnitudes
  static_assert(kilometre_t::magnitude_type::num == 1000);
  static_assert(kilometre_t::magnitude_type::den == 1);

  static_assert(centimetre_t::magnitude_type::num == 1);
  static_assert(centimetre_t::magnitude_type::den == 100);

  static_assert(millimetre_t::magnitude_type::num == 1);
  static_assert(millimetre_t::magnitude_type::den == 1000);

} // namespace units::si
