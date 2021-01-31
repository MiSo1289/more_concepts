# More concepts
```c++ 
#include <more_concepts/more_concepts.hpp>
```
This library aims to provide general purpose concepts that are not available in the C++20 concepts library, most notably container concepts. It also provides utilities for writing your own concepts (a `mock_iterator` that can mock any iterator category - see bellow). 

## Container concepts

The container concepts are intended to serve as an abstraction for the STL container interfaces, allowing writing constrained generic code that can use any container of some category, both standard and third-party (as long as it provides an STL compliant interface).

While a lot of generic algorithms can do with range and iterator concepts, this is sometimes not enough - for example, how does one write a constrained container adaptor? Let's say you want to implement a priority queue that can use any index-able sequence type with back-insertion to store the heap. Using this library, you could express this as:
```c++
template <typename T, 
          more_concepts::random_access_container_of<T> Seq = std::vector<T>>
requires more_concepts::back_growable_container<Seq>
class priority_queue;
```

### General container concepts
```c++ 
#include <more_concepts/base_containers.hpp>
```
- `container<C>` - Satisfied by all well-behaved (not `vector<bool>`) standard containers.
- `mutable_container<C>` - A container that allows mutable iteration. Satisfied by all standard containers except `set`. 
- `sized_container<C>` - A container that knows its size. Satisfied by all standard containers except `forward_list`.
- `clearable_container<C>` - A container that can be cleared . Satisfied by all standard containers except `array`.
- `reversible_container<C>` - A container that allows reverse iteration. Satisfied by all standard containers except `forward_list`.

For each of the above, an `_of<C, ValueType>` version is provided (e.g. `reversible_container_of`), that also requires the `value_type` to be the same as specified.

### Sequence container concepts
```c++ 
#include <more_concepts/sequence_containers.hpp>
```
- `sequence_container<C>` - A mutable container that represents linear ordering of elements (corresponds to the standard `SequenceContainer` named requirement). Provides efficient access to the beginning of the sequence. Satisfied by all standard sequence containers, namely `vector`, `array`, `basic_string`, `deque`, `list`, and `forward_list`.
- `double_ended_container<C>` - A sized and reversible sequence container that provides efficient access to the end of the sequence. Satisfied by all standard sequence containers except `forward_list`.
- `random_access_container<C>` - A double-ended sequence container that provides indexed access to elements. Satisfied by `vector`, `array`, `basic_string`, and `deque`.
- `contiguous_container<C>` - A random-access sequence container, stored contiguously in memory. Satisfied by `vector`, `array`, and `basic_string`.
- `resizable_sequence_container<C>` - A clearable double-ended container that allows resizing, range construction and assignment, and insertion / erasure in the middle). Satisfied by `vector`, `basic_string`, `deque`, and `list`.
- `inplace_constructing_sequence_container<C>` - Extends the `resizable_sequence_container` interface with in-place construction. Satisfied by all standard models of `resizable_sequence_container` except `basic_string`.
- `front_growable_container<C>` - A sequence container that allows efficient inserting / erasure at the front. Satisfied by `deque`, `list`, and `forward_list`.
- `inplace_front_constructing_container<C>` - Extends the `front_growable_container` interface with in-place construction. Satisfied by all standard models of `front_growable_container`.
- `back_growable_container<C>` - A double-ended container that allows efficient inserting / erasure at the back. Satisfied by `vector`, `basic_string`, `deque`, and `list`.
- `inplace_back_constructing_container<C>` - Extends the `back_growable_container` interface with in-place construction. Satisfied by all standard models of `back_growable_container` except `basic_string`.

For each sequence container concept, an `_of<C, ValueType>` version is also provided.

### Associative container concepts
```c++ 
#include <more_concepts/associative_containers.hpp>
```
- `associative_container<C>` - A container that provides fast lookup of objects based on keys. Represents a union of the standard named requirements `AssociativeContainer` and `UnorderedAssociativeContainer`. Satisfied by all standard associative containers.
- `unique_associative_container<C>` - An associative container with unique keys. Satisfied by `set`, `map`, `unordered_set` and `unordered_map`.
- `multiple_associative_container<C>` - An associative container with non-unique keys. Satisfied by `multiset`, `multimap`, `unordered_multiset` and `unordered_multimap`.
- `map_container<C>` - An associative container representing a key-value mapping. Satisfied by `map`, `multimap`, `unordered_map` and `unordered_multimap`.
- `unique_map_container<C>` - A map container with unique keys. Satisfied by `map` and `unordered_map`.
- `multiple_map_container<C>` - A map container with non-unique keys. Satisfied by `multimap` and `unordered_multimap`.

For each associative container / map concept, `ordered_` and `unordered_` versions are available (e.g. `ordered_unique_map_container`).

For each generic (non-map) associative container concept, an `_of<C, ValueType, KeyType=ValueType>` version is available.

For each map container concept, an `_of<C, KeyType, MappedType>` version is available.

### General concepts
```c++ 
#include <more_concepts/base_concepts.hpp>
```  
Concepts that are simple wrappers over standard type traits:
- `decayed<T>` - Types that are non-reference, non-c-array, non-function or function reference, non-const and non-volatile. Assigning an object of this type to an auto variable preserves the type. Used to constrain the `value_type` of containers.
- `aggregate<T>` - Types that support aggregate initialization.
- `trivial<T>` - Types that can be `memcpy`-ied, and don't need any (non-trivial) initialization or destruction.
- `enum_type<T>` - Scoped and unscoped enumeration types.
- `error_code_enum<T>` - Error enum that can be used to construct a `std::error_code`.
- `error_condition_enum<T>` - Error enum that can be used to construct a `std::error_condition`.

Function concepts:
- `invocable_as<Fn, Ret(Args...)...>` - Function types that can be called with `std::invoke` using one or more function signatures. The return type of each signature is only checked for convertibility.
- `callable_as<Fn, Ret(Args...)...>` - Function types that can be called with the function-call operator using one or more function signatures. The return type of each signature must be matched exactly.
- `hash_function` - corresponds to the `Hash` standard named requirement. Used to define the `unordered_associative_container` concept. 

### Mock iterator
```c++ 
#include <more_concepts/mock_iterator.hpp>
```
The `mock_iterator<T, IteratorCategory, RWCategory>` class template can be used to write concepts that require some operation to accept any iterator of some category. 

Template parameters:
- `T` - iterator value type.
- `IteratorCategory` - can be one of the standard iterator category tags (e.g. `std::input_iterator_tag`). The mock iterator provides the minimal needed interface to satisfy the requested category. E.g. for the input and output iterator categories, a proxy reference type is used instead of a raw reference.
- `RWCategory` - can be one of `mutable_iterator_tag`, `const_iterator_tag`. Indicates whether the mock iterator should support write access.