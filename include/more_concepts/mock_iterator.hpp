#pragma once

#include <iterator>
#include <type_traits>

#include "more_concepts/detail/mock_iterator.hpp"

namespace more_concepts
{
    struct mutable_iterator_tag
    {
    };
    struct const_iterator_tag
    {
    };

    /// A dummy implementation of a specific (legacy / concept-based) iterator category.
    /// Can be used to check whether a function accepts any iterator of some category.
    /// While this is not fool-proof (a function can be written to accept mock_iterator
    /// specifically), it is generally good enough.
    template <typename T, typename IteratorCategory, typename RWCategory>
    class mock_iterator final
        : public detail::mock_iterator_value_type_def<T, IteratorCategory>,
          public detail::mock_iterator_reference_def<
              T, IteratorCategory, std::same_as<RWCategory, mutable_iterator_tag>>,
          public detail::mock_iterator_element_type_def<T, IteratorCategory>
    {
      public:
        using iterator_category = IteratorCategory;
        using difference_type = std::ptrdiff_t;

        auto operator++() -> mock_iterator&;

        auto operator++(int) -> mock_iterator;

        auto operator*() const -> typename mock_iterator::deref_result;

        auto operator->() const -> typename mock_iterator::arrow_result
        requires std::derived_from<IteratorCategory, std::input_iterator_tag>;

        auto operator==(mock_iterator const&) const -> bool
        requires std::derived_from<IteratorCategory, std::input_iterator_tag>;

        auto operator--() -> mock_iterator&
        requires std::derived_from<IteratorCategory, std::bidirectional_iterator_tag>;

        auto operator--(int) -> mock_iterator
        requires std::derived_from<IteratorCategory, std::bidirectional_iterator_tag>;

        auto operator+=(difference_type) -> mock_iterator&
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator-=(difference_type) -> mock_iterator&
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator[](difference_type) const -> typename mock_iterator::deref_result
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator+(difference_type) const -> mock_iterator
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        template <std::convertible_to<difference_type> D>
        friend auto operator+(D const&, mock_iterator const&) -> mock_iterator
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator-(difference_type const&) const -> mock_iterator
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator-(mock_iterator const&) const -> difference_type
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator<(mock_iterator const&) const -> bool
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator>(mock_iterator const&) const -> bool
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator<=(mock_iterator const&) const -> bool
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;

        auto operator>=(mock_iterator const&) const -> bool
        requires std::derived_from<IteratorCategory, std::random_access_iterator_tag>;
    };

    template <typename T, typename IteratorCategory>
    using mock_const_iterator = mock_iterator<T, IteratorCategory, const_iterator_tag>;

    template <typename T, typename IteratorCategory>
    using mock_mutable_iterator = mock_iterator<T, IteratorCategory, mutable_iterator_tag>;
}
