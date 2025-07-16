# Line Follower Robot - TODO

## High Priority (P1)
- [ ] Add interactive config for ADC LF

## Medium Priority (P2)
- [ ] Change UI() - make it better
- [ ] clean main.c file - helper functions for state machine config
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

## Low Priority (P3)
- [ ] State Machine print config - print over USART the state machine config.
  Which states, what edges, super state hierarchy
- [ ] Write a tool that takes State Machine print and displays it graphicaly

## Completed
- [x] Add ADC for LF `feat/adc-line-follower`
