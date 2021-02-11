#pragma once

#include <concepts>
#include <initializer_list>
#include <iterator>
#include <utility>

#include "more_concepts/base_concepts.hpp"
#include "more_concepts/base_containers.hpp"
#include "more_concepts/mock_iterator.hpp"

namespace more_concepts
{
    /// A container that provides fast lookup of objects based on keys.
    /// Satisfied by all standard set and map types.
    ///
    /// Note: this concept corresponds to a common subset of the standard named requirements
    /// AssociativeContainer and UnorderedAssociativeContainer.
    /// These correspond to the ordered_associative_container and
    /// unordered_associative_container concepts respectively.
    ///
    /// Note: the container is required to support .emplace_hint(const_iterator, Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename C>
    concept associative_container
    = container<C> and
      sized_container<C> and
      clearable_container<C> and
      std::default_initializable<C> and
      requires(C& cont, C const& const_cont)
      {
          typename C::key_type;

          // Searching
          requires
          requires(typename C::key_type const& key)
          {
              { const_cont.count(key) } -> std::same_as<typename C::size_type>;
              { const_cont.contains(key) } -> std::same_as<bool>;
              { cont.find(key) } -> std::same_as<typename C::iterator>;
              { const_cont.find(key) } -> std::same_as<typename C::const_iterator>;
              {
              cont.equal_range(key)
              } -> std::same_as<std::pair<typename C::iterator, typename C::iterator>>;
              {
              const_cont.equal_range(key)
              } -> std::same_as<std::pair<typename C::const_iterator, typename C::const_iterator>>;
          };

          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires(typename C::const_iterator const& hint)
                   {{ cont.emplace_hint(hint) } -> std::same_as<typename C::iterator>; };

          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(
                       typename C::value_type const& value,
                       typename C::const_iterator const& hint,
                       mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& first,
                       mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& last,
                       std::initializer_list<typename C::value_type> const& init_list)
                   {
                       // Range construction and assignment
                       C(first, last);
                       C(init_list);
                       cont = init_list;

                       // Copy insertion
                       { cont.insert(hint, value) } -> std::same_as<typename C::iterator>;

                       // Range insertion
                       cont.insert(first, last);
                       cont.insert(init_list);

                       // Emplacement (copy constructor)
                       { cont.emplace_hint(hint, value) } -> std::same_as<typename C::iterator>;
                   };

          // Move-enabled operations
          requires not std::movable<typename C::value_type> or
                   requires(
                       typename C::value_type&& tmp_value,
                       typename C::const_iterator const& hint)
                   {
                       // Move insertion
                       { cont.insert(hint, std::move(tmp_value)) } -> std::same_as<typename C::iterator>;

                       // Emplacement (move constructor)
                       { cont.emplace_hint(hint, std::move(tmp_value)) } -> std::same_as<typename C::iterator>;
                   };

          // Erasure
          requires
          requires(
              typename C::const_iterator const& pos,
              typename C::const_iterator const& first,
              typename C::const_iterator const& last,
              typename C::key_type const& key)
          {
              { cont.erase(pos) } -> std::same_as<typename C::iterator>;
              { cont.erase(first, last) } -> std::same_as<typename C::iterator>;
              { cont.erase(key) } -> std::same_as<typename C::size_type>;
          };
      };

    /// An associative container without multiple key occurrence.
    /// Satisfied by set, map, unordered_set and unordered_map.
    ///
    /// Note: the container is required to support .emplace(Args...) for every constructor
    /// of the form value_type(Args...).
    template <typename C>
    concept unique_associative_container
    = associative_container<C> and
      requires(C& cont, C const& const_cont)
      {
          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires(typename C::const_iterator const& hint)
                   {{ cont.emplace() } -> std::same_as<std::pair<typename C::iterator, bool>>; };

          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(typename C::value_type const& value)
                   {
                       // Copy insertion
                       { cont.insert(value) } -> std::same_as<std::pair<typename C::iterator, bool>>;

                       // Emplacement (copy constructor)
                       { cont.emplace(value) } -> std::same_as<std::pair<typename C::iterator, bool>>;
                   };

          // Move-enabled operations
          requires not std::movable<typename C::value_type> or
                   requires(typename C::value_type&& tmp_value)
                   {
                       // Move insertion
                       { cont.insert(std::move(tmp_value)) } -> std::same_as<std::pair<typename C::iterator, bool>>;

                       // Emplacement (move constructor)
                       { cont.emplace(std::move(tmp_value)) } -> std::same_as<std::pair<typename C::iterator, bool>>;
                   };
      };

    /// An associative container with multiple key occurrence.
    /// Satisfied by multiset, multimap, unordered_multiset and unordered_multimap.
    ///
    /// Note: the container is required to support .emplace(Args...) for every constructor
    /// of the form value_type(Args...).
    template <typename C>
    concept multiple_associative_container
    = associative_container<C> and
      requires(C& cont, C const& const_cont)
      {
          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires(typename C::const_iterator const& hint)
                   {{ cont.emplace() } -> std::same_as<typename C::iterator>; };

          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(typename C::value_type const& value)
                   {
                       // Copy insertion
                       { cont.insert(value) } -> std::same_as<typename C::iterator>;

                       // Emplacement (copy constructor)
                       { cont.emplace(value) } -> std::same_as<typename C::iterator>;
                   };

          // Move-enabled operations
          requires not std::movable<typename C::value_type> or
                   requires(typename C::value_type&& tmp_value)
                   {
                       // Move insertion
                       { cont.insert(std::move(tmp_value)) } -> std::same_as<typename C::iterator>;

                       // Emplacement (move constructor)
                       { cont.emplace(std::move(tmp_value)) } -> std::same_as<typename C::iterator>;
                   };
      };

    /// An associative container ordered on keys.
    /// Satisfied by set, map, multiset and multimap.
    ///
    /// Note: corresponds to the standard AssociativeContainer named requirement.
    template <typename C>
    concept ordered_associative_container
    = associative_container<C> and
      requires(C& cont, C const& const_cont)
      {
          // Comparators
          typename C::key_compare;
          typename C::value_compare;
          requires std::strict_weak_order<
              typename C::key_compare, typename C::key_type, typename C::key_type>;
          requires std::strict_weak_order<
              typename C::value_compare, typename C::value_type, typename C::value_type>;

          // Ordered associative containers of ordered values should provide a lexicographical ordering
          requires not std::totally_ordered<typename C::value_type> or
                   std::totally_ordered<C>;

          // Observers
          { const_cont.key_comp() } -> std::same_as<typename C::key_compare>;
          { const_cont.value_comp() } -> std::same_as<typename C::value_compare>;

          // Construction with comparator
          requires
          requires(typename C::key_compare const& key_comp)
          {
              C(key_comp);

              requires not std::copyable<typename C::value_type> or
                       requires(
                           mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& first,
                           mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& last,
                           std::initializer_list<typename C::value_type> const& init_list)
                       {
                           C(first, last, key_comp);
                           C(init_list, key_comp);
                       };
          };

          // Binary searching
          requires
          requires(typename C::key_type const& key)
          {
              { cont.lower_bound(key) } -> std::same_as<typename C::iterator>;
              { const_cont.lower_bound(key) } -> std::same_as<typename C::const_iterator>;
              { cont.upper_bound(key) } -> std::same_as<typename C::iterator>;
              { const_cont.upper_bound(key) } -> std::same_as<typename C::const_iterator>;
          };
      };

    /// An ordered associative container without multiple key occurrence.
    /// Satisfied by set and map.
    template <typename C>
    concept ordered_unique_associative_container
    = unique_associative_container<C> and ordered_associative_container<C>;

    /// An ordered associative container with multiple key occurrence.
    /// Satisfied by multiset and multimap.
    template <typename C>
    concept ordered_multiple_associative_container
    = multiple_associative_container<C> and ordered_associative_container<C>;

    /// An associative container based on key hashing (hash table).
    /// Satisfied by unordered_set, unordered_map, unordered_multiset and unordered_multimap.
    ///
    /// Note: corresponds to the standard UnorderedAssociativeContainer named requirement.
    template <typename C>
    concept unordered_associative_container
    = associative_container<C> and
      requires(C& cont, C const& const_cont)
      {
          // Key hash function
          typename C::hasher;
          requires hash_function<typename C::hasher, typename C::key_type>;

          // Key equality predicate
          typename C::key_equal;
          requires std::equivalence_relation<
              typename C::key_equal, typename C::key_type, typename C::key_type>;

          // Observers
          { const_cont.hash_function() } -> std::same_as<typename C::hasher>;
          { const_cont.key_eq() } -> std::same_as<typename C::key_equal>;

          // Hash policy
          requires
          requires(float const& ml, std::size_t n) {
              { const_cont.load_factor() } -> std::same_as<float>;
              { const_cont.max_load_factor() } -> std::same_as<float>;
              cont.max_load_factor(ml);
              cont.rehash(n);
              cont.reserve(n);
          };

          // Minimal bucket interface
          { const_cont.bucket_count() } -> std::same_as<std::size_t>;
          // Not requiring local_iterator etc as its not provided by many popular
          // non-std hash tables, e.g. absl::flat_hash_set and tsl::robin_map.

          // Construction with bucket count, hash function, and equality predicate
          requires
          requires(
              std::size_t const& bucket_count,
              typename C::hasher const& hash,
              typename C::key_equal const& equal)
          {
              C(bucket_count);
              C(bucket_count, hash);
              C(bucket_count, hash, equal);

              requires not std::copyable<typename C::value_type> or
                       requires(
                           mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& first,
                           mock_const_iterator<typename C::value_type, std::input_iterator_tag> const& last,
                           std::initializer_list<typename C::value_type> const& init_list)
                       {
                           C(first, last, bucket_count);
                           C(first, last, bucket_count, hash);
                           C(first, last, bucket_count, hash, equal);
                           C(init_list, bucket_count);
                           C(init_list, bucket_count, hash);
                           C(init_list, bucket_count, hash, equal);
                       };
          };
      };

    /// A hash-based associative container without multiple key occurrence.
    /// Satisfied by unordered_set and unordered_map.
    template <typename C>
    concept unordered_unique_associative_container
    = unique_associative_container<C> and unordered_associative_container<C>;

    /// A hash-based associative container with multiple key occurrence.
    /// Satisfied by unordered_multiset and unordered_multimap.
    template <typename C>
    concept unordered_multiple_associative_container
    = multiple_associative_container<C> and unordered_associative_container<C>;

    /// An associative container that stores a mapped object alongside its keys.
    /// Satisfied by map, multimap, unordered_map and unordered_multimap.
    template <typename C>
    concept map_container
    = associative_container<C> and
      requires(C& cont, C const& const_cont)
      {
          typename C::mapped_type;

          requires
          requires(typename C::const_iterator const& hint)
          {
              requires not std::copyable<typename C::key_type> or
                       requires(typename C::key_type const& key)
                       {
                           // Emplacement (key copy, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        cont.emplace_hint(hint, key)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple())
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        cont.emplace_hint(hint, key, obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(obj))
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        cont.emplace_hint(hint, key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(std::move(obj)))
                                        }  -> std::same_as<typename C::iterator>;
                                    };
                       };

              requires not std::movable<typename C::key_type> or
                       requires(typename C::key_type&& key)
                       {
                           // Emplacement (key move, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        cont.emplace_hint(hint, std::move(key))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple())
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key move, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        cont.emplace_hint(hint, std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(obj))
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key move, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        cont.emplace_hint(hint, std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(std::move(obj)))
                                        }  -> std::same_as<typename C::iterator>;
                                    };
                       };
          };
      };

    /// A map container without multiple key occurrence.
    /// Satisfied by map and unordered_map.
    ///
    /// Note: the container is required to support
    /// .try_emplace(key_type const&, Args...)
    /// .try_emplace(key_type&&, Args...)
    /// .try_emplace(const_iterator, key_type const&, Args...)
    /// .try_emplace(const_iterator, key_type&&, Args...)
    /// for every constructor of the form mapped_type(Args...), and
    /// .insert_or_assign(key_type const&, M&&)
    /// .insert_or_assign(key_type&&, M&&)
    /// .insert_or_assign(const_iterator, key_type const&, M&&)
    /// .insert_or_assign(const_iterator, key_type&&, M&&)
    /// for every type M where std::is_assignable_v<mapped_type&, M&&> is true.
    template <typename C>
    concept unique_map_container
    = map_container<C> and
      requires(C& cont, C const& const_cont)
      {
          // Element access
          requires
          requires(typename C::key_type const& key, typename C::key_type const& tmp_key)
          {
              // operator[] access
              requires not std::default_initializable<typename C::mapped_type> or
                       requires
                       {
                           // Key copy
                           requires not std::copyable<typename C::key_type> or
                                    requires {{ cont[key] } -> std::same_as<typename C::mapped_type&>; };

                           // Key move
                           requires not std::movable<typename C::key_type> or
                                    requires
                                    {{ cont[std::move(tmp_key)] } -> std::same_as<typename C::mapped_type&>; };
                       };

              // Checked access
              { cont.at(key) } -> std::same_as<typename C::mapped_type&>;
              { const_cont.at(key) } -> std::same_as<typename C::mapped_type const&>;
          };

          requires
          requires(typename C::const_iterator const& hint)
          {
              // Key copy
              requires not std::copyable<typename C::key_type> or
                       requires(typename C::key_type const& key)
                       {
                           // Emplacement (key copy, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        cont.emplace(key)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple())
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(key)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(hint, key)
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object copy
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        // Insertion
                                        {
                                        cont.insert_or_assign(key, obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.insert_or_assign(hint, key, obj)
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key copy, mapped object copy constructor)
                                        {
                                        cont.emplace(key, obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(key, obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(hint, key, obj)
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object move
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        // Insertion
                                        {
                                        cont.insert_or_assign(key, std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.insert_or_assign(hint, key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key copy, mapped object move constructor)
                                        {
                                        cont.emplace(key, std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(key, std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(hint, key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };

              // Key move
              requires not std::movable<typename C::key_type> or
                       requires(typename C::key_type&& key)
                       {
                           // Emplacement (key move, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        cont.emplace(std::move(key))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple())
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(std::move(key))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(hint, std::move(key))
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object copy
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        // Insertion
                                        {
                                        cont.insert_or_assign(std::move(key), obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.insert_or_assign(hint, std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key move, mapped object copy constructor)
                                        {
                                        cont.emplace(std::move(key), obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(std::move(key), obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(hint, std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object move
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        // Insertion
                                        {
                                        cont.insert_or_assign(std::move(key), std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.insert_or_assign(hint, std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key move, mapped object move constructor)
                                        {
                                        cont.emplace(std::move(key), std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(std::move(key), std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        cont.try_emplace(hint, std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };
          };
      };

    /// A map container with multiple key occurrence.
    /// Satisfied by multimap and unordered_multimap.
    template <typename C>
    concept multiple_map_container
    = map_container<C> and
      requires(C& cont, C const& const_cont)
      {
          requires
          requires(typename C::const_iterator const& hint)
          {
              requires not std::copyable<typename C::key_type> or
                       requires(typename C::key_type const& key)
                       {
                           // Emplacement (key copy, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        cont.emplace(key)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple())
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        cont.emplace(key, obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        cont.emplace(key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };

              requires not std::movable<typename C::key_type> or
                       requires(typename C::key_type&& key)
                       {
                           // Emplacement (key move, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        cont.emplace(std::move(key))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple())
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        cont.emplace(std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        cont.emplace(std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        cont.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };
          };
      };

    /// An ordered map container.
    /// Satisfied by map and multimap.
    template <typename C>
    concept ordered_map_container = map_container<C> and ordered_associative_container<C>;

    /// An ordered map container without multiple key occurrence.
    /// Satisfied by map.
    template <typename C>
    concept ordered_unique_map_container
    = unique_map_container<C> and ordered_map_container<C>;

    /// An ordered map container with multiple key occurrence.
    /// Satisfied by multimap.
    template <typename C>
    concept ordered_multiple_map_container
    = multiple_map_container<C> and ordered_map_container<C>;

    /// A hash-based map container.
    /// Satisfied by unordered_map and unordered_multimap.
    template <typename C>
    concept unordered_map_container = map_container<C> and unordered_associative_container<C>;

    /// A hash-based map container without multiple key occurrence.
    /// Satisfied by unordered_map.
    template <typename C>
    concept unordered_unique_map_container
    = unique_map_container<C> and unordered_map_container<C>;

    /// A hash-based map container with multiple key occurrence.
    /// Satisfied by unordered_multimap.
    template <typename C>
    concept unordered_multiple_map_container
    = multiple_map_container<C> and unordered_map_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept associative_container_of
    = container_of<C, ValueType> and
      associative_container<C> and
      std::same_as<KeyType, typename C::key_type>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unique_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and unique_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept multiple_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and multiple_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept ordered_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and ordered_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept ordered_unique_associative_container_of
    = unique_associative_container_of<C, ValueType, KeyType> and ordered_unique_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept ordered_multiple_associative_container_of
    = multiple_associative_container_of<C, ValueType, KeyType> and ordered_multiple_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unordered_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and unordered_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unordered_unique_associative_container_of
    = unique_associative_container_of<C, ValueType, KeyType> and unordered_unique_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unordered_multiple_associative_container_of
    = multiple_associative_container_of<C, ValueType, KeyType> and unordered_multiple_associative_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept map_container_of
    = map_container<C> and
      std::same_as<KeyType, typename C::key_type> and
      std::same_as<MappedType, typename C::mapped_type>;

    template <typename C, typename KeyType, typename MappedType>
    concept unique_map_container_of
    = map_container_of<C, KeyType, MappedType> and unique_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept multiple_map_container_of
    = map_container_of<C, KeyType, MappedType> and multiple_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept ordered_map_container_of
    = map_container_of<C, KeyType, MappedType> and
      ordered_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept ordered_unique_map_container_of
    = unique_map_container_of<C, KeyType, MappedType> and ordered_unique_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept ordered_multiple_map_container_of
    = multiple_map_container_of<C, KeyType, MappedType> and ordered_multiple_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept unordered_map_container_of
    = map_container_of<C, KeyType, MappedType> and
      unordered_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept unordered_unique_map_container_of
    = unique_map_container_of<C, KeyType, MappedType> and unordered_unique_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept unordered_multiple_map_container_of
    = multiple_map_container_of<C, KeyType, MappedType> and unordered_multiple_map_container<C>;
}
