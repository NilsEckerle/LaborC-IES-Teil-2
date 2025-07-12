cmake_minimum_required(VERSION 3.10)

# Set project name
project(atmega328p_firmware C)

option(TESTING "Enables/Disables compiling for testing" OFF)

if (TESTING)
	set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
	set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
	set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
	set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/testing-bin")

	set(MCU atmega328p)
	set(F_CPU 16000000UL)

	set(CMAKE_C_COMPILER gcc)

	set(CMAKE_C_FLAGS "-DLOGGER_USE_PRINTF -g -Wall -Wextra -std=gnu99")

	enable_testing()

	add_subdirectory(tests)

	# # Create test executables for specific files
	# # Example: Test for dynamic_array.c
	# add_executable(test_dynamic_array
	# 	${SRC_DIR}/tools/dynamic_array.c
	# 	${TEST_DIR}/dyn_arr/test_dyn_arr.c
	# )
	# target_include_directories(test_dynamic_array PRIVATE ${INCLUDE_DIR})
	# add_test(NAME test_dyn_arr COMMAND test_dynamic_array)
	#
	# # Example: Test for dynamic_array.c
	# add_executable(test_dynamic_array_add
	# 	${SRC_DIR}/tools/dynamic_array.c
	# 	${TEST_DIR}/dyn_arr/test_add.c
	# )
	# target_include_directories(test_dynamic_array_add PRIVATE ${INCLUDE_DIR})
	# add_test(NAME test_dyn_arr_add COMMAND test_dynamic_array_add)

else()
	# Directory paths (change these to customize your project structure)
	set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
	set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
	set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

	# Set the target microcontroller
	set(MCU atmega328p)
	set(F_CPU 16000000UL)

	# Set compiler and tools
	set(CMAKE_SYSTEM_NAME Generic)
	set(CMAKE_C_COMPILER avr-gcc)
	set(CMAKE_OBJCOPY avr-objcopy)
	set(CMAKE_SIZE avr-size)

	# Compiler flags
	set(CMAKE_C_FLAGS "-mmcu=${MCU} -D F_CPU=${F_CPU} -Os -Wall -Wextra -std=gnu99")

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

	# Add this after your target definition
	add_custom_command(TARGET firmware.elf POST_BUILD
		COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware.elf>
		COMMENT "Memory usage:")

	# Create bin directory
	file(MAKE_DIRECTORY ${BIN_DIR})
endif()
