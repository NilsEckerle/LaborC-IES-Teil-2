# Line Follower Robot - TODO
## High Priority (P1)

## Medium Priority (P2)
- [ ] drive backward onto start after finishing all rounds

## Low Priority (P3)

## Completed
- [x] Add ADC for LF `feat/adc-line-follower`
- [x] Add interactive config for ADC LF `feat/running-robot-lf-config`
- [x] read Arbeitsblatt and correct Lastenhaeft
- [x] make plan in which order i do the Lastenhaeft
- [x] create `roboter_settings.h` and create `serial_messages.h` and use them
- [x] set round default to 3 rounds
- [x] set start steuer command to `S`
- [x] set help steuer command to `h`
- [x] clean main.c file - helper functions for state machine config
- [x] setup `P` steuer command
- [x] Change UI() - make it better
- [x] check Lastenhaeft for new tasks and mark completed
- [x] Add documentation where it is missing
- [x] document how state machine works in doxygen (not code)
- [x] chek if LF can be implemented with bit fields. -- not needed --
    ```c
    // rest bit of uint8_t | 1bit | 1bit | 1bit
    typedef struct LF {
        uint8_t rest : 5;
        uint8_t left : 1;
        uint8_t middle : 1;
        uint8_t right : 1;
    } t_LF
    void main() {
        t_LF lf = {};
        lf.left = HIGH;
        lf.middle = LOW;
        lf.right = LOW;
    }
    ```
- [x] State Machine print config - print over USART the state machine config.
  Which states, what edges, super state hierarchy
- [x] Write a tool that takes State Machine print and displays it graphically `feat/state-machine-diagram-generation` / `feat/state-machine-diagram--generation-extern-script`
- [x] build state machine diagram system: -- scraped --
      write 2nd `state.c` and `state_machine.c` which doesnt add or configures the
      machine, but prints out everything so i can parse the output with another
      tool.
