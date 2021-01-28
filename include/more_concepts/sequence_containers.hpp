#pragma once

#include <concepts>
#include <initializer_list>
#include <iterator>
#include <vector>

#include "more_concepts/containers.hpp"
#include "more_concepts/mock_iterator.hpp"

namespace more_concepts
{
    /// From https://en.cppreference.com/w/cpp/named_req/SequenceContainer:
    /// A SequenceContainer is a Container that stores objects of the same type
    /// in a linear arrangement.
    ///
    /// Satisfied by all standard sequence containers.
    template<typename C>
    concept sequence_container
    = mutable_container<C> and
      requires(C& cont, C const& const_cont)
      {
          { cont.front() } -> std::same_as<typename C::reference>;
          { const_cont.front() } -> std::same_as<typename C::const_reference>;
      };

    /// A sized sequence container that allows efficient access at both ends.
    /// Satisfied by vector, array, deque and list.
    template<typename C>
    concept double_ended_container
    = sequence_container<C> and
      sized_container<C> and
      requires(C& cont, C const& const_cont)
      {
          { cont.back() } -> std::same_as<typename C::reference>;
          { const_cont.back() } -> std::same_as<typename C::const_reference>;
      };

    /// A double-ended container that allows indexing.
    /// Satisfied by vector, array and deque.
    template<typename C>
    concept random_access_container
    = double_ended_container<C> and
      requires(C& cont, C const& const_cont, typename C::size_type const idx)
      {
          requires std::random_access_iterator<typename C::iterator>;
          requires std::random_access_iterator<typename C::const_iterator>;

          { cont[idx] } -> std::same_as<typename C::reference>;
          { const_cont[idx] } -> std::same_as<typename C::const_reference>;
          { cont.at(idx) } -> std::same_as<typename C::reference>;
          { const_cont.at(idx) } -> std::same_as<typename C::const_reference>;
      };


    /// A random access container that stores elements in a contiguous memory region.
    /// Satisfied by vector and array.
    template<typename C>
    concept contiguous_container
    = random_access_container<C> and
      requires(C& cont, C const& const_cont)
      {
          requires std::contiguous_iterator<typename C::iterator>;
          requires std::contiguous_iterator<typename C::const_iterator>;

          typename C::pointer;
          typename C::const_pointer;
          requires std::same_as<typename C::pointer, typename C::value_type*>;
          requires std::same_as<typename C::const_pointer, typename C::value_type const*>;

          { cont.data() } -> std::same_as<typename C::pointer>;
          { const_cont.data() } -> std::same_as<typename C::const_pointer>;
      };

    /// A sequence container that allows resizing, and inserting/erasing in the middle.
    /// Satisfied by vector, deque and list.
    template<typename C>
    concept resizable_container
    = sequence_container<C> and
      requires(
          C& cont,
          typename C::size_type const size,
          typename C::value_type const& elem,
          typename C::value_type&& tmp_elem,
          typename C::const_iterator const iter,
          mock_const_iterator<typename C::value_type> const input_iter,
          std::initializer_list<typename C::value_type> const init_list)
      {
          requires std::constructible_from<C, typename C::size_type, typename C::value_type>;
          requires std::constructible_from<C, decltype(input_iter), decltype(input_iter)>;
          requires std::constructible_from<C, std::initializer_list<typename C::value_type>>;
          requires std::assignable_from<C&, std::initializer_list<typename C::value_type>>;

          { cont.insert(iter, elem) } -> std::same_as<typename C::iterator>;
          { cont.insert(iter, std::move(tmp_elem)) } -> std::same_as<typename C::iterator>;
          { cont.insert(iter, input_iter, input_iter) } -> std::same_as<typename C::iterator>;
          { cont.insert(iter, init_list) } -> std::same_as<typename C::iterator>;

          { cont.erase(iter) } -> std::same_as<typename C::iterator>;
          { cont.erase(iter, iter) } -> std::same_as<typename C::iterator>;

          cont.clear();

          cont.assign(iter, iter);
          cont.assign(init_list);
          cont.assign(size, elem);

          cont.resize(size);
          cont.resize(size, elem);
      };

    /// A sequence container that allows efficient insertion at the front.
    /// Satisfied by deque, list and forward_list.
    template<typename C>
    concept front_growable_container
    = sequence_container<C> and
      requires(C& cont, typename C::value_type const& elem, typename C::value_type&& tmp_elem)
      {
          cont.push_front(elem);
          cont.push_front(std::move(tmp_elem));
          cont.pop_front();
      };

    /// A double-ended container that allows efficient insertion at the back.
    /// Satisfied by vector, deque and list.
    template<typename C>
    concept back_growable_container
    = double_ended_container<C> and
      requires(C& cont, typename C::value_type const& elem, typename C::value_type&& tmp_elem)
      {
          cont.push_back(elem);
          cont.push_back(std::move(tmp_elem));
          cont.pop_back();
      };
}
