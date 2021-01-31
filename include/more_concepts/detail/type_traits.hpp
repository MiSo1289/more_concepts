#pragma once

#include <type_traits>

namespace more_concepts::detail
{
    template <typename Ret, typename Fn, typename... Args>
    struct is_callable_r : std::false_type
    {
    };

    template <typename Ret, typename Fn, typename... Args>
    requires
    requires(Fn& fn, Args&& ... args)
    {{ static_cast<Fn&>(fn)(std::forward<Args>(args)...) } -> std::same_as<Ret>; }
    struct is_callable_r<Ret, Fn, Args...> : std::true_type
    {
    };

    template <typename Ret, typename Fn, typename... Args>
    inline constexpr auto is_callable_r_v = is_callable_r<Ret, Fn, Args...>::value;
}
