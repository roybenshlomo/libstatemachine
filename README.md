# libstatemachine
Simple implementation(s) of a finite state machine

I just bought a mechanical keyboard and wanted to write some code :)

I have been an engineering manager for the last few years, this is an old interview question I probably asked over 200 times interviewing software engineering candidates and saw different types of solutions. I think it's a good design question because it shows how people are modelling a problem (a simple one in this case) and how well and fast they are able to understand a new concept and implement it, what is a generic, reusable facility and how an API is designed. It also helps place candidates on a software engineering "spectrum" - from electrical engineers that solve it with a set of if-else statements to high level engineers solving it with OOP (and of course those who don't get the concept at all and usually end up drawing a set of circles and arrows to illustrate the state machine).

The implementations here (in C and C++) are my favorites, there are a lot of other good implementations, but I like the simplicity of the API and the usability of the library from an external user which in my eyes is how you measure a good library (if someone who doesn't know it at all can easily use it and it would just work).

# The original question I ask in interviews
A state machine is a generic concept that defines a set of states and events, that given a pair of state and event the state machine performs an operation and either moves to a different state or stays in the same one. Being generic, a state machine can represent a variety of different system - a phone, a traffic light, a vending machine etc.
Your task is to implement a generic state machine (a library) that will be used by other developers, think about the data structures you would use to model the problem, what is the API you provide to your users and how the statemachine engine really operates (i.e how given an event it performs the correct operation and shifts states).

I usually provide this simple example that represents a desk phone

| State  | Event | Activity | Next State |
| ------ | ----- | -------- | ---------- |
| IDLE   | INCOMING_CALL | START_RINGING | RINGING |
| IDLE   | HOOK_PICKUP | INITIATE_CALL | ACTIVE_CALL |
| RINGING   | HOOK_PICKUP | INITIATE_CALL | ACTIVE_CALL |
| ACTIVE_CALL   | HOOK_DOWN | TERMINATE_CALL | IDLE |


# C implementation
The C implementation defines the state machine as a two-dimensional array (one dimension represents the states, the other represents the possible events), each element in the array is a structure that includes a function pointer for the activity to perform and the next state the state machine needs to go to.

# C++ implementation
The C++ implementation is very similar in concept but in my opinion is a little "fancier", it uses a template type for the states and the events, given higher versatility of the states/events definitions (for example they can be a string) and uses a map with key being pair of [state, event] and value a pair of [funtion pointer, state]. It's a header only implementation, what I like about it is that it's very clean, it's about 3 times fewer lines of code compared to the C implementation.

# Thread-safety
The state machine default behavior is not thread-safe, i.e there is no synchronization between different threads inserting events to the state machine. Thread safety can be enabled (in both implementations) using the compiler definition **THREAD_SAFE**, there are two reasons I made the design choice of not having it enabled by default:
1. I think the right design for a state machine is to have its own thread waiting on a queue for inserted events that takes care of synchronization between different threads wanting to insert events into the state machine.
2. I want the implementation to be as portable as possible, I have used a similar implementation to the C implementation on various embedded systems that did not have an OS at all let alone a pthread library.
