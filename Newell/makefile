# avr-gcc for microcontroller
CC = avr-gcc

# debuginformations
# CFLAGS = -g -Wall
CFLAGS = -Os -mmcu=atmega328p
LDFLAGS =
SRC_DIRECTORY = src
BIN_DIRECTORY = bin
BUILD_DIRECTORY = build

# List your source files here (without path or extension)
SOURCES = skeleton

# Automatically generate object file names
OBJECTS = $(addprefix $(BUILD_DIRECTORY)/, $(addsuffix .o, $(SOURCES)))

# Name of the final executable
TARGET = $(BIN_DIRECTORY)/program

all: setup_dir $(TARGET) file_for_avr

setup_dir:
	@mkdir -p $(BUILD_DIRECTORY)
	@mkdir -p $(BIN_DIRECTORY)

# Pattern rule to compile .c files to .o files
$(BUILD_DIRECTORY)/%.o: $(SRC_DIRECTORY)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files into the final executable
# $(TARGET): $(OBJECTS)
# 	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Link all object files into the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	@rm -f $(BUILD_DIRECTORY)/*
	@rm -f $(BIN_DIRECTORY)/*

file_for_avr:
	avr-objcopy -O ihex $(TARGET) flashable.hex

.PHONY: all setup_dir clean
