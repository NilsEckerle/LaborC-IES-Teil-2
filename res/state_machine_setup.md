# State machine setup

![State diagram generated with plantuml](res/images/state_machine_diagram.png)

In the diagrm is the current state machine setup.

**Entry:** The green dot indicates the entry state of the state machine.

**Edges:** Each arrow is a edge, that can be passed into the next state. Each edge has a
condition and some have execution function that get called when passing a edge.

**Hierachie:** Some states are grouped by a 'parent state' (They build the hierachie).
The parrent state edges are checked before the own edges are.

**Explaination no terminating state:** This state machine has no exit, because
it is ment to run on a microcontroller, thus it should never exit and terminate
the program.
If the state machine could exit and the program terminates it wouldn't be
possible to get info or a notification that the program isn't running anymore.
In the worst case the user (or some other proccess monitoring the USART
connection) could assume that the state machine is running when it is not.

See: setup and ussage in `main.c`, documentation in `state.h` and
`state_machine.h`
