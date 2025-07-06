# Project Makefile - supports both Arduino build and unit testing

# Directories
BUILD_DIR = build
TEST_BUILD_DIR = build_test
BIN_DIR = bin

# Default target
all: build

# Build for Arduino (AVR)
build:
	@echo "Building for Arduino (ATmega328P)..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. -DBUILD_TESTS=OFF && make
	@echo "Arduino build complete. Files are in $(BIN_DIR)/ directory."

# Build and run unit tests
test:
	@echo "Building and running unit tests..."
	@mkdir -p $(TEST_BUILD_DIR)
	@cd $(TEST_BUILD_DIR) && cmake .. -DBUILD_TESTS=ON && make
	@echo ""
	@echo "Running tests..."
	@cd $(TEST_BUILD_DIR) && ctest --verbose
	@echo "Tests complete."

# Run tests without rebuilding
run-tests:
	@if [ -d "$(TEST_BUILD_DIR)" ]; then \
		echo "Running tests..."; \
		cd $(TEST_BUILD_DIR) && ctest --verbose; \
	else \
		echo "Test build directory not found. Run 'make test' first."; \
	fi

# Clean all build directories
clean:
	@echo "Cleaning build directories..."
	@rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)
	@echo "Clean complete."

# Clean only Arduino build
clean-build:
	@echo "Cleaning Arduino build directory..."
	@rm -rf $(BUILD_DIR)

# Clean only test build
clean-test:
	@echo "Cleaning test build directory..."
	@rm -rf $(TEST_BUILD_DIR)

# Flash to SimulIDE (your existing target)
flash_simulide: clean build
	@echo "Flashing to SimulIDE..."
	avrdude -c arduino -p atmega328p -P /dev/tnt2 -b 115200 -D -U flash:w:$(BIN_DIR)/firmware.hex

# Flash to real Arduino (common alternative)
flash_arduino: clean build
	@echo "Flashing to Arduino..."
	avrdude -c arduino -p atmega328p -P /dev/ttyUSB0 -b 115200 -D -U flash:w:$(BIN_DIR)/firmware.hex

# Build both Arduino and tests
build-all: clean
	@echo "Building both Arduino firmware and tests..."
	@$(MAKE) build
	@$(MAKE) test

# Development workflow: test then build
dev: test build

# Help target
help:
	@echo "Available targets:"
	@echo "  build         - Build for Arduino (default)"
	@echo "  test          - Build and run unit tests"
	@echo "  run-tests     - Run tests without rebuilding"
	@echo "  clean         - Clean all build directories"
	@echo "  clean-build   - Clean only Arduino build"
	@echo "  clean-test    - Clean only test build"
	@echo "  flash_simulide- Flash to SimulIDE"
	@echo "  flash_arduino - Flash to real Arduino"
	@echo "  build-all     - Build both Arduino and tests"
	@echo "  dev           - Test then build (development workflow)"
	@echo "  help          - Show this help"

# Declare phony targets
.PHONY: all build test run-tests clean clean-build clean-test flash_simulide flash_arduino build-all dev help
