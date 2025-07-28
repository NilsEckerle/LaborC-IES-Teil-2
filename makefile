MCU = atmega328p
F_CPU = 16000000UL

ARDUINO_DEVICE_FILE = /dev/ttyACM0
SIMULIDE_DEVICE_FILE = /dev/tnt2
INC = include
SRC = src
BUILD_DIR = build
DOCS_DIR = docs
DOCS_CONFIG_DIR = res
BIN_DIR = bin
TARGET = firmware

SOURCES = $(shell find $(SRC) -name '*.c')
OBJECTS = $(patsubst $(SRC)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -D F_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu99
LDFLAGS = -mmcu=$(MCU)

ELF = $(BIN_DIR)/$(TARGET).elf
HEX = $(BIN_DIR)/$(TARGET).hex

default: flash_arduino

help:
	@echo "Available targets:"
	@echo "  help               - show this help"
	@echo "  build              - Build the firmware"
	@echo "  documentation      - Build the documentation"
	@echo "  clean              - Remove all build files"
	@echo "  clean_documentation - Remove documentation files"
	@echo "  clean_all          - Remove build and documentation files"
	@echo "  flash_simulide     - Build and flash to SimulIDE at $(SIMULIDE_DEVICE_FILE)"
	@echo "  flash_arduino      - Build and flash to Arduino at $(ARDUINO_DEVICE_FILE)"

$(BUILD_DIR)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

objects: $(OBJECTS)

# Link all object files into ELF
$(ELF): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Convert ELF to HEX
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

build: objects $(ELF) $(HEX)

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

flash_simulide: build
	avrdude -D -c arduino -p atmega328p -P $(SIMULIDE_DEVICE_FILE) -U flash:w:bin/firmware.hex

flash_arduino: build
	avrdude -c arduino -p atmega328p -P $(ARDUINO_DEVICE_FILE) -b 115200 -U flash:w:bin/firmware.hex

clean_documentation:
	@rm -rf $(DOCS_DIR)

documentation:
	@mkdir -p $(DOCS_DIR)
	cd res && doxygen Doxyfile
	
clean_all: clean clean_documentation

.PHONY: help default build flash_arduino flash_simulide documentation clean clean_documentation clean_all
