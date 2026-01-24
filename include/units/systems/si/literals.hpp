// SPDX-License-Identifier: MIT

#pragma once

#include <units/core/quantity.hpp>
#include <units/systems/si/base.hpp>
#include <units/systems/si/derived.hpp>
#include <units/systems/si/prefixes.hpp>

namespace units::si {

  // =============================================================================
  // Unit factory types with static operator()
  // =============================================================================
  // These factory types use C++23 static operator() for zero overhead.
  // No hidden 'this' pointer - critical for embedded systems.

  // -----------------------------------------------------------------------------
  // Length factories
  // -----------------------------------------------------------------------------

  /// Factory for metre quantities
  struct metre_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, metre);
      }
  };
  inline constexpr metre_factory m_ {};

  /// Factory for kilometre quantities
  struct kilometre_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, kilometre);
      }
  };
  inline constexpr kilometre_factory km_ {};

  /// Factory for centimetre quantities
  struct centimetre_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, centimetre);
      }
  };
  inline constexpr centimetre_factory cm_ {};

  /// Factory for millimetre quantities
  struct millimetre_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, millimetre);
      }
  };
  inline constexpr millimetre_factory mm_ {};

  // -----------------------------------------------------------------------------
  // Time factories
  // -----------------------------------------------------------------------------

  /// Factory for second quantities
  struct second_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, second);
      }
  };
  inline constexpr second_factory s_ {};

  /// Factory for millisecond quantities
  struct millisecond_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, millisecond);
      }
  };
  inline constexpr millisecond_factory ms_ {};

  // -----------------------------------------------------------------------------
  // Mass factories
  // -----------------------------------------------------------------------------

  /// Factory for kilogram quantities
  struct kilogram_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, kilogram);
      }
  };
  inline constexpr kilogram_factory kg_ {};

  /// Factory for gram quantities
  struct gram_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, gram);
      }
  };
  inline constexpr gram_factory g_ {};

  // -----------------------------------------------------------------------------
  // Derived unit factories
  // -----------------------------------------------------------------------------

  /// Factory for newton quantities
  struct newton_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, newton);
      }
  };
  inline constexpr newton_factory N_ {};

  /// Factory for joule quantities
  struct joule_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, joule);
      }
  };
  inline constexpr joule_factory J_ {};

  /// Factory for watt quantities
  struct watt_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, watt);
      }
  };
  inline constexpr watt_factory W_ {};

  /// Factory for pascal quantities
  struct pascal_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, pascal);
      }
  };
  inline constexpr pascal_factory Pa_ {};

  /// Factory for hertz quantities
  struct hertz_factory {
      template<typename Rep>
      [[nodiscard]]
      static constexpr auto operator()(Rep val) noexcept {
        return make_quantity(val, hertz);
      }
  };
  inline constexpr hertz_factory Hz_ {};

  // =============================================================================
  // User-defined literals
  // =============================================================================

  namespace literals {

    // -----------------------------------------------------------------------------
    // Length literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for metres (integer)
    [[nodiscard]]
    consteval auto operator""_m(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), metre);
    }

    /// User-defined literal for metres (floating point)
    [[nodiscard]]
    consteval auto operator""_m(long double val) noexcept {
      return make_quantity(static_cast<double>(val), metre);
    }

    /// User-defined literal for kilometres (integer)
    [[nodiscard]]
    consteval auto operator""_km(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), kilometre);
    }

    /// User-defined literal for kilometres (floating point)
    [[nodiscard]]
    consteval auto operator""_km(long double val) noexcept {
      return make_quantity(static_cast<double>(val), kilometre);
    }

    /// User-defined literal for centimetres (integer)
    [[nodiscard]]
    consteval auto operator""_cm(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), centimetre);
    }

    /// User-defined literal for centimetres (floating point)
    [[nodiscard]]
    consteval auto operator""_cm(long double val) noexcept {
      return make_quantity(static_cast<double>(val), centimetre);
    }

    /// User-defined literal for millimetres (integer)
    [[nodiscard]]
    consteval auto operator""_mm(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), millimetre);
    }

    /// User-defined literal for millimetres (floating point)
    [[nodiscard]]
    consteval auto operator""_mm(long double val) noexcept {
      return make_quantity(static_cast<double>(val), millimetre);
    }

    // -----------------------------------------------------------------------------
    // Time literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for seconds (integer)
    [[nodiscard]]
    consteval auto operator""_s(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), second);
    }

    /// User-defined literal for seconds (floating point)
    [[nodiscard]]
    consteval auto operator""_s(long double val) noexcept {
      return make_quantity(static_cast<double>(val), second);
    }

    /// User-defined literal for milliseconds (integer)
    [[nodiscard]]
    consteval auto operator""_ms(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), millisecond);
    }

    /// User-defined literal for milliseconds (floating point)
    [[nodiscard]]
    consteval auto operator""_ms(long double val) noexcept {
      return make_quantity(static_cast<double>(val), millisecond);
    }

    // -----------------------------------------------------------------------------
    // Mass literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for kilograms (integer)
    [[nodiscard]]
    consteval auto operator""_kg(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), kilogram);
    }

    /// User-defined literal for kilograms (floating point)
    [[nodiscard]]
    consteval auto operator""_kg(long double val) noexcept {
      return make_quantity(static_cast<double>(val), kilogram);
    }

    /// User-defined literal for grams (integer)
    [[nodiscard]]
    consteval auto operator""_g(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), gram);
    }

    /// User-defined literal for grams (floating point)
    [[nodiscard]]
    consteval auto operator""_g(long double val) noexcept {
      return make_quantity(static_cast<double>(val), gram);
    }

    // -----------------------------------------------------------------------------
    // Derived unit literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for newtons (integer)
    [[nodiscard]]
    consteval auto operator""_N(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), newton);
    }

    /// User-defined literal for newtons (floating point)
    [[nodiscard]]
    consteval auto operator""_N(long double val) noexcept {
      return make_quantity(static_cast<double>(val), newton);
    }

    /// User-defined literal for joules (integer)
    [[nodiscard]]
    consteval auto operator""_J(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), joule);
    }

    /// User-defined literal for joules (floating point)
    [[nodiscard]]
    consteval auto operator""_J(long double val) noexcept {
      return make_quantity(static_cast<double>(val), joule);
    }

    /// User-defined literal for watts (integer)
    [[nodiscard]]
    consteval auto operator""_W(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), watt);
    }

    /// User-defined literal for watts (floating point)
    [[nodiscard]]
    consteval auto operator""_W(long double val) noexcept {
      return make_quantity(static_cast<double>(val), watt);
    }

    /// User-defined literal for pascals (integer)
    [[nodiscard]]
    consteval auto operator""_Pa(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), pascal);
    }

    /// User-defined literal for pascals (floating point)
    [[nodiscard]]
    consteval auto operator""_Pa(long double val) noexcept {
      return make_quantity(static_cast<double>(val), pascal);
    }

    /// User-defined literal for hertz (integer)
    [[nodiscard]]
    consteval auto operator""_Hz(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), hertz);
    }

    /// User-defined literal for hertz (floating point)
    [[nodiscard]]
    consteval auto operator""_Hz(long double val) noexcept {
      return make_quantity(static_cast<double>(val), hertz);
    }

    // -----------------------------------------------------------------------------
    // Electric current literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for amperes (integer)
    [[nodiscard]]
    consteval auto operator""_A(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), ampere);
    }

    /// User-defined literal for amperes (floating point)
    [[nodiscard]]
    consteval auto operator""_A(long double val) noexcept {
      return make_quantity(static_cast<double>(val), ampere);
    }

    // -----------------------------------------------------------------------------
    // Temperature literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for kelvin (integer)
    [[nodiscard]]
    consteval auto operator""_K(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), kelvin);
    }

    /// User-defined literal for kelvin (floating point)
    [[nodiscard]]
    consteval auto operator""_K(long double val) noexcept {
      return make_quantity(static_cast<double>(val), kelvin);
    }

    // -----------------------------------------------------------------------------
    // Amount of substance literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for moles (integer)
    [[nodiscard]]
    consteval auto operator""_mol(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), mole);
    }

    /// User-defined literal for moles (floating point)
    [[nodiscard]]
    consteval auto operator""_mol(long double val) noexcept {
      return make_quantity(static_cast<double>(val), mole);
    }

    // -----------------------------------------------------------------------------
    // Luminous intensity literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for candelas (integer)
    [[nodiscard]]
    consteval auto operator""_cd(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), candela);
    }

    /// User-defined literal for candelas (floating point)
    [[nodiscard]]
    consteval auto operator""_cd(long double val) noexcept {
      return make_quantity(static_cast<double>(val), candela);
    }

    // -----------------------------------------------------------------------------
    // Angle literals
    // -----------------------------------------------------------------------------

    /// User-defined literal for radians (integer)
    [[nodiscard]]
    consteval auto operator""_rad(unsigned long long val) noexcept {
      return make_quantity(static_cast<double>(val), radian);
    }

    /// User-defined literal for radians (floating point)
    [[nodiscard]]
    consteval auto operator""_rad(long double val) noexcept {
      return make_quantity(static_cast<double>(val), radian);
    }

  } // namespace literals

} // namespace units::si
