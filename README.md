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
Just do `make` in the project root. For easy testing you can run `make test` or
`make test-verbose`. This is running unit test on components that doesnt involve
the hardware (avr). It is then compiled with gcc rather than gcc-avr to be
runnable on your machine.

## Code Documentation
Documentation in this project is initially generated using AI (Claude Sonnet)
and then thoroughly reviewed, revised, and approved by human developers before
being committed to the repository. All AI-generated code and documentation
undergoes mandatory human review to ensure accuracy, completeness, and adherence
to project standards.

**Important:** No AI-generated code or documentation is pushed to the repository
without prior human review and approval.
