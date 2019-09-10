#include "statemachine.h"
#include <stdlib.h>

/*
 * get entry at the appropriate field in the [state, event] matrix
 */
statemachine_entry_t *get_entry(statemachine_t *statemachine, unsigned int state, unsigned int event)
{
	return (statemachine_entry_t *)(statemachine->states + (state * statemachine->max_events + event));
}

/*
 * create a state machine instance
 */
statemachine_t *create_statemachine(unsigned int num_states, unsigned int num_events, unsigned int initial_state,
				    event_handler error_handler)
{
	// allocate a new state machine
	statemachine_t *statemachine = (statemachine_t *)malloc(sizeof(statemachine_t));

	// check that allocation succeeded
	if (statemachine) {
		// set initial state
		statemachine->current_state = initial_state;

		// set maximal values
		statemachine->max_events = num_events;
		statemachine->max_states = num_states;

		// allocate the data structures for the statemachines
		statemachine->states =
			(statemachine_entry_t *)malloc(sizeof(statemachine_entry_t) * num_states * num_events);

		for (unsigned int state = 0; state < num_states; state++) {
			for (unsigned int event = 0; event < num_events; event++) {
				statemachine_entry_t *current_entry = get_entry(statemachine, state, event);
				current_entry->handler = error_handler;
				current_entry->next_state = state;
			}
		}
#ifdef THREAD_SAFE
		if (pthread_mutex_init(&statemachine->lock,0) != 0) {
			destory_statemachine(statemachine);
		}
#endif // THREAD_SAFE
	}

	return statemachine;
}

/*
 * destroy state machine and deallocate all allocated memory
 */
void destory_statemachine(statemachine_t *statemachine)
{
#ifdef THREAD_SAFE
	// release allocated mutex
	pthread_mutex_destroy(&statemachine->lock);
#endif // THREAD_SAFE

	// release allocated memory
	if (statemachine->states) {
		free(statemachine->states);
		statemachine->states = 0;
	}

	if (statemachine) {
		free(statemachine);
		statemachine = 0;
	}
}

/*
 * add an event handler to the state machine
 */
void add_event_handler(statemachine_t *statemachine, unsigned int state, unsigned int event, event_handler handler,
		       unsigned int next_state)
{
	// check that the state and event are not out of range
	if (state >= statemachine->max_states || event >= statemachine->max_events) {
		return;
	}

#ifdef THREAD_SAFE
	pthread_mutex_lock(&statemachine->lock);
#endif // THREAD_SAFE

	statemachine_entry_t *entry = get_entry(statemachine, state, event);

	if (entry) {
		entry->handler = handler;
		entry->next_state = next_state;
	}

#ifdef THREAD_SAFE
	pthread_mutex_unlock(&statemachine->lock);
#endif // THREAD_SAFE    
}

/*
 * handle an event (will run the actual handler of the event)
 */
void handle_event(statemachine_t *statemachine, unsigned int event, void *args)
{
	// check that event is within allowed range
	if (event >= statemachine->max_events) {
		return;
	}

#ifdef THREAD_SAFE
	pthread_mutex_lock(&statemachine->lock);
#endif // THREAD_SAFE

	statemachine_entry_t *entry = get_entry(statemachine, statemachine->current_state, event);

	if (entry) {
		if (entry->handler) {
			entry->handler(args);
		}
		statemachine->current_state = entry->next_state;
	}

#ifdef THREAD_SAFE
	pthread_mutex_unlock(&statemachine->lock);
#endif // THREAD_SAFE
}
