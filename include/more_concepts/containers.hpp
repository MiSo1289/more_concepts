#pragma once

#include <concepts>
#include <iterator>
#include <limits>

namespace more_concepts
{
    // TODO Associative containers

    /// From https://en.cppreference.com/w/cpp/named_req/Container:
    /// "A Container is an object used to store other objects and taking care
    /// of the management of the memory used by the objects it contains."
    ///
    /// Satisfied by all well-behaved standard containers.
    /// Is not satisfied by vector<bool>.
    template<typename C>
    concept container =
    requires(C& cont, C const& const_cont)
    {
        typename C::value_type;
        requires std::same_as<typename C::value_type, std::decay_t<typename C::value_type>>;

        typename C::reference;
        typename C::const_reference;
        // Well-behaved containers should not rely on proxies for iterators / references.
        requires std::same_as<typename C::reference, typename C::value_type&>;
        requires std::same_as<typename C::const_reference, typename C::value_type const&>;

        typename C::iterator;
        typename C::const_iterator;
        requires std::forward_iterator<typename C::iterator>;
        requires std::forward_iterator<typename C::const_iterator>;
        requires std::convertible_to<typename C::iterator, typename C::const_iterator>;
        requires std::same_as<std::iter_value_t<typename C::iterator>, typename C::value_type>;
        requires std::same_as<std::iter_value_t<typename C::const_iterator>, typename C::value_type>;
        requires std::same_as<std::iter_reference_t<typename C::iterator>, typename C::reference> or
                 std::same_as<std::iter_reference_t<typename C::iterator>, typename C::const_reference>;
        requires std::same_as<
            std::iter_reference_t<typename C::const_iterator>,
            typename C::const_reference>;

        typename C::difference_type;
        typename C::size_type;
        requires std::signed_integral<typename C::difference_type>;
        requires std::unsigned_integral<typename C::size_type>;
        // size_type should be able to represent all positive values of difference_type.
        requires (static_cast<typename C::difference_type>(
                      static_cast<typename C::size_type>(
                          std::numeric_limits<typename C::difference_type>::max()))
                  == std::numeric_limits<typename C::difference_type>::max());
        // difference_type should be the same as the one defined by iterator_traits.
        requires std::same_as<
            typename C::difference_type,
            typename std::iterator_traits<typename C::iterator>::difference_type>;
        requires std::same_as<
            typename C::difference_type,
            typename std::iterator_traits<typename C::const_iterator>::difference_type>;

        // If the value_type is regular, the container should be regular as well.
        requires (not std::regular<typename C::value_type>) or
                 std::regular<C>;

        { cont.begin() } -> std::same_as<typename C::iterator>;
        { cont.end() } -> std::same_as<typename C::iterator>;
        { const_cont.begin() } -> std::same_as<typename C::const_iterator>;
        { const_cont.end() } -> std::same_as<typename C::const_iterator>;
        { cont.cbegin() } -> std::same_as<typename C::const_iterator>;
        { cont.cend() } -> std::same_as<typename C::const_iterator>;

        { const_cont.max_size() } -> std::same_as<typename C::size_type>;
        { const_cont.empty() } -> std::convertible_to<bool>;
    };

    /// A container that allows mutable access to its elements.
    /// Satisfied by all standard containers except set.
    template<typename C>
    concept mutable_container
    = container<C> and
      std::same_as<std::iter_reference_t<typename C::iterator>, typename C::reference>;

    /// A container that knows its size.
    /// Satisfied by all standard containers except forward_list.
    template<typename C>
    concept sized_container
    = container<C> and
      requires(C const& const_cont)
      {
          { const_cont.size() } -> std::same_as<typename C::size_type>;
      };

    /// A container that can be cleared.
    /// Satisfied by all standard containers except array.
    template<typename C>
    concept clearable_container
    = container<C> and
      requires(C& cont)
      {
          cont.clear();
      };

    /// A container that allows reverse iteration.
    /// Satisfied by all standard containers except forward_list.
    template<typename C>
    concept reversible_container
    = container<C> and
      requires(C& cont, C const& const_cont)
      {
          requires std::bidirectional_iterator<typename C::iterator>;
          requires std::bidirectional_iterator<typename C::const_iterator>;

          typename C::reverse_iterator;
          typename C::const_reverse_iterator;
          requires std::bidirectional_iterator<typename C::reverse_iterator>;
          requires std::bidirectional_iterator<typename C::const_reverse_iterator>;

          { cont.rbegin() } -> std::same_as<typename C::reverse_iterator>;
          { cont.rend() } -> std::same_as<typename C::reverse_iterator>;
          { const_cont.rbegin() } -> std::same_as<typename C::const_reverse_iterator>;
          { const_cont.rend() } -> std::same_as<typename C::const_reverse_iterator>;
          { cont.crbegin() } -> std::same_as<typename C::const_reverse_iterator>;
          { cont.crend() } -> std::same_as<typename C::const_reverse_iterator>;
      };
}
