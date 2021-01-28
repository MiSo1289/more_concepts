#pragma once

#include <iterator>
#include <type_traits>

namespace more_concepts::detail
{
    template<typename T, bool readable, bool writable>
    class mock_iterator_proxy_reference
    {
      public:
        auto operator=(T const&) -> mock_iterator_proxy_reference&
        requires writable;

        auto operator=(T&&) -> mock_iterator_proxy_reference&
        requires writable;

        operator T()
        requires readable;

        auto operator->() -> T const*
        requires readable;

        auto operator->() -> T*
        requires readable and writable;
    };

    template<typename T, typename IteratorCategory>
    struct mock_iterator_value_type_def
    {
    };

    template<typename T>
    struct mock_iterator_value_type_def<T, std::output_iterator_tag>
    {
        using value_type = void;
    };

    template<typename T, std::derived_from<std::input_iterator_tag> IteratorCategory>
    struct mock_iterator_value_type_def<T, IteratorCategory>
    {
        using value_type = T;
    };

    template<typename T, typename IteratorCategory, bool writable>
    struct mock_iterator_reference_def
    {
    };

    template<typename T>
    struct mock_iterator_reference_def<T, std::output_iterator_tag, true>
    {
        using reference = void;

      protected:
        using deref_result = mock_iterator_proxy_reference<T, false, true>;
        using arrow_result = void;
    };

    template<typename T, bool writable>
    struct mock_iterator_reference_def<T, std::input_iterator_tag, writable>
    {
        using reference = mock_iterator_proxy_reference<T, true, writable>;

      protected:
        using deref_result = reference;
        using arrow_result = reference;
    };

    template<typename T, std::derived_from<std::forward_iterator_tag> IteratorCategory>
    struct mock_iterator_reference_def<T, IteratorCategory, true>
    {
        using reference = T&;

      protected:
        using deref_result = reference;
        using arrow_result = T*;
    };

    template<typename T, std::derived_from<std::forward_iterator_tag> IteratorCategory>
    struct mock_iterator_reference_def<T, IteratorCategory, false>
    {
        using reference = T const&;

      protected:
        using deref_result = reference;
        using arrow_result = T const*;
    };

    template<typename T, typename IteratorCategory>
    struct mock_iterator_element_type_def {};

    template<typename T, std::derived_from<std::contiguous_iterator_tag> IteratorCategory>
    struct mock_iterator_element_type_def<T, IteratorCategory>
    {
        //using element_type = T;
    };
}
