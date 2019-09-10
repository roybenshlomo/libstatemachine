#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#ifdef THREAD_SAFE
#include <pthread.h>
#endif // THREAD_SAFE

/*
 * event handler function type
 */
typedef void (*event_handler_t)(void *);

/*
 * definition of a single state machine entry
 */
typedef struct statemachine_entry {
	unsigned int next_state;
	event_handler_t handler;
} statemachine_entry_t;

/*
 * definition of state machine encapsulating structure
 */
typedef struct statemachine_t {
	unsigned int current_state;
	statemachine_entry_t *states;
	unsigned int max_states;
	unsigned int max_events;
#ifdef THREAD_SAFE
	pthread_mutex_t lock;
#endif // THREAD_SAFE
} statemachine_t;

/*
 * create a state machine instance
 */
statemachine_t *create_statemachine(unsigned int num_states, unsigned int num_events, unsigned int initial_state,
				    event_handler_t error_handler);

/*
 * add an event handler to the state machine
 */
void add_event_handler(statemachine_t *statemachine, unsigned int state, unsigned int event, event_handler_t handler,
		       unsigned int next_state);

/*
 * handle an event (will run the actual handler of the event)
 */
void handle_event(statemachine_t *statemachine, unsigned int event, void *args);

/*
 * destroy state machine and deallocate all allocated memory
 */
void destory_statemachine(statemachine_t *statemachine);

#endif //_STATE_MACHINE_H_