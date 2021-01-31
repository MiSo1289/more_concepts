add_executable(more_concepts_tests)
add_test(
  NAME more_concepts_tests
  COMMAND more_concepts_tests
)
target_include_directories(
  more_concepts_tests

  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(
  more_concepts_tests

  PRIVATE
  more_concepts::more_concepts
)

add_subdirectory(more_concepts)
