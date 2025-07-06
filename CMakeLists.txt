cmake_minimum_required(VERSION 3.10)

# Set project name
project(atmega328p_firmware C)

# Directory paths
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(UNITY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/Unity")

# Set the target microcontroller
set(MCU atmega328p)
set(F_CPU 16000000UL)

# Option to build for testing (defaults to Arduino build)
option(BUILD_TESTS "Build for unit testing on host PC" OFF)

if(BUILD_TESTS)
	message(STATUS "Building for unit tests on host PC")

	# Use native compiler for tests
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_C_FLAGS "-Wall -Wextra -std=gnu99 -g -DUNIT_TEST -DUNITY_INCLUDE_CONFIG_H")

	# Enable testing
	enable_testing()

	# Include directories
	include_directories(${INCLUDE_DIR})
	include_directories(${TEST_DIR})
	include_directories(${UNITY_DIR}/src)

	# Unity source files
	set(UNITY_SOURCES
		${UNITY_DIR}/src/unity.c
	)

	# Create Unity library
	add_library(unity ${UNITY_SOURCES})
	target_include_directories(unity PUBLIC ${UNITY_DIR}/src)

	# Find all source files except main.c (assuming main.c has main() function)
	file(GLOB_RECURSE LIB_SOURCES "${SRC_DIR}/*.c")
	list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.c$")

	# Create a library from your application code (excluding main)
	if(LIB_SOURCES)
		add_library(firmware_lib ${LIB_SOURCES})
		target_include_directories(firmware_lib PUBLIC ${INCLUDE_DIR})
		target_compile_definitions(firmware_lib PRIVATE UNIT_TEST)
	endif()

	# Find all test files
	file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/test_*.c")

	# Create test executables
	foreach(test_file ${TEST_SOURCES})
		get_filename_component(test_name ${test_file} NAME_WE)
		add_executable(${test_name} ${test_file})

		# Link with Unity and firmware library
		target_link_libraries(${test_name} unity)
		if(LIB_SOURCES)
			target_link_libraries(${test_name} firmware_lib)
		endif()

		target_include_directories(${test_name} PRIVATE ${INCLUDE_DIR} ${TEST_DIR})
		target_compile_definitions(${test_name} PRIVATE UNIT_TEST)

		# Add as a test
		add_test(NAME ${test_name} COMMAND ${test_name})
	endforeach()

	# Create a test runner that runs all tests
	add_custom_target(run_all_tests
		COMMAND ${CMAKE_CTEST_COMMAND} --verbose
		DEPENDS ${TEST_SOURCES}
		COMMENT "Running all tests"
	)

else()
	message(STATUS "Building for Arduino (${MCU})")

	# Arduino build configuration
	set(CMAKE_SYSTEM_NAME Generic)
	set(CMAKE_C_COMPILER avr-gcc)
	set(CMAKE_OBJCOPY avr-objcopy)
	set(CMAKE_SIZE avr-size)

	# Compiler flags for Arduino
	set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -Wall -Wextra -std=gnu99")

	# Include directories (recursively include all subdirectories)
	include_directories(${INCLUDE_DIR})
	file(GLOB_RECURSE INCLUDE_DIRS "${INCLUDE_DIR}/*")
	foreach(dir ${INCLUDE_DIRS})
		if(IS_DIRECTORY ${dir})
			include_directories(${dir})
		endif()
	endforeach()

	# Check if source directory exists
	if(NOT EXISTS "${SRC_DIR}")
		message(FATAL_ERROR "Source directory does not exist: ${SRC_DIR}")
	endif()

	# Source files (recursively find all .c files in src and subdirectories)
	file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.c")

	# Debug: Print found sources
	message(STATUS "Looking for sources in: ${SRC_DIR}")
	message(STATUS "Found sources: ${SOURCES}")

	# Check if we found any sources
	if(NOT SOURCES)
		message(FATAL_ERROR "No .c source files found in ${SRC_DIR}. Make sure you have .c files in your src directory.")
	endif()

	# Create executable
	add_executable(firmware.elf ${SOURCES})

	# Custom commands to create hex and bin files
	add_custom_command(TARGET firmware.elf POST_BUILD
		COMMAND ${CMAKE_OBJCOPY} -O ihex firmware.elf ${BIN_DIR}/firmware.hex
		COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf ${BIN_DIR}/firmware.bin
		COMMAND ${CMAKE_SIZE} firmware.elf
		COMMENT "Creating hex and binary files"
	)

	# Memory usage report
	add_custom_command(TARGET firmware.elf POST_BUILD
		COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware.elf>
		COMMENT "Memory usage:")

	# Create bin directory
	file(MAKE_DIRECTORY ${BIN_DIR})
endif()
