# avr-gcc for microcontroller
CC = avr-gcc
# debuginformations
# CFLAGS = -g -Wall
CFLAGS = -Os -mmcu=atmega328p -D F_CPU=16E6
LDFLAGS =
SRC_DIRECTORY = src
BIN_DIRECTORY = bin
BUILD_DIRECTORY = build
FIRMWARE_DIRECTORY = firmware
# List your source files here (without path or extension)
SOURCES = main iesusart logger shift_register
# Automatically generate object file names
OBJECTS = $(addprefix $(BUILD_DIRECTORY)/, $(addsuffix .o, $(SOURCES)))
# Name of the final executable
TARGET = $(BIN_DIRECTORY)/program

all: setup_dir compile_and_link file_for_avr

testat:
	./compile.sh simulate src/Testat_1_ECKERLE.c src/iesusart.c

setup_dir:
	@mkdir -p $(BUILD_DIRECTORY)
	@mkdir -p $(BIN_DIRECTORY)
	@mkdir -p $(FIRMWARE_DIRECTORY)

clean:
	@rm -f $(BUILD_DIRECTORY)/*
	@rm -f $(BIN_DIRECTORY)/*
	@rm -f $(FIRMWARE_DIRECTORY)/*

compile_and_link:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(addprefix $(SRC_DIRECTORY)/, $(addsuffix .c, $(SOURCES)))

file_for_avr:
	avr-objcopy --set-start=0 -O ihex $(TARGET) $(FIRMWARE_DIRECTORY)/"firmware_$$(date +'%Y-%m-%d_%H-%M-%S').hex"

flash_newest_hex_to_ttyACM0:
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U flash:w:$(shell ls -t $(FIRMWARE_DIRECTORY)/firmware_*.hex | head -1):i

.PHONY: all setup_dir clean flash_newest_hex_to_ttyACM0
