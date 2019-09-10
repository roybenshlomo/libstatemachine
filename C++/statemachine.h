#pragma once
#include <map>
#include <functional>
#include <any>

template<class State, class Event>
class statemachine
{
    using handler_function = std::function<void(std::any &)>;
    using state_key = std::pair<State,Event>;
    using state_value = std::pair<handler_function, State>;
    using states_map = std::map<state_key, state_value>;
    public:
        /*
         * constructor
         */
        statemachine(State init_state);

        /*
         * add a handler to the state machine
         */
        void add_handler(State state, Event event, handler_function handler, State next_state);

        /*
         * handle an incoming event
         */
        void handle_event(Event event, std::any & args);
    private:
        /*
         * map holding transitions between pairs of state,event
         * to their appropriate function handler and next state
         */
        states_map m_map;
        State m_current_state;
};

template<class State, class Event>
statemachine<State,Event>::statemachine(State init_state)
{
    m_current_state = init_state;
}

template<class State, class Event>
void statemachine<State,Event>::add_handler(State state, Event event, handler_function handler, State next_state)
{
    m_map[{state,event}] = {handler, next_state};
}

template<class State, class Event>
void statemachine<State,Event>::handle_event(Event event, std::any & args)
{
    auto handler = m_map.find({m_current_state, event});

    // handler not found in map, throw an exception
    if (handler == m_map.end()) {
        throw std::runtime_error("Invalid transition of events");
    }

    handler->second.first(args);
    m_current_state = handler->second.second;
}
