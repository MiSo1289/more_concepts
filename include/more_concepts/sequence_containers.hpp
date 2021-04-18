#pragma once

#include <concepts>
#include <initializer_list>
#include <iterator>

#include "more_concepts/base_containers.hpp"
#include "more_concepts/mock_iterator.hpp"

namespace more_concepts
{
    /// From https://en.cppreference.com/w/cpp/named_req/SequenceContainer:
    /// A SequenceContainer is a Container that stores objects of the same type
    /// in a linear arrangement.
    ///
    /// Satisfied by all standard sequence containers.
    template <typename C>
    concept sequence_container
    = container<C> and
      requires(C& cont, C const& const_cont)
      {
          { cont.front() } -> std::same_as<typename C::reference>;
          { const_cont.front() } -> std::same_as<typename C::const_reference>;

          // Sequences of ordered values should provide a lexicographical ordering
          requires not std::totally_ordered<typename C::value_type> or
                   std::totally_ordered<C>;
      };

    /// A sized and reversible sequence container that allows efficient access at both ends.
    /// Satisfied by vector, array, basic_string, deque and list.
    template <typename C>
    concept double_ended_container
    = sequence_container<C> and
      sized_container<C> and
      reversible_container<C> and
      requires(C& cont, C const& const_cont)
      {
          { cont.back() } -> std::same_as<typename C::reference>;
          { const_cont.back() } -> std::same_as<typename C::const_reference>;
      };

    /// A double-ended container that allows indexed access.
    /// Satisfied by vector, array, basic_string and deque.
    template <typename C>
    concept random_access_container
    = double_ended_container<C> and
      requires(C& cont, C const& const_cont, typename C::size_type const idx)
      {
          requires std::random_access_iterator<typename C::iterator>;
          requires std::random_access_iterator<typename C::const_iterator>;

          // Unchecked indexing
          { cont[idx] } -> std::same_as<typename C::reference>;
          { const_cont[idx] } -> std::same_as<typename C::const_reference>;

          // Bounds-checked indexing
          { cont.at(idx) } -> std::same_as<typename C::reference>;
          { const_cont.at(idx) } -> std::same_as<typename C::const_reference>;
      };

    /// A random access container that stores elements in a contiguous memory region.
    /// Satisfied by vector, array and basic_string.
    template <typename C>
    concept contiguous_container
    = random_access_container<C> and
    requires(C& cont, C const& const_cont)
    {
        requires std::contiguous_iterator<typename C::iterator>;
        requires std::contiguous_iterator<typename C::const_iterator>;

        typename C::pointer;
        typename C::const_pointer;
        requires std::contiguous_iterator<typename C::pointer>;
        requires std::contiguous_iterator<typename C::const_pointer>;
        requires std::convertible_to<typename C::pointer, typename C::const_pointer>;

        { cont.data() } -> std::same_as<typename C::pointer>;
        { const_cont.data() } -> std::same_as<typename C::const_pointer>;
    };

    /// A sequence container that allows resizing, range construction and assignment,
    /// and insertion / erasure in the middle.
    /// Satisfied by vector, basic_string, deque and list.
    template <typename C>
    concept resizable_sequence_container
    = double_ended_container<C> and
      clearable_container<C> and
      std::default_initializable<C> and
      requires(C& cont, typename C::const_iterator const& pos)
      {
          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(
                       typename C::size_type const& size,
                       typename C::value_type const& value,
                       mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& first,
                       mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& last,
                       std::initializer_list<typename C::value_type> const& init_list)
                   {
                       // Copy insertion
                       { cont.insert(pos, value) } -> std::same_as<typename C::iterator>;
                       { cont.insert(pos, first, last) } -> std::same_as<typename C::iterator>;
                       { cont.insert(pos, init_list) } -> std::same_as<typename C::iterator>;

                       // Range construction and assignment
                       C(first, last);
                       C(init_list);
                       C(size, value);
                       cont = init_list;
                       cont.assign(first, last);
                       cont.assign(init_list);
                       cont.assign(size, value);

                       // Resizing
                       cont.resize(size);
                       cont.resize(size, value);
                   };

          // Move insertion
          requires not std::movable<typename C::value_type> or
                   requires(typename C::value_type&& value)
                   {{ cont.insert(pos, std::move(value)) } -> std::same_as<typename C::iterator>; };

          // Erasure
          requires
          requires(
              typename C::const_iterator const& first,
              typename C::const_iterator const& last)
          {
              { cont.erase(pos) } -> std::same_as<typename C::iterator>;
              { cont.erase(first, last) } -> std::same_as<typename C::iterator>;
          };
      };

    /// A resizable container that also allows in-place element construction.
    /// Satisfied by vector, deque and list.
    ///
    /// Note: the container is required to support .emplace(const_iterator, Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename C>
    concept inplace_constructing_sequence_container
    = resizable_sequence_container<C> and
      requires(C& cont, typename C::const_iterator const& pos)
      {
          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires {{ cont.emplace(pos) } -> std::same_as<typename C::iterator>; };

          // Emplacement (copy constructor)
          requires not std::copyable<typename C::value_type> or
                   requires(typename C::value_type const& value)
                   {{ cont.emplace(pos, value) } -> std::same_as<typename C::iterator>; };

          // Emplacement (move constructor)
          requires not std::movable<typename C::value_type> or
                   requires(typename C::value_type&& value)
                   {{ cont.emplace(pos, std::move(value)) } -> std::same_as<typename C::iterator>; };
      };

    /// A sequence container that allows efficient insertion and erasure at the front.
    /// Satisfied by deque, list and forward_list.
    template <typename C>
    concept front_growable_container
    = sequence_container<C> and
      std::default_initializable<C> and
      requires(C& cont, typename C::value_type const& value, typename C::value_type&& tmp_value)
      {
          // Front copy insertion
          requires not std::copyable<typename C::value_type> or
                   requires { cont.push_front(value); };

          // Front move insertion
          requires not std::movable<typename C::value_type> or
                   requires { cont.push_front(std::move(tmp_value)); };

          // Front erasure
          cont.pop_front();
      };

    /// A front-growable container that also allows inplace front construction.
    /// Satisfied by deque, list and forward_list.
    ///
    /// Note: the container is required to support .emplace_front(Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename C>
    concept inplace_front_constructing_container
    = front_growable_container<C> and
      requires(C& cont, typename C::value_type const& value, typename C::value_type&& tmp_value)
      {
          // Front emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires {{ cont.emplace_front() } -> std::same_as<typename C::reference>; };

          // Front emplacement (copy constructor)
          requires not std::copyable<typename C::value_type> or
                   requires {{ cont.emplace_front(value) } -> std::same_as<typename C::reference>; };

          // Front emplacement (move constructor)
          requires not std::movable<typename C::value_type> or
                   requires {{ cont.emplace_front(std::move(tmp_value)) } -> std::same_as<typename C::reference>; };
      };

    /// A double-ended container that allows efficient insertion and erasure at the back.
    /// Satisfied by vector, basic_string, deque, and list.
    template <typename C>
    concept back_growable_container
    = double_ended_container<C> and
      std::default_initializable<C> and
      requires(C& cont, typename C::value_type const& value, typename C::value_type&& tmp_value)
      {
          // Back copy insertion
          requires not std::copyable<typename C::value_type> or
                   requires { cont.push_back(value); };

          // Back move insertion
          requires not std::movable<typename C::value_type> or
                   requires { cont.push_back(std::move(tmp_value)); };

          // Back erasure
          cont.pop_back();
      };

    /// A back-growable container that also allows inplace back construction.
    /// Satisfied by vector, deque, and list.
    ///
    /// Note: the container is required to support .emplace_back(Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename C>
    concept inplace_back_constructing_container
    = back_growable_container<C> and
      requires(C& cont, typename C::value_type const& value, typename C::value_type&& tmp_value)
      {
          // Back emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires {{ cont.emplace_back() } -> std::same_as<typename C::reference>; };

          // Back emplacement (copy constructor)
          requires not std::copyable<typename C::value_type> or
                   requires {{ cont.emplace_back(value) } -> std::same_as<typename C::reference>; };

          // Back emplacement (move constructor)
          requires not std::movable<typename C::value_type> or
                   requires {{ cont.emplace_back(std::move(tmp_value)) } -> std::same_as<typename C::reference>; };
      };

    template <typename C, typename ValueType>
    concept sequence_container_of = container_of<C, ValueType> and sequence_container<C>;

    template <typename C, typename ValueType>
    concept double_ended_container_of = container_of<C, ValueType> and double_ended_container<C>;

    template <typename C, typename ValueType>
    concept random_access_container_of = container_of<C, ValueType> and random_access_container<C>;

    template <typename C, typename ValueType>
    concept contiguous_container_of = container_of<C, ValueType> and contiguous_container<C>;

    template <typename C, typename ValueType>
    concept resizable_sequence_container_of = container_of<C, ValueType> and resizable_sequence_container<C>;

    template <typename C, typename ValueType>
    concept inplace_constructing_sequence_container_of
    = container_of<C, ValueType> and inplace_back_constructing_container<C>;

    template <typename C, typename ValueType>
    concept front_growable_container_of = container_of<C, ValueType> and front_growable_container<C>;

    template <typename C, typename ValueType>
    concept inplace_front_constructing_container_of
    = container_of<C, ValueType> and inplace_front_constructing_container<C>;

    template <typename C, typename ValueType>
    concept back_growable_container_of = container_of<C, ValueType> and back_growable_container<C>;

    template <typename C, typename ValueType>
    concept inplace_back_constructing_container_of
    = container_of<C, ValueType> and inplace_back_constructing_container<C>;
}
