cmake_minimum_required(VERSION 3.10)

# Get all test files in current directory
file(GLOB TEST_FILES "test_*.c")

# Source file being tested
set(SOURCE_FILE "${SRC_DIR}/tools/dynamic_array.c")

# Create test executables for each test file
foreach(TEST_FILE ${TEST_FILES})
    # Get filename without extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    
    # Create executable
    add_executable(${TEST_NAME}
        ${SOURCE_FILE}
        ${TEST_FILE}
    )
    
    # Set include directories
    target_include_directories(${TEST_NAME} PRIVATE ${INCLUDE_DIR})
    
    # Add to test suite
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    # Optional: Set test properties
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "All.*tests passed!"
    )
endforeach()

# Optional: Create a custom target to run all dynamic array tests
add_custom_target(test_dyn_arr_all
    COMMAND ${CMAKE_CTEST_COMMAND} -R "test_.*" --output-on-failure
    DEPENDS ${TEST_FILES}
    COMMENT "Running all dynamic array tests"
)
