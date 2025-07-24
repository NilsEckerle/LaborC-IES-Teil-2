MCU = atmega328p
F_CPU = 16000000UL

ARDUINO_DEVICE_FILE = /dev/ttyACM0
SIMULIDE_DEVICE_FILE = /dev/tnt2
INC = include
SRC = src
BUILD_DIR = build
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

help:
	@echo "make clean - to remove all build files"
	@echo "make build - to build"
	@echo "make flash_simulide - to build and flash simulide at $(SIMULIDE_DEVICE_FILE)"
	@echo "make flash_arduino - to build and flash arduino at $(ARDUINO_DEVICE_FILE)"

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
	avrdude -c arduino -p atmega328p -P $(SIMULIDE_DEVICE_FILE) -U flash:w:bin/firmware.hex

flash_arduino: build
	avrdude -c arduino -p atmega328p -P $(ARDUINO_DEVICE_FILE) -b 115200 flash:w:bin/firmware.hex

.PHONY: help build clean flash_simulide flash_arduino
