// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include <units/core/quantity.hpp>
#include <units/core/quantity_point.hpp>
#include <units/core/quantity_spec.hpp>
#include <units/core/reference.hpp>
#include <units/core/unit.hpp>
#include <units/systems/isq/base.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>

namespace units {

  // =============================================================================
  // Temperature conversion constants
  // =============================================================================
  // Named constants for temperature scale offsets and conversion factors

  namespace temperature_constants {

    /// Offset from absolute zero to ice point (0 degC) in kelvin
    inline constexpr double ice_point_offset_k = 273.15;

    /// Offset from absolute zero to fahrenheit zero (0 degF) in kelvin
    /// Calculated as: 459.67 / 1.8 = 255.372222...
    inline constexpr double fahrenheit_zero_offset_k = 255.37222222222222;

    /// Fahrenheit to kelvin scale factor: 1 degF = 5/9 K
    inline constexpr double fahrenheit_to_kelvin_factor = 5.0 / 9.0;

    /// Kelvin to fahrenheit scale factor: 1 K = 9/5 degF
    inline constexpr double kelvin_to_fahrenheit_factor = 9.0 / 5.0;

    /// Offset from fahrenheit zero to ice point: 32 degF = 0 degC
    inline constexpr double fahrenheit_ice_point_offset = 32.0;

    /// Rankine offset: 0 degF = 459.67 R, also 0 R = 0 K
    inline constexpr double rankine_offset = 459.67;

  } // namespace temperature_constants

  // =============================================================================
  // Temperature Point Origins
  // =============================================================================
  // Temperature is an affine quantity - it has a meaningful zero point.
  // Different temperature scales have different zeros:
  // - Kelvin: absolute zero (0 K = -273.15 degC)
  // - Celsius: ice point / water freezing (0 degC = 273.15 K)
  // - Fahrenheit: historical brine mixture (0 degF = 255.372 K)

  /// Absolute zero - the zero point for thermodynamic temperature (Kelvin scale)
  /// This is the theoretical temperature where all thermal motion ceases.
  struct absolute_zero_t final : absolute_point_origin<isq::thermodynamic_temperature_t> {};
  inline constexpr absolute_zero_t absolute_zero {};

  namespace si {

    // =============================================================================
    // Kelvin reference and quantity_point type aliases
    // =============================================================================

    /// Reference for temperature in kelvin
    using kelvin_ref = reference<isq::thermodynamic_temperature_t, kelvin_t>;

    /// Type alias for temperature quantity_point in kelvin (with absolute zero origin)
    template<typename Rep = double>
    using kelvin_point = quantity_point<kelvin_ref, absolute_zero_t, Rep>;

    // =============================================================================
    // Celsius scale - offset from absolute zero by 273.15 K
    // =============================================================================

    /// The ice point - zero degrees Celsius
    /// Defined as exactly 273.15 K above absolute zero
    /// This is the freezing point of water at standard pressure
    struct ice_point_t final
        : relative_point_origin<absolute_zero_t,
                                kelvin_ref,
                                double,
                                temperature_constants::ice_point_offset_k> {};
    inline constexpr ice_point_t ice_point {};

    /// Reference for temperature intervals in Celsius
    /// Note: For intervals/differences, 1 degC = 1 K
    using celsius_ref = reference<isq::thermodynamic_temperature_t, degree_celsius_t>;

    /// Type alias for temperature quantity_point in Celsius (with ice point origin)
    template<typename Rep = double>
    using celsius_point = quantity_point<celsius_ref, ice_point_t, Rep>;

    // =============================================================================
    // Fahrenheit scale
    // =============================================================================

    /// Fahrenheit zero point - defined relative to absolute zero
    /// 0 degF = 255.372222... K = 459.67 R (Rankine)
    /// More precisely: 0 degF = (459.67 / 1.8) K = 255.372222... K
    struct fahrenheit_zero_t final
        : relative_point_origin<absolute_zero_t,
                                kelvin_ref,
                                double,
                                temperature_constants::fahrenheit_zero_offset_k> {};
    inline constexpr fahrenheit_zero_t fahrenheit_zero {};

    /// degree Fahrenheit - unit for Fahrenheit temperature intervals
    /// 1 degF = 5/9 K (or 1 K = 9/5 degF = 1.8 degF)
    struct degree_fahrenheit_t final
        : named_unit<"degF", kind_of<isq::thermodynamic_temperature_t>, mag_ratio<5, 9> // 1 degF =
                                                                                        // 5/9 K
                     > {};
    inline constexpr degree_fahrenheit_t degree_fahrenheit {};

    /// Reference for temperature intervals in Fahrenheit
    using fahrenheit_ref = reference<isq::thermodynamic_temperature_t, degree_fahrenheit_t>;

    /// Type alias for temperature quantity_point in Fahrenheit
    template<typename Rep = double>
    using fahrenheit_point = quantity_point<fahrenheit_ref, fahrenheit_zero_t, Rep>;

    // =============================================================================
    // Temperature quantity_point factory functions
    // =============================================================================

    /// Create a temperature point in Kelvin
    template<typename Rep = double>
    [[nodiscard]]
    constexpr auto kelvin_pt(Rep value) noexcept {
      return kelvin_point<Rep> { quantity<kelvin_ref, Rep> { value } };
    }

    /// Create a temperature point in Celsius
    template<typename Rep = double>
    [[nodiscard]]
    constexpr auto celsius_pt(Rep value) noexcept {
      return celsius_point<Rep> { quantity<celsius_ref, Rep> { value } };
    }

    /// Create a temperature point in Fahrenheit
    template<typename Rep = double>
    [[nodiscard]]
    constexpr auto fahrenheit_pt(Rep value) noexcept {
      return fahrenheit_point<Rep> { quantity<fahrenheit_ref, Rep> { value } };
    }

    // =============================================================================
    // Conversion utilities
    // =============================================================================

    namespace detail {

      /// Convert a Celsius temperature point to Kelvin
      template<typename Rep>
      [[nodiscard]]
      constexpr auto celsius_to_kelvin(celsius_point<Rep> temp_c) noexcept {
        // K = C + 273.15
        return kelvin_point<Rep> { quantity<kelvin_ref, Rep> {
            temp_c.value() + static_cast<Rep>(temperature_constants::ice_point_offset_k) } };
      }

      /// Convert a Kelvin temperature point to Celsius
      template<typename Rep>
      [[nodiscard]]
      constexpr auto kelvin_to_celsius(kelvin_point<Rep> temp_k) noexcept {
        // C = K - 273.15
        return celsius_point<Rep> { quantity<celsius_ref, Rep> {
            temp_k.value() - static_cast<Rep>(temperature_constants::ice_point_offset_k) } };
      }

      /// Convert a Fahrenheit temperature point to Kelvin
      template<typename Rep>
      [[nodiscard]]
      constexpr auto fahrenheit_to_kelvin(fahrenheit_point<Rep> temp_f) noexcept {
        // K = (F + 459.67) * 5/9
        // Or equivalently: K = (F - 32) * 5/9 + 273.15
        return kelvin_point<Rep> { quantity<kelvin_ref, Rep> {
            (temp_f.value() + static_cast<Rep>(temperature_constants::rankine_offset))
            * static_cast<Rep>(temperature_constants::fahrenheit_to_kelvin_factor) } };
      }

      /// Convert a Kelvin temperature point to Fahrenheit
      template<typename Rep>
      [[nodiscard]]
      constexpr auto kelvin_to_fahrenheit(kelvin_point<Rep> temp_k) noexcept {
        // F = K * 9/5 - 459.67
        return fahrenheit_point<Rep> { quantity<fahrenheit_ref, Rep> {
            temp_k.value() * static_cast<Rep>(temperature_constants::kelvin_to_fahrenheit_factor)
            - static_cast<Rep>(temperature_constants::rankine_offset) } };
      }

      /// Convert Celsius to Fahrenheit
      template<typename Rep>
      [[nodiscard]]
      constexpr auto celsius_to_fahrenheit(celsius_point<Rep> temp_c) noexcept {
        // F = C * 9/5 + 32
        return fahrenheit_point<Rep> { quantity<fahrenheit_ref, Rep> {
            temp_c.value() * static_cast<Rep>(temperature_constants::kelvin_to_fahrenheit_factor)
            + static_cast<Rep>(temperature_constants::fahrenheit_ice_point_offset) } };
      }

      /// Convert Fahrenheit to Celsius
      template<typename Rep>
      [[nodiscard]]
      constexpr auto fahrenheit_to_celsius(fahrenheit_point<Rep> temp_f) noexcept {
        // C = (F - 32) * 5/9
        return celsius_point<Rep> { quantity<celsius_ref, Rep> {
            (temp_f.value() - static_cast<Rep>(temperature_constants::fahrenheit_ice_point_offset))
            * static_cast<Rep>(temperature_constants::fahrenheit_to_kelvin_factor) } };
      }

    } // namespace detail

    /// Convert a temperature point to Kelvin
    template<typename TempPoint>
      requires QuantityPoint<TempPoint>
    [[nodiscard]]
    constexpr auto to_kelvin(TempPoint temp) noexcept {
      using origin = typename TempPoint::origin_type;

      if constexpr (std::same_as<origin, absolute_zero_t>) {
        // Already in Kelvin scale
        return kelvin_point<typename TempPoint::rep> {
          quantity<kelvin_ref, typename TempPoint::rep> { temp.value() }
        };
      }
      else if constexpr (std::same_as<origin, ice_point_t>) {
        return detail::celsius_to_kelvin(temp);
      }
      else if constexpr (std::same_as<origin, fahrenheit_zero_t>) {
        return detail::fahrenheit_to_kelvin(temp);
      }
    }

    /// Convert a temperature point to Celsius
    template<typename TempPoint>
      requires QuantityPoint<TempPoint>
    [[nodiscard]]
    constexpr auto to_celsius(TempPoint temp) noexcept {
      using origin = typename TempPoint::origin_type;

      if constexpr (std::same_as<origin, ice_point_t>) {
        // Already in Celsius scale
        return celsius_point<typename TempPoint::rep> {
          quantity<celsius_ref, typename TempPoint::rep> { temp.value() }
        };
      }
      else if constexpr (std::same_as<origin, absolute_zero_t>) {
        return detail::kelvin_to_celsius(temp);
      }
      else if constexpr (std::same_as<origin, fahrenheit_zero_t>) {
        return detail::fahrenheit_to_celsius(temp);
      }
    }

    /// Convert a temperature point to Fahrenheit
    template<typename TempPoint>
      requires QuantityPoint<TempPoint>
    [[nodiscard]]
    constexpr auto to_fahrenheit(TempPoint temp) noexcept {
      using origin = typename TempPoint::origin_type;

      if constexpr (std::same_as<origin, fahrenheit_zero_t>) {
        // Already in Fahrenheit scale
        return fahrenheit_point<typename TempPoint::rep> {
          quantity<fahrenheit_ref, typename TempPoint::rep> { temp.value() }
        };
      }
      else if constexpr (std::same_as<origin, absolute_zero_t>) {
        return detail::kelvin_to_fahrenheit(temp);
      }
      else if constexpr (std::same_as<origin, ice_point_t>) {
        return detail::celsius_to_fahrenheit(temp);
      }
    }

    // =============================================================================
    // Static assertions for temperature
    // =============================================================================

    // Origins are valid
    static_assert(PointOrigin<absolute_zero_t>);
    static_assert(PointOrigin<ice_point_t>);
    static_assert(PointOrigin<fahrenheit_zero_t>);

    // Origin types are correct
    static_assert(is_absolute_origin_v<absolute_zero_t>);
    static_assert(is_relative_origin_v<ice_point_t>);
    static_assert(is_relative_origin_v<fahrenheit_zero_t>);

    // degree_fahrenheit is a valid unit
    static_assert(Unit<degree_fahrenheit_t>);
    static_assert(NamedUnit<degree_fahrenheit_t>);

    // Verify Fahrenheit magnitude (1 degF = 5/9 K)
    static_assert(degree_fahrenheit_t::magnitude_type::num == 5);
    static_assert(degree_fahrenheit_t::magnitude_type::den == 9);

    // Zero overhead guarantees for temperature points
    static_assert(sizeof(kelvin_point<double>) == sizeof(double));
    static_assert(sizeof(celsius_point<double>) == sizeof(double));
    static_assert(sizeof(fahrenheit_point<double>) == sizeof(double));

  } // namespace si

} // namespace units
