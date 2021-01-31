#pragma once

#include <concepts>
#include <cstddef>
#include <system_error>
#include <type_traits>

#include "more_concepts/detail/type_traits.hpp"

namespace more_concepts
{
    /// Types that are non-reference, non-c-array, non-function or function reference,
    /// non-const and non-volatile.
    ///
    /// Assigning an object of this type to an auto variable preserves the type.
    template <typename T>
    concept decayed = std::same_as<T, std::decay_t<T>>;

    /// Types that support aggregate initialization:
    /// https://en.cppreference.com/w/cpp/language/aggregate_initialization
    template <typename T>
    concept aggregate = std::is_aggregate_v<T>;

    /// Types which can be trivially constructed and destructed (as in without performing
    /// any (de)initialization at all), and can be trivially copied (as in copying is equivalent
    /// to calling memcpy).
    template <typename T>
    concept trivial = std::is_trivial_v<T>;

    /// Type is a scoped or unscoped enumeration (enum / enum class).
    template <typename T>
    concept enum_type = std::is_enum_v<T>;

    /// An enum type that represents an error, and can be used to construct a std::error_code
    /// object.
    template <typename T>
    concept error_code_enum = enum_type<T> and std::is_error_code_enum_v<T>;

    /// An enum type that represents an error, and can be used to construct a std::error_condition
    /// object.
    template <typename T>
    concept error_condition_enum = enum_type<T> and std::is_error_condition_enum_v<T>;

    /// Types that can be called with std::invoke using one or more function signatures.
    /// The return type of each signature is only checked for convertibility.
    template <typename Fn, typename... Signatures>
    concept invocable_as =
    requires(Signatures& ... signatures)
    {
        ([] <typename Ret, typename... Args>(auto(&)(Args...) -> Ret)
        requires std::is_invocable_r_v<Ret, Fn, Args>
        {}(signatures), ...);
    };

    /// Types that can be called with the function call operator using one or more function
    /// signatures. The return type of each signature must match exactly.
    template <typename Fn, typename... Signatures>
    concept callable_as =
    requires(Signatures& ... signatures)
    {
        // Call operator checking is deferred to a type trait;
        // doing it inline breaks the compiler (GCC 10.2)
        ([] <typename Ret, typename... Args>(auto(&)(Args...) -> Ret)
        requires detail::is_callable_r_v<Ret, Fn, Args...>
        {}(signatures), ...);
    };

    /// From https://en.cppreference.com/w/cpp/named_req/Hash:
    /// A Hash is a function object for which the output depends only on the input
    /// and has a very low probability of yielding the same output given different input values.
    ///
    /// Used to define the unordered_associative_container concept.
    template <typename Fn, typename KeyType>
    concept hash_function
    = callable_as<
          Fn const,
          auto(KeyType&) -> std::size_t,
          auto(KeyType const&) -> std::size_t>;
}
