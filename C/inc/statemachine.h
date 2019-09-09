#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

/*
 * event handler function type
 */
typedef void (*event_handler)(void *);

/*
 * definition of a single state machine entry
 */
typedef struct statemachine_entry_t
{
    unsigned int next_state;
    event_handler handler;
} statemachine_entry_t;

/*
 * definition of state machine encapsulating structure
 */
typedef struct statemachine_t
{
    unsigned int current_state;
    statemachine_entry_t *states;
    unsigned int max_states;
    unsigned int max_events;
} statemachine_t;

/*
 * create a state machine instance
 */
statemachine_t * create_statemachine(unsigned int num_states, unsigned int num_events, unsigned int initial_state);

/*
 * add an event handler to the state machine
 */
void add_event_handler(statemachine_t * statemachine, unsigned int state, unsigned int event, event_handler handler, unsigned int next_state);

/*
 * handle an event (will run the actual handler of the event)
 */
void handle_event(statemachine_t * statemachine, unsigned int event, void * args);

/*
 * destroy state machine and deallocate all allocated memory
 */
void destory_statemachine(statemachine_t * statemachine);

#endif //_STATE_MACHINE_H_