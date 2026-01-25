// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include <algorithm>
#include <format>
#include <string>
#include <string_view>

#include <units/core/quantity.hpp>
#include <units/core/reference.hpp>
#include <units/core/unit.hpp>

namespace units {

  // =============================================================================
  // Unit symbol generation
  // =============================================================================

  namespace detail {

    /// Get the symbol from a unit with a symbol member
    template<typename U>
      requires requires { U::symbol; }
    constexpr auto get_unit_symbol() -> std::string_view {
      return std::string_view { U::symbol.data, U::symbol.size() };
    }

    /// Build symbol for a unit term (unit^power)
    template<typename Term>
    constexpr void append_unit_term_symbol(std::string &result) {
      using unit           = typename Term::unit;
      constexpr auto power = Term::power;

      if constexpr (requires { unit::symbol; }) {
        result += std::string { unit::symbol.data, unit::symbol.size() };
        if constexpr (power != 1) {
          // Add superscript power
          if constexpr (power == 2) {
            result += "\xC2\xB2"; // UTF-8 superscript 2
          }
          else if constexpr (power == 3) {
            result += "\xC2\xB3"; // UTF-8 superscript 3
          }
          else if constexpr (power == -1) {
            result += "\xE2\x81\xBB\xC2\xB9"; // UTF-8 superscript -1
          }
          else if constexpr (power == -2) {
            result += "\xE2\x81\xBB\xC2\xB2"; // UTF-8 superscript -2
          }
          else {
            result += "^";
            result += std::to_string(power);
          }
        }
      }
    }

    /// Append symbols for numerator terms
    template<typename... Terms>
    constexpr void append_numerator_symbols(std::string &result,
                                            unit_numerator<Terms...> * /*tag*/) {
      bool first = true;
      (([&] -> auto {
         if (!first) {
           result += "\xC2\xB7"; // UTF-8 middle dot (·)
         }
         first = false;
         append_unit_term_symbol<Terms>(result);
       }()),
       ...);
    }

    /// Append symbols for denominator terms
    template<typename... Terms>
    constexpr void append_denominator_symbols(std::string &result,
                                              unit_denominator<Terms...> * /*tag*/) {
      bool first = true;
      (([&] -> auto {
         if (!first) {
           result += "\xC2\xB7"; // UTF-8 middle dot (·)
         }
         first = false;
         append_unit_term_symbol<Terms>(result);
       }()),
       ...);
    }

    /// Count terms in unit_numerator
    template<typename Num>
    struct numerator_term_count;

    template<typename... Terms>
    struct numerator_term_count<unit_numerator<Terms...>> {
        static constexpr std::size_t value = sizeof...(Terms);
    };

    /// Count terms in unit_denominator
    template<typename Den>
    struct denominator_term_count;

    template<typename... Terms>
    struct denominator_term_count<unit_denominator<Terms...>> {
        static constexpr std::size_t value = sizeof...(Terms);
    };

  } // namespace detail

  /// Get the unit symbol as a string for any unit type
  template<Unit U>
  [[nodiscard]]
  inline auto unit_symbol() -> std::string {
    if constexpr (requires { U::symbol; }) {
      // Named or scaled unit with a symbol
      return std::string { U::symbol.data, U::symbol.size() };
    }
    else if constexpr (requires {
                         typename U::numerator_terms;
                         typename U::denominator_terms;
                       })
    {
      // Derived unit - compose symbol from terms
      using Num = typename U::numerator_terms;
      using Den = typename U::denominator_terms;

      constexpr auto num_count = detail::numerator_term_count<Num>::value;
      constexpr auto den_count = detail::denominator_term_count<Den>::value;

      std::string result;

      if constexpr (num_count > 0) {
        detail::append_numerator_symbols(result, static_cast<Num *>(nullptr));
      }
      else {
        result = "1";
      }

      if constexpr (den_count > 0) {
        result += "/";
        if constexpr (den_count > 1) {
          result += "(";
        }
        detail::append_denominator_symbols(result, static_cast<Den *>(nullptr));
        if constexpr (den_count > 1) {
          result += ")";
        }
      }

      return result;
    }
    else {
      return "?";
    }
  }

  /// Get the unit symbol for a reference type
  template<Reference Ref>
  [[nodiscard]]
  inline auto reference_symbol() -> std::string {
    return unit_symbol<get_unit<Ref>>();
  }

  // =============================================================================
  // Format flags
  // =============================================================================

  /// Format options for quantities
  struct quantity_format_options {
      int  precision  = -1;    // -1 means default precision
      bool show_unit  = true;  // false for {:n} format
      char type       = 'g';   // Format type: f, e, g, etc.
      bool use_locale = false; // Use locale-specific formatting
  };

  // =============================================================================
  // std::formatter specialization for quantity
  // =============================================================================

} // namespace units

/// std::formatter specialization for units::quantity
///
/// Format specification:
///   [[fill]align][width][.precision][type][n]
///
/// Where:
///   - fill, align, width, precision, type: standard floating-point format options
///   - n: suppress unit symbol output
///
/// Examples:
///   std::format("{}", 5.0_m)        -> "5 m"
///   std::format("{:.2f}", 5.123_m)  -> "5.12 m"
///   std::format("{:n}", 5.0_m)      -> "5"
///   std::format("{:10.3f}", 5.0_m)  -> "     5.000 m"
template<units::Reference Ref, typename Rep>
struct std::formatter<units::quantity<Ref, Rep>> {
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    // std::formatter is a standard library customization point that requires public members
    units::quantity_format_options opts;
    std::string                    format_spec;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    constexpr auto parse(std::format_parse_context &ctx) {
      const auto *it  = ctx.begin();
      const auto *end = ctx.end();

      // Build format spec for the numeric value
      format_spec = "{:";

      // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      while (it != end && *it != '}') {
        if (*it == 'n') {
          // 'n' means no unit symbol
          opts.show_unit = false;
          ++it;
        }
        else {
          format_spec += *it;
          ++it;
        }
      }
      // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

      format_spec += '}';

      return it;
    }

    auto format(const units::quantity<Ref, Rep> &q, std::format_context &ctx) const {
      auto out = ctx.out();

      // Format the numeric value using runtime format
      // Store value in lvalue for make_format_args
      Rep         value     = q.value();
      std::string value_str = std::vformat(format_spec, std::make_format_args(value));
      out                   = std::ranges::copy(value_str, out).out;

      // Add unit symbol if requested
      if (opts.show_unit) {
        *out++      = ' '; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto symbol = units::reference_symbol<Ref>();
        out         = std::ranges::copy(symbol, out).out;
      }

      return out;
    }
};

namespace units {

  // =============================================================================
  // Convenience formatting functions
  // =============================================================================

  /// Format a quantity to a string with default formatting
  template<Reference Ref, typename Rep>
  [[nodiscard]]
  inline auto to_string(const quantity<Ref, Rep> &q) -> std::string {
    return std::format("{}", q);
  }

  /// Format a quantity to a string with specified precision
  template<Reference Ref, typename Rep>
  [[nodiscard]]
  inline auto to_string(const quantity<Ref, Rep> &q, int precision) -> std::string {
    // Format value with precision, then add unit symbol
    auto value_str = std::format("{:.{}f}", q.value(), precision);
    return value_str + " " + reference_symbol<Ref>();
  }

  /// Format a quantity value only (no unit)
  template<Reference Ref, typename Rep>
  [[nodiscard]]
  inline auto value_to_string(const quantity<Ref, Rep> &q) -> std::string {
    return std::format("{:n}", q);
  }

} // namespace units
