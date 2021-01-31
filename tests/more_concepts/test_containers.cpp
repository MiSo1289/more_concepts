#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "more_concepts/associative_containers.hpp"
#include "more_concepts/base_containers.hpp"
#include "more_concepts/sequence_containers.hpp"

namespace
{
    template <typename ValueType, typename... Ts>
    struct require_containers
    {
        static_assert((more_concepts::container<Ts> and ...));
        static_assert((more_concepts::container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_mutable_containers : require_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::mutable_container<Ts> and ...));
        static_assert((more_concepts::mutable_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_sized_containers : require_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::sized_container<Ts> and ...));
        static_assert((more_concepts::sized_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_clearable_containers : require_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::clearable_container<Ts> and ...));
        static_assert((more_concepts::clearable_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_reversible_containers : require_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::reversible_container<Ts> and ...));
        static_assert((more_concepts::reversible_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_mutable_sequence_containers : require_mutable_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::sequence_container<Ts> and ...));
        static_assert((more_concepts::sequence_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_double_ended_containers
        : require_mutable_sequence_containers<ValueType, Ts...>,
          require_sized_containers<ValueType, Ts...>,
          require_reversible_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::double_ended_container<Ts> and ...));
        static_assert((more_concepts::double_ended_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_contiguous_random_access_containers : require_double_ended_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::contiguous_container<Ts> and ...));
        static_assert((more_concepts::contiguous_container_of<Ts, ValueType> and ...));
        static_assert((more_concepts::random_access_container<Ts> and ...));
        static_assert((more_concepts::random_access_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_resizable_containers
        : require_double_ended_containers<ValueType, Ts...>,
          require_clearable_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::resizable_sequence_container<Ts> and ...));
        static_assert((more_concepts::resizable_sequence_container_of<Ts, ValueType> and ...));
    };


    template <typename ValueType, typename... Ts>
    struct require_inplace_constructing_resizable_containers : require_resizable_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::inplace_constructing_sequence_container<Ts> and ...));
        static_assert((more_concepts::inplace_constructing_sequence_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_front_growable_containers : require_mutable_sequence_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::front_growable_container<Ts> and ...));
        static_assert((more_concepts::front_growable_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_inplace_front_constructing_containers : require_front_growable_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::inplace_front_constructing_container<Ts> and ...));
        static_assert((more_concepts::inplace_front_constructing_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_back_growable_containers : require_double_ended_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::back_growable_container<Ts> and ...));
        static_assert((more_concepts::back_growable_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename... Ts>
    struct require_inplace_back_constructing_containers : require_back_growable_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::inplace_back_constructing_container<Ts> and ...));
        static_assert((more_concepts::inplace_back_constructing_container_of<Ts, ValueType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_associative_containers
        : require_sized_containers<ValueType, Ts...>,
          require_clearable_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::associative_container<Ts> and ...));
        static_assert((more_concepts::associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_unique_associative_containers : require_associative_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::unique_associative_container<Ts> and ...));
        static_assert((more_concepts::unique_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_multiple_associative_containers : require_associative_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::multiple_associative_container<Ts> and ...));
        static_assert((more_concepts::multiple_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_ordered_associative_containers : require_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::ordered_associative_container<Ts> and ...));
        static_assert((more_concepts::ordered_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_ordered_unique_associative_containers : require_ordered_associative_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::ordered_unique_associative_container<Ts> and ...));
        static_assert((more_concepts::ordered_unique_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_ordered_multiple_associative_containers : require_ordered_associative_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::ordered_multiple_associative_container<Ts> and ...));
        static_assert((more_concepts::ordered_multiple_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_unordered_associative_containers : require_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::unordered_associative_container<Ts> and ...));
        static_assert((more_concepts::unordered_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_unordered_unique_associative_containers : require_unordered_associative_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::unordered_unique_associative_container<Ts> and ...));
        static_assert((more_concepts::unordered_unique_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename... Ts>
    struct require_unordered_multiple_associative_containers
        : require_unordered_associative_containers<ValueType, Ts...>
    {
        static_assert((more_concepts::unordered_multiple_associative_container<Ts> and ...));
        static_assert((more_concepts::unordered_multiple_associative_container_of<Ts, ValueType, KeyType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_map_containers : require_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::map_container<Ts> and ...));
        static_assert((more_concepts::map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_unique_map_containers
        : require_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_unique_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::unique_map_container<Ts> and ...));
        static_assert((more_concepts::unique_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_multiple_map_containers
        : require_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_multiple_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::multiple_map_container<Ts> and ...));
        static_assert((more_concepts::multiple_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_ordered_map_containers
        : require_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_ordered_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::ordered_map_container<Ts> and ...));
        static_assert((more_concepts::ordered_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_ordered_unique_map_containers
        : require_ordered_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_unique_map_containers<ValueType, KeyType, MappedType, Ts...>
    {
        static_assert((more_concepts::ordered_unique_map_container<Ts> and ...));
        static_assert((more_concepts::ordered_unique_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_ordered_multiple_map_containers
        : require_ordered_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_multiple_map_containers<ValueType, KeyType, MappedType, Ts...>
    {
        static_assert((more_concepts::ordered_multiple_map_container<Ts> and ...));
        static_assert((more_concepts::ordered_multiple_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_unordered_map_containers
        : require_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_unordered_associative_containers<ValueType, KeyType, Ts...>
    {
        static_assert((more_concepts::unordered_map_container<Ts> and ...));
        static_assert((more_concepts::unordered_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_unordered_unique_map_containers
        : require_unordered_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_unique_map_containers<ValueType, KeyType, MappedType, Ts...>
    {
        static_assert((more_concepts::unordered_unique_map_container<Ts> and ...));
        static_assert((more_concepts::unordered_unique_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    template <typename ValueType, typename KeyType, typename MappedType, typename... Ts>
    struct require_unordered_multiple_map_containers
        : require_unordered_map_containers<ValueType, KeyType, MappedType, Ts...>,
          require_multiple_map_containers<ValueType, KeyType, MappedType, Ts...>
    {
        static_assert((more_concepts::unordered_multiple_map_container<Ts> and ...));
        static_assert((more_concepts::unordered_multiple_map_container_of<Ts, KeyType, MappedType> and ...));
    };

    // Sequence containers

    using test_value_type = int;
    constexpr auto test_arr_size = 10;

    using test_array = std::array<test_value_type, test_arr_size>;
    using test_vector = std::vector<test_value_type>;
    using test_string = std::basic_string<test_value_type>;
    using test_deque = std::deque<test_value_type>;
    using test_list = std::list<test_value_type>;
    using test_forward_list = std::forward_list<test_value_type>;

    constexpr auto contiguous_random_access = require_contiguous_random_access_containers<
        test_value_type,

        test_array,
        test_vector,
        test_string>{};

    constexpr auto clearable = require_clearable_containers<
        test_value_type,

        test_vector,
        test_string,
        test_deque,
        test_list,
        test_forward_list>{};

    constexpr auto double_ended = require_double_ended_containers<
        test_value_type,

        test_array,
        test_vector,
        test_string,
        test_deque,
        test_list>{};

    constexpr auto resizable = require_resizable_containers<
        test_value_type,

        test_vector,
        test_string,
        test_deque,
        test_list>{};

    constexpr auto inplace_constructing = require_inplace_constructing_resizable_containers<
        test_value_type,

        test_vector,
        test_deque,
        test_list>{};

    constexpr auto front_growable = require_front_growable_containers<
        test_value_type,

        test_deque,
        test_list,
        test_forward_list>{};

    constexpr auto inplace_front_constructing = require_inplace_front_constructing_containers<
        test_value_type,

        test_deque,
        test_list,
        test_forward_list>{};

    constexpr auto back_growable = require_back_growable_containers<
        test_value_type,

        test_vector,
        test_string,
        test_deque,
        test_list>{};

    constexpr auto inplace_back_constructing = require_inplace_back_constructing_containers<
        test_value_type,

        test_vector,
        test_deque,
        test_list>{};


    // Associative containers

    using test_key_type = std::string;
    using test_kv_type = std::pair<test_key_type const, test_value_type>;

    using test_set = std::set<test_key_type>;
    using test_multiset = std::multiset<test_key_type>;
    using test_unordered_set = std::unordered_set<test_key_type>;
    using test_unordered_multiset = std::unordered_multiset<test_key_type>;
    using test_map = std::map<test_key_type, test_value_type>;
    using test_multimap = std::multimap<test_key_type, test_value_type>;
    using test_unordered_map = std::unordered_map<test_key_type, test_value_type>;
    using test_unordered_multimap = std::unordered_multimap<test_key_type, test_value_type>;

    constexpr auto ordered_unique_sets = require_ordered_unique_associative_containers<
        test_key_type,
        test_key_type,

        test_set>{};
    
    constexpr auto ordered_multiple_sets = require_ordered_multiple_associative_containers<
        test_key_type,
        test_key_type,

        test_multiset>{};

    constexpr auto unordered_unique_sets = require_unordered_unique_associative_containers<
        test_key_type,
        test_key_type,

        test_unordered_set>{};

    constexpr auto unordered_multiple_sets = require_unordered_multiple_associative_containers<
        test_key_type,
        test_key_type,

        test_unordered_multiset>{};
    
    constexpr auto ordered_unique_maps = require_ordered_unique_map_containers<
        test_kv_type,
        test_key_type,
        test_value_type,

        test_map>{};

    constexpr auto ordered_multiple_maps = require_ordered_multiple_map_containers<
        test_kv_type,
        test_key_type,
        test_value_type,

        test_multimap>{};

    constexpr auto unordered_unique_maps = require_unordered_unique_map_containers<
        test_kv_type,
        test_key_type,
        test_value_type,

        test_unordered_map>{};

    constexpr auto unordered_multiple_maps = require_unordered_multiple_map_containers<
        test_kv_type,
        test_key_type,
        test_value_type,

        test_unordered_multimap>{};
}
