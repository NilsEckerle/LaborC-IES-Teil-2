build:
	@mkdir -p build
	@cd build && cmake .. && make

clean:
	@rm -rf build

flash_simulide: clean build
	avrdude -c arduino -p atmega328p -P /dev/tnt2 -b 115200 -D -U flash:w:bin/firmware.hex

.PHONY: build clean flash_simulide
