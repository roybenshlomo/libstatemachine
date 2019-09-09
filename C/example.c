#include "statemachine.h"
#include <stdio.h>

typedef enum states_t
{
    IDLE,
    RINGING,
    ACTIVE_CALL,
    NUM_OF_STATES
} states_t;

typedef enum events_t
{
    HOOK_PICKUP,
    HOOK_DOWN,
    INCOMING_CALL,
    NUM_OF_EVENTS
} events_t;

void generic_error(void * args)
{
    printf("received a generic error (invalid state,event pair)\n");
}

void start_ringing(void * args)
{
    printf("gling gling gling\n");
}

void initiate_call(void * args)
{
    printf("hello how can i help you?\n");
}

void call_tone(void * args)
{
    printf("beeeeeeeep\n");
}

void no_action(void * args)
{
    printf("nothing to do here really, a no-op\n");
}

void terminate_call(void * args)
{
    printf("goodbye!\n");
}

int main(int argc, char *argv[])
{
    // create a new state machine
    statemachine_t * statemachine = create_statemachine(NUM_OF_STATES, NUM_OF_EVENTS, IDLE, generic_error);

    // populate the different state machine state,event definitions
    add_event_handler(statemachine, IDLE, HOOK_PICKUP, call_tone, ACTIVE_CALL);
    add_event_handler(statemachine, IDLE, INCOMING_CALL, start_ringing, RINGING);
    add_event_handler(statemachine, RINGING, HOOK_PICKUP, initiate_call, ACTIVE_CALL);
    add_event_handler(statemachine, RINGING, INCOMING_CALL, no_action, RINGING);
    add_event_handler(statemachine, ACTIVE_CALL, HOOK_DOWN, terminate_call, IDLE);

    // event sequence (will be triggered by different aspects of the application in a real-world scenario)
    handle_event(statemachine, INCOMING_CALL, 0);
    handle_event(statemachine, HOOK_PICKUP, 0);
    handle_event(statemachine, HOOK_DOWN, 0);
    
    // terminate state machine handle
    destory_statemachine(statemachine);
}