# LaborC-IES-Teil-2

![Roboter image](roboter.png)

## Features
**Base features**
- Drive 3 rounds then stop
- Constant USART Prints of round state and on some events
- Manual starting and help per USART command
- Doxygen documentation
- Building of project and documentation via make

**Additional features:**
- Hierarchical state machine. See `state.h` and `state_machine.h` in documentation.
    - Configurable at runtime via adding edges with conditions and optional
      execution when traversing the edge
    - Adding a parrent state to achive state hierachie to avoid edge duplication
- State machine diagram generation. See `scripts/extract-state-machine.sh` and
  `make generate_state_machine_diagram`
    - For a example see `Related Pages > State machine diagram` in the
      documentation
    - External source code analysis to extract states and edges, so the code
      doesn't need unnessesary complexity like mocking the hardware. See
    - Generation via *plantuml*. See https://plantuml.com/
- Runtime configuration of Roboter. See *'?' when running roboter for help*
    - Configurable line follower sensor treshold for left, middle and right
      seperatly
    - Configurable rounds to drive

## Requirements
Project was tested and developed on debian.
To install requirements run:
```bash
sudo apt install cmake gcc-avr binutils-avr avr-libc
```

To generate the state machine diagram you have to install `plantuml`. Also needed
is `grep`, `perl`, and `sed`, which all should be installed by default.
To install requirements run:
```bash
sudo apt install plantuml grep perl sed
```

## Build
To build this project run make in the root of the project:
```bash
make        # builds and flashes to standart arduino port
make build  # builds the project
make help   # show more commands
```

## Build documentation
On github see the pages prebuild documentation https://nilseckerle.github.io/LaborC-IES-Teil-2/

To build the documentation run
```bash
make documentation  # builds the documentation in docs/
```

### generating new state machine diagram image
The state machine setup can be drawn as a diagram. To generate it run:
```bash
make generate_state_machine_diagram
```
The image in `res/images/state_machine_diagram.png` is overwriten.

## How to develop in this Repo

### Task management System
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
1. For each task you want to do this:
    ```sh
    # Create a branch from main
    git checkout project
    git pull origin project
    git switch -c issue/descriptive-name

    # Work on your changes
    # ... make commits ...

    # Push new branch
    git push origin issue/descriptive-name
    ```
2. Add branch name to TODO.md
3. Mark Task in TODO.md as done
4. To commit you would do this, notice that we merge with `--no-ff` to preserve
   the branch: 
   ```sh
    # After review, merge with --no-ff to create merge commit
    git checkout project
    git merge --no-ff issue/descriptive-name
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
