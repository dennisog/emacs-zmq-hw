//
// emacs-zmq-hw.hpp
//

extern "C" {
#include <emacs-module.h>
}

#include <string>

//
// non-extern "C" functions. These are not meant to be called by emacs.
//
std::string get_string(emacs_env *env, emacs_value val);

//
// the 0MQ <-> emacs interface
//
extern "C" {
// get a new connection
emacs_value new_connection(emacs_env *env, long nargs, emacs_value args[],
                           void *data);

// delete a connection
emacs_value delete_connection(emacs_env *env, long nargs, emacs_value args[],
                              void *data);

// send a string through the 0MQ connection
emacs_value my_zmq_send(emacs_env *env, long nargs, emacs_value args[],
                        void *data);

// receive a string from the 0MQ connection
emacs_value my_zmq_receive(emacs_env *env, long nargs, emacs_value args[],
                           void *data);
} // extern "C"

//
// emacs module housekeeping
//
extern "C" {
// bind NAME to FUN
void bind_function(emacs_env *env, const char *name, emacs_value Sfun);

// provide FEATURE to emacs
void provide(emacs_env *env, const char *feature);

// initialize the module
int emacs_module_init(struct emacs_runtime *ert);
} // extern "C"
