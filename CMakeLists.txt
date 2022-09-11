cmake_minimum_required(VERSION 3.17)
set(MORE_CONCEPTS_VERSION 0.1.1)
project(MoreConcepts VERSION "${MORE_CONCEPTS_VERSION}")

option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()

add_library(more_concepts INTERFACE)
add_library(more_concepts::more_concepts ALIAS more_concepts)
target_compile_features(more_concepts INTERFACE cxx_std_20)
set_property(TARGET more_concepts PROPERTY VERSION ${MORE_CONCEPTS_VERSION})
target_include_directories(more_concepts INTERFACE
	$<BUILD_INTERFACE:include>
	$<INSTALL_INTERFACE:include>
)

install(TARGETS more_concepts
	EXPORT MoreConceptsTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION .)
install(EXPORT MoreConceptsTargets
	FILE MoreConceptsTargets.cmake
	NAMESPACE more_concepts::
	DESTINATION lib/cmake
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/MoreConceptsConfig.cmake"
	INSTALL_DESTINATION lib/cmake
)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/MoreConceptsConfigVersion.cmake"
	VERSION ${MORE_CONCEPTS_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/MoreConceptsConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/MoreConceptsConfigVersion.cmake"
	DESTINATION lib/cmake
)
