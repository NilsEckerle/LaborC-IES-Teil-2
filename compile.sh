#!/bin/bash

echo "COMPILING $2 $3 $4 $5"
avr-gcc $2 $3 $4 $5 -o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6

echo "GENERATING FIRMWARE"
avr-objcopy --set-start=0 -O ihex tmpfile tmpfile.hex
rm tmpfile

if [ ! "$1" = "simulate" ]; then
  echo "FLASHING TO $1"
  avrdude -p atmega328p -c arduino -P $1 -b 115200 -U flash:w:tmpfile.hex:i
fi

x="Firmware_$(date +"%d.%m.%Y_%H:%M:%S").hex"
mv tmpfile.hex $x
echo "FIRMWARE FOR SIMULATION: $x"
