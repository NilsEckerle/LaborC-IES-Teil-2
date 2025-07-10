build:
	@mkdir -p build
	@cd build && cmake -D TESTING=OFF .. && make

test: build-test
	@cd test-build && ctest

test-verbose: build-test
	@cd test-build && ctest --verbose

build-test:
	@mkdir -p test-build
	@cd test-build && cmake -D TESTING=ON .. && make

clean:
	@rm -rf build
	@rm -rf test-build

flash_simulide: clean build
	avrdude -c arduino -p atmega328p -P /dev/tnt2 -b 115200 -D -U flash:w:bin/firmware.hex

flash_arduino: clean build
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:bin/firmware.hex

.PHONY: build build-test test clean flash_simulide
