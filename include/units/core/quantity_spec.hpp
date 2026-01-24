// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <type_traits>

namespace units {

  // =============================================================================
  // Quantity character - physical nature of the quantity
  // =============================================================================

  /// Represents the mathematical character of a physical quantity
  enum class quantity_character : unsigned char {
    scalar, // Magnitude only (e.g., mass, temperature)
    vector, // Magnitude and direction (e.g., velocity, force)
    tensor  // Multi-dimensional array (e.g., stress tensor)
  };

  // =============================================================================
  // Forward declarations
  // =============================================================================

  template<typename T>
  struct is_quantity_spec;

  template<typename T>
  concept QuantitySpec = is_quantity_spec<T>::value;

  // =============================================================================
  // Quantity specification base - uses C++23 deducing this
  // =============================================================================

  namespace detail {

    /// Tag to mark root quantity specs (no parent)
    struct root_quantity_spec_tag {};

    /// Check if a type is a root (base) quantity spec
    template<typename T>
    struct is_root_quantity_spec : std::false_type {};

    template<typename T>
      requires requires { typename T::parent_spec; }
            && std::same_as<typename T::parent_spec, root_quantity_spec_tag>
    struct is_root_quantity_spec<T> : std::true_type {};

    /// Check if Child is the same as or derives from Parent in the quantity hierarchy
    template<typename Child, typename Parent>
    struct is_child_of_impl : std::false_type {};

    // Same type is always a match
    template<typename T>
    struct is_child_of_impl<T, T> : std::true_type {};

    // Recursively check parent chain
    template<typename Child, typename Parent>
      requires(!std::same_as<Child, Parent>) && requires { typename Child::parent_spec; }
           && (!std::same_as<typename Child::parent_spec, root_quantity_spec_tag>)
    struct is_child_of_impl<Child, Parent>
        : is_child_of_impl<typename Child::parent_spec, Parent> {};

  } // namespace detail

  /// Check if Child is the same as or a child of Parent in the quantity hierarchy
  template<typename Child, typename Parent>
  inline constexpr bool is_child_of_v = detail::is_child_of_impl<Child, Parent>::value;

  /// Check if type is a root (base) quantity spec
  template<typename T>
  inline constexpr bool is_root_quantity_spec_v = detail::is_root_quantity_spec<T>::value;

  // =============================================================================
  // Derived quantity spec - represents expressions like length/time
  // =============================================================================

  namespace detail {

    /// Tag for numerator terms in a derived quantity expression
    template<typename... Ts>
    struct numerator {
        using types = numerator;
    };

    /// Tag for denominator terms in a derived quantity expression
    template<typename... Ts>
    struct denominator {
        using types = denominator;
    };

    /// Flatten nested derived specs for canonicalization
    template<typename T>
    struct flatten_spec {
        using num = numerator<T>;
        using den = denominator<>;
    };

    // Forward declaration
    template<typename Num, typename Den>
    struct derived_quantity_spec;

    template<typename... NumTs, typename... DenTs>
    struct flatten_spec<derived_quantity_spec<numerator<NumTs...>, denominator<DenTs...>>> {
        using num = numerator<NumTs...>;
        using den = denominator<DenTs...>;
    };

    /// Merge two numerator lists
    template<typename N1, typename N2>
    struct merge_numerators;

    template<typename... T1s, typename... T2s>
    struct merge_numerators<numerator<T1s...>, numerator<T2s...>> {
        using type = numerator<T1s..., T2s...>;
    };

    /// Merge two denominator lists
    template<typename D1, typename D2>
    struct merge_denominators;

    template<typename... T1s, typename... T2s>
    struct merge_denominators<denominator<T1s...>, denominator<T2s...>> {
        using type = denominator<T1s..., T2s...>;
    };

    /// Convert numerator to denominator (for division)
    template<typename N>
    struct as_denominator;

    template<typename... Ts>
    struct as_denominator<numerator<Ts...>> {
        using type = denominator<Ts...>;
    };

    /// Convert denominator to numerator (for division)
    template<typename D>
    struct as_numerator;

    template<typename... Ts>
    struct as_numerator<denominator<Ts...>> {
        using type = numerator<Ts...>;
    };

  } // namespace detail

  /// A quantity spec derived from an expression (e.g., length / time)
  /// Num and Den are detail::numerator<...> and detail::denominator<...>
  template<typename Num, typename Den>
  struct derived_quantity_spec {
      using parent_spec       = detail::root_quantity_spec_tag;
      using numerator_specs   = Num;
      using denominator_specs = Den;

      static constexpr quantity_character character = quantity_character::scalar;

      /// Multiply with another quantity spec
      template<typename Self, typename Other>
        requires is_quantity_spec<std::remove_cvref_t<Other>>::value
      constexpr auto operator*(this Self /*self*/, Other /*other*/) {
        using self_flat  = detail::flatten_spec<std::remove_cvref_t<Self>>;
        using other_flat = detail::flatten_spec<std::remove_cvref_t<Other>>;

        using new_num = typename detail::merge_numerators<typename self_flat::num,
                                                          typename other_flat::num>::type;

        using new_den = typename detail::merge_denominators<typename self_flat::den,
                                                            typename other_flat::den>::type;

        return derived_quantity_spec<new_num, new_den> {};
      }

      /// Divide by another quantity spec
      template<typename Self, typename Other>
        requires is_quantity_spec<std::remove_cvref_t<Other>>::value
      constexpr auto operator/(this Self /*self*/, Other /*other*/) {
        using self_flat  = detail::flatten_spec<std::remove_cvref_t<Self>>;
        using other_flat = detail::flatten_spec<std::remove_cvref_t<Other>>;

        using new_num = typename detail::merge_numerators<
            typename self_flat::num,
            typename detail::as_numerator<typename other_flat::den>::type>::type;

        using new_den = typename detail::merge_denominators<
            typename self_flat::den,
            typename detail::as_denominator<typename other_flat::num>::type>::type;

        return derived_quantity_spec<new_num, new_den> {};
      }
  };

  // =============================================================================
  // Base quantity_spec template
  // =============================================================================

  /// Base class for all quantity specifications.
  /// Uses C++23 deducing `this` for clean inheritance without CRTP.
  ///
  /// @tparam Parent The parent quantity spec type, or void for root specs
  /// @tparam Character The physical character (scalar, vector, tensor)
  template<typename Parent = void, quantity_character Character = quantity_character::scalar>
  struct quantity_spec {
      /// Parent in the quantity hierarchy (or root_quantity_spec_tag for base specs)
      using parent_spec
          = std::conditional_t<std::is_void_v<Parent>, detail::root_quantity_spec_tag, Parent>;

      /// Physical character of this quantity
      static constexpr quantity_character character = Character;

      /// Multiply two quantity specs: Q1 * Q2
      /// Uses C++23 deducing this
      template<typename Self, typename Other>
        requires is_quantity_spec<std::remove_cvref_t<Other>>::value
      constexpr auto operator*(this Self /*self*/, Other /*other*/) {
        using self_flat  = detail::flatten_spec<std::remove_cvref_t<Self>>;
        using other_flat = detail::flatten_spec<std::remove_cvref_t<Other>>;

        using new_num = typename detail::merge_numerators<typename self_flat::num,
                                                          typename other_flat::num>::type;

        using new_den = typename detail::merge_denominators<typename self_flat::den,
                                                            typename other_flat::den>::type;

        return derived_quantity_spec<new_num, new_den> {};
      }

      /// Divide two quantity specs: Q1 / Q2
      /// Uses C++23 deducing this
      template<typename Self, typename Other>
        requires is_quantity_spec<std::remove_cvref_t<Other>>::value
      constexpr auto operator/(this Self /*self*/, Other /*other*/) {
        using self_flat  = detail::flatten_spec<std::remove_cvref_t<Self>>;
        using other_flat = detail::flatten_spec<std::remove_cvref_t<Other>>;

        // Division: multiply numerator by other's denominator,
        // and multiply denominator by other's numerator
        // Convert types appropriately
        using new_num = typename detail::merge_numerators<
            typename self_flat::num,
            typename detail::as_numerator<typename other_flat::den>::type>::type;

        using new_den = typename detail::merge_denominators<
            typename self_flat::den,
            typename detail::as_denominator<typename other_flat::num>::type>::type;

        return derived_quantity_spec<new_num, new_den> {};
      }
  };

  // =============================================================================
  // Quantity spec type trait
  // =============================================================================

  namespace detail {

    template<typename T, typename = void>
    struct has_quantity_spec_interface : std::false_type {};

    template<typename T>
    struct has_quantity_spec_interface<T,
                                       std::void_t<typename T::parent_spec, decltype(T::character)>>
        : std::true_type {};

  } // namespace detail

  /// Type trait to check if T is a quantity specification
  template<typename T>
  struct is_quantity_spec : detail::has_quantity_spec_interface<std::remove_cvref_t<T>> {};

  template<typename T>
  inline constexpr bool is_quantity_spec_v = is_quantity_spec<T>::value;

  // =============================================================================
  // Derived quantity spec type traits
  // =============================================================================

  /// Check if a quantity spec is derived (from an expression)
  template<typename T>
  struct is_derived_quantity_spec : std::false_type {};

  template<typename Num, typename Den>
  struct is_derived_quantity_spec<derived_quantity_spec<Num, Den>> : std::true_type {};

  template<typename T>
  inline constexpr bool is_derived_quantity_spec_v = is_derived_quantity_spec<T>::value;

  // =============================================================================
  // Kind - wrapper to associate quantities with different "kinds"
  // =============================================================================

  /// Wrapper to mark a quantity spec as the "kind" for a unit definition.
  /// Different kinds with the same dimension are not interchangeable
  /// (e.g., torque vs energy).
  template<QuantitySpec Q>
  struct kind_of {
      using spec                                    = Q;
      static constexpr quantity_character character = Q::character;
  };

  /// Check if T is a kind_of wrapper
  template<typename T>
  struct is_kind_of : std::false_type {};

  template<QuantitySpec Q>
  struct is_kind_of<kind_of<Q>> : std::true_type {};

  template<typename T>
  inline constexpr bool is_kind_of_v = is_kind_of<T>::value;

  // =============================================================================
  // Quantity spec concepts
  // =============================================================================

  /// Concept for types that are derived quantity specs
  template<typename T>
  concept DerivedQuantitySpec = is_derived_quantity_spec_v<T>;

  /// Concept for quantity specs with scalar character
  template<typename T>
  concept ScalarQuantitySpec = QuantitySpec<T> && T::character == quantity_character::scalar;

  /// Concept for quantity specs with vector character
  template<typename T>
  concept VectorQuantitySpec = QuantitySpec<T> && T::character == quantity_character::vector;

  /// Concept for quantity specs with tensor character
  template<typename T>
  concept TensorQuantitySpec = QuantitySpec<T> && T::character == quantity_character::tensor;

  /// Concept checking if Child is the same as or a subtype of Parent
  template<typename Child, typename Parent>
  concept QuantitySpecOf
      = QuantitySpec<Child> && QuantitySpec<Parent> && is_child_of_v<Child, Parent>;

  // =============================================================================
  // Compatibility predicates
  // =============================================================================

  namespace detail {

    /// Implementation of implicitly_convertible
    template<typename From, typename To>
    struct implicitly_convertible_impl : std::false_type {};

    // Same type is always convertible
    template<typename T>
    struct implicitly_convertible_impl<T, T> : std::true_type {};

    // Child -> Parent is implicit (width -> length)
    template<typename From, typename To>
      requires(!std::same_as<From, To>) && is_child_of_v<From, To>
    struct implicitly_convertible_impl<From, To> : std::true_type {};

    /// Implementation of explicitly_convertible
    template<typename From, typename To>
    struct explicitly_convertible_impl {
        // Explicit conversion allowed if either direction is a child
        static constexpr bool value = is_child_of_v<From, To> || is_child_of_v<To, From>;
    };

    /// Find common ancestor in quantity spec hierarchy
    template<typename Q1, typename Q2, typename = void>
    struct common_quantity_spec_impl;

    // Same type - common is the type itself
    template<typename T>
    struct common_quantity_spec_impl<T, T> {
        using type = T;
    };

    // Q1 is child of Q2 - common is Q2
    template<typename Q1, typename Q2>
      requires(!std::same_as<Q1, Q2>) && is_child_of_v<Q1, Q2>
    struct common_quantity_spec_impl<Q1, Q2> {
        using type = Q2;
    };

    // Q2 is child of Q1 - common is Q1
    template<typename Q1, typename Q2>
      requires(!std::same_as<Q1, Q2>) && (!is_child_of_v<Q1, Q2>) && is_child_of_v<Q2, Q1>
    struct common_quantity_spec_impl<Q1, Q2> {
        using type = Q1;
    };

    // Neither is child of other - try finding common ancestor through parents
    template<typename Q1, typename Q2>
      requires(!std::same_as<Q1, Q2>) && (!is_child_of_v<Q1, Q2>) && (!is_child_of_v<Q2, Q1>)
           && requires { typename Q1::parent_spec; }
           && (!std::same_as<typename Q1::parent_spec, root_quantity_spec_tag>)
    struct common_quantity_spec_impl<Q1, Q2>
        : common_quantity_spec_impl<typename Q1::parent_spec, Q2> {};

  } // namespace detail

  /// Check if From can be implicitly converted to To.
  /// True when From is the same as or a child of To.
  template<typename From, typename To>
  inline constexpr bool implicitly_convertible_v
      = detail::implicitly_convertible_impl<From, To>::value;

  /// Check if From can be explicitly converted to To.
  /// True when either is a child of the other.
  template<typename From, typename To>
  inline constexpr bool explicitly_convertible_v
      = detail::explicitly_convertible_impl<From, To>::value;

  /// Find the common quantity spec (first common ancestor) of Q1 and Q2.
  /// Used to determine the result type of operations like width + height -> length.
  template<QuantitySpec Q1, QuantitySpec Q2>
  using common_quantity_spec = typename detail::common_quantity_spec_impl<Q1, Q2>::type;

// =============================================================================
// Macro for defining quantity specs (optional convenience)
// =============================================================================

/// Helper macro for defining named quantity specifications.
/// Creates both the type and a constexpr instance.
#define UNITS_QUANTITY_SPEC(name, ...)                            \
  struct name##_t final : ::units::quantity_spec<__VA_ARGS__> {}; \
  inline constexpr name##_t name {}

} // namespace units
