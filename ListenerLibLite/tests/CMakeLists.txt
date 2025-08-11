# Collect source files for ListenerLibLite tests
file(GLOB TESTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create ListenerLibLite test executable target
add_executable(tests_ListenerLibLite
	${TESTS_SOURCES}
)

# Output test executable to bin folder in build directory
set_target_properties(tests_ListenerLibLite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Specify the precompiled header file for ListenerLibLite test executable
target_precompile_headers(tests_ListenerLibLite REUSE_FROM ListenerLibLite)

# Link ListenerLibLite static library and GTest static library to ListenerLibLite test executable
target_link_libraries(tests_ListenerLibLite
	PRIVATE
		ListenerLibLite
		gtest_main
)

# Create test with ListenerLibLite test executable and GTest
add_test(NAME tests_ListenerLibLite COMMAND tests_ListenerLibLite)