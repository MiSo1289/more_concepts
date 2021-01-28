cmake_minimum_required(VERSION 3.17)
set(MORE_CONCEPTS_VERSION 0.1.0)
project(MoreConcepts VERSION "${MORE_CONCEPTS_VERSION}")

add_subdirectory(tests)

add_library(more_concepts INTERFACE)
add_library(more_concepts::more_concepts ALIAS more_concepts)
target_compile_features(more_concepts INTERFACE cxx_std_20)
target_include_directories(more_concepts INTERFACE include)
