#include <iostream>
#include <list>

#include "statemachine.h"

enum class states_t
{
    idle,
    ringing,
    active_call
};

enum class events_t
{
    hook_pickup,
    hook_down,
    incoming_call
};

class phone_context
{
    public:
        phone_context(){}
        void add_call(std::string & number)
        {
            m_last_called.emplace_front(number);
        }
    private:
        std::list<std::string> m_last_called;
};

void generic_error(std::any & args)
{
    std::cout << "received a generic error (invalid state,event pair)" << std::endl;
}

void start_ringing(std::any & args)
{
    std::cout << "gling gling gling" << std::endl;
}

void initiate_call(std::any & args)
{
    std::cout << "hello how can i help you?" << std::endl;
}

void call_tone(std::any & args)
{
    std::cout << "beeeeeeeep" << std::endl;
}

void no_action(std::any & args)
{
    std::cout << "nothing to do here really, a no-op" << std::endl;
}

void terminate_call(std::any & args)
{
    std::cout << "goodbye!" << std::endl;
}


int main(int argc, char * argv[])
{
    statemachine<states_t, events_t> fsm(states_t::idle);

    fsm.add_handler(states_t::idle, events_t::hook_pickup, call_tone, states_t::active_call);
    fsm.add_handler(states_t::idle, events_t::incoming_call, start_ringing, states_t::ringing);
    fsm.add_handler(states_t::ringing, events_t::hook_pickup, initiate_call, states_t::active_call);
    fsm.add_handler(states_t::ringing, events_t::incoming_call, no_action, states_t::ringing);
    fsm.add_handler(states_t::active_call, events_t::hook_down, terminate_call, states_t::idle);

    phone_context context;
    auto context_param = std::make_any<phone_context>(context);

    fsm.handle_event(events_t::incoming_call, context_param);
    fsm.handle_event(events_t::hook_pickup, context_param);
    fsm.handle_event(events_t::hook_down, context_param);

    return 0;
}