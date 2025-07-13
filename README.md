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

## How to develop in this Repo

### Task management System
This Repo uses a combined approach
1. **TODO.md** - Central planning and tracking
2. **Git branches** - Implementation workflow

### The broad idea
- New tasks are added in the TODO.md
- when solving a task do it in a git branch
    - naming the branch for what it is e.g. *issue/sensor-calibration*
    - add the branch name in the TODO.md for later refference
- merge the branch into the main branch
- in TODO.md mark task as complete.

### Workflow
1. for each task you want to do this:
    ```sh
    # Create a branch from main
    git checkout project
    git pull origin project
    git checkout -b issue/descriptive-name

    # Work on your changes
    # ... make commits ...

    # Push new branch
    git push origin issue/descriptive-name
    ```
2. add branch name to TODO.md
3. To commit you would do this, notice that we merge with `--no-ff` to preserve
   the branch: 
   ```sh
    # After review, merge with --no-ff to create merge commit
    git checkout project
    git merge --no-ff issue/descriptive-name
    ```
4. Mark Task in TODO.md as done
    ```sh
    git push origin project
    git branch -d issue/descriptive-name
    ```

## Code Documentation
Documentation in this project is initially generated using AI (Claude Sonnet)
and then thoroughly reviewed, revised, and approved by human developers before
being committed to the repository. All AI-generated code and documentation
undergoes mandatory human review to ensure accuracy, completeness, and adherence
to project standards.

**Important:** No AI-generated code or documentation is pushed to the repository
without prior human review and approval.
