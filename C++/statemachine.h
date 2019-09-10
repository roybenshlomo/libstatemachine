#pragma once
#include <map>
#include <functional>
#include <any>
#ifdef THREAD_SAFE
#include <mutex>
#endif // THREAD_SAFE

template<class State, class Event>
class statemachine
{
    using handler_function_t = std::function<void(std::any &)>;
    using state_key_t = std::pair<State,Event>;
    using state_value_t = std::pair<handler_function_t, State>;
    using states_map_t = std::map<state_key_t, state_value_t>;
    public:
        /*
         * constructor
         */
        statemachine(State init_state);

        /*
         * add a handler to the state machine
         */
        void add_handler(State state, Event event, handler_function_t handler, State next_state);

        /*
         * handle an incoming event
         */
        void handle_event(Event event, std::any & args);
    private:
        /*
         * map holding transitions between pairs of state,event
         * to their appropriate function handler and next state
         */
        states_map_t m_map;
        State m_current_state;
        
        #ifdef THREAD_SAFE
        std::mutex m_lock;
        #endif // THREAD_SAFE
};

template<class State, class Event>
statemachine<State,Event>::statemachine(State init_state)
{
    m_current_state = init_state;
}

template<class State, class Event>
void statemachine<State,Event>::add_handler(State state, Event event, handler_function_t handler, State next_state)
{
    #ifdef THREAD_SAFE
    std::scoped_lock lock(m_lock);
    #endif // THREAD_SAFE
    m_map[{state,event}] = {handler, next_state};
}

template<class State, class Event>
void statemachine<State,Event>::handle_event(Event event, std::any & args)
{
    #ifdef THREAD_SAFE
    std::scoped_lock lock(m_lock);
    #endif // THREAD_SAFE
    auto handler = m_map.find({m_current_state, event});

    // handler not found in map, throw an exception
    if (handler == m_map.end()) {
        throw std::runtime_error("Invalid state machine transition");
    }

    handler->second.first(args);
    m_current_state = handler->second.second;
}
