# Line Follower Robot - TODO

## High Priority (P1)
- [ ] create `roboter_settings.h` and create `serial_messages.h` and use them
- [ ] set round default to 3 rounds
- [ ] set start steuer command to `S`
- [ ] set help steuer command to `h`
- [ ] clean main.c file - helper functions for state machine config
- [ ] setup `P` steuer command
- [ ] Change UI() - make it better
- [ ] check Lastenhaeft for new tasks and mark completed


## Medium Priority (P2)
- [ ] Add documentation where it is missing
- [ ] document how state machine works in doxygen (not code)
- [ ] chek if LF can be implemented with bit fields.
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
- [ ] drive backwartd onto start after finishing all rounds

## Low Priority (P3)
- [ ] State Machine print config - print over USART the state machine config.
  Which states, what edges, super state hierarchy
- [ ] Write a tool that takes State Machine print and displays it graphicaly

## Completed
- [x] Add ADC for LF `feat/adc-line-follower`
- [x] Add interactive config for ADC LF `feat/running-robot-lf-config`
- [x] read Arbeitsblatt and correct Lastenhaeft
- [x] make plan in which order i do the Lastenhaeft
