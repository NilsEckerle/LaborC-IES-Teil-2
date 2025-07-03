# LaborC-IES-Teil-2

## Requirements

on debian install
```bash
sudo apt install cmake gcc-avr binutils-avr avr-libc
```

## Build

To build this project do the following steps:
```bash
mkdir build
cd build
cmake ..
make
```

For ease of use i have added a make file in the root, which performs this by it self.
Just do `make` in the project root.
