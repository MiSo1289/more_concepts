#include <iterator>

#include "more_concepts/mock_iterator.hpp"

namespace
{
    using test_value_type = int;
    
    using mock_input_iterator = more_concepts::mock_const_iterator<
        test_value_type, std::input_iterator_tag>;
    using mock_forward_iterator = more_concepts::mock_const_iterator<
        test_value_type, std::forward_iterator_tag>;
    using mock_bidirectional_iterator = more_concepts::mock_const_iterator<
        test_value_type, std::bidirectional_iterator_tag>;
    using mock_random_access_iterator = more_concepts::mock_const_iterator<
        test_value_type, std::random_access_iterator_tag>;
    using mock_contiguous_iterator = more_concepts::mock_const_iterator<
        test_value_type, std::contiguous_iterator_tag>;
    
    using mock_output_iterator = more_concepts::mock_mutable_iterator<
        test_value_type, std::output_iterator_tag>;
    using mock_writable_input_iterator = more_concepts::mock_mutable_iterator<
        test_value_type, std::input_iterator_tag>;
    using mock_writable_forward_iterator = more_concepts::mock_mutable_iterator<
        test_value_type, std::forward_iterator_tag>;
    using mock_writable_bidirectional_iterator = more_concepts::mock_mutable_iterator<
        test_value_type, std::bidirectional_iterator_tag>;
    using mock_writable_random_access_iterator = more_concepts::mock_mutable_iterator<
        test_value_type, std::random_access_iterator_tag>;
    using mock_writable_contiguous_iterator = more_concepts::mock_mutable_iterator<
        test_value_type, std::contiguous_iterator_tag>;
}

static_assert(std::input_iterator<mock_input_iterator>);
static_assert(std::forward_iterator<mock_forward_iterator>);
static_assert(std::bidirectional_iterator<mock_bidirectional_iterator>);
static_assert(std::random_access_iterator<mock_random_access_iterator>);
static_assert(std::contiguous_iterator<mock_contiguous_iterator>);
