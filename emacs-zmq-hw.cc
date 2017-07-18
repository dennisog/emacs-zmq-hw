//
// emacs_interface.cc
//
// Lots of this is from http://diobla.info/blog-archive/modules-tut.html
// and from https://github.com/jkitchin/emacs-modules/blob/master/zeromq
//
// Demo of an emacs client to a basic zmq

#include "emacs-zmq-hw.hpp"
#include "connection.hpp"

int plugin_is_GPL_compatible;

//
// non-extern "C" functions. These are not meant to be called by emacs.
//
std::string get_string(emacs_env *env, emacs_value val) {
  ptrdiff_t size = 0;
  env->copy_string_contents(env, val, NULL, &size);
  std::vector<char> buf(size);
  env->copy_string_contents(env, val, buf.data(), &size);
  std::string str;
  str.assign(buf.data(), size);
  return str;
}

//
// the 0MQ <-> emacs interface
//

extern "C" {
emacs_value new_connection(emacs_env *env, long nargs, emacs_value args[],
                           void *data) {
  long port = 5555;
  std::string ip("127.0.0.1");
  auto conn = static_cast<void *>(new myzmq::HWConnection(port, ip));
  return env->make_user_ptr(env, myzmq::HWConnection::del, conn);
}

emacs_value delete_connection(emacs_env *env, long nargs, emacs_value args[],
                              void *data) {
  auto conn =
      static_cast<myzmq::HWConnection *>(env->get_user_ptr(env, args[0]));
  myzmq::HWConnection::del(conn);
  return env->intern(env, "t");
}

emacs_value my_zmq_send(emacs_env *env, long nargs, emacs_value args[],
                        void *data) {
  auto conn =
      static_cast<myzmq::HWConnection *>(env->get_user_ptr(env, args[0]));
  auto str = get_string(env, args[1]);
  conn->send(str);
  return env->intern(env, "t");
}

emacs_value my_zmq_receive(emacs_env *env, long nargs, emacs_value args[],
                           void *data) {
  auto conn =
      static_cast<myzmq::HWConnection *>(env->get_user_ptr(env, args[0]));
  auto str = conn->receive();
  return env->make_string(env, str.data(), str.length());
}

} // extern "C"

//
// the emacs interface
//

/* Bind NAME to FUN.  */
extern "C" {
void bind_function(emacs_env *env, const char *name, emacs_value Sfun) {
  /* Set the function cell of the symbol named NAME to SFUN using
     the 'fset' function.  */

  /* Convert the strings to symbols by interning them */
  emacs_value Qfset = env->intern(env, "fset");
  emacs_value Qsym = env->intern(env, name);

  /* Prepare the arguments array */
  emacs_value args[] = {Qsym, Sfun};

  /* Make the call (2 == nb of arguments) */
  env->funcall(env, Qfset, 2, args);
}

/* Provide FEATURE to Emacs.  */
void provide(emacs_env *env, const char *feature) {
  /* call 'provide' with FEATURE converted to a symbol */

  emacs_value Qfeat = env->intern(env, feature);
  emacs_value Qprovide = env->intern(env, "provide");
  emacs_value args[] = {Qfeat};

  env->funcall(env, Qprovide, 1, args);
}

int emacs_module_init(struct emacs_runtime *ert) {
  emacs_env *env = ert->get_environment(ert);

  // bind the functions
  bind_function(env, "zmq-hw-new-connection",
                env->make_function(env, 0, 0, new_connection,
                                   "Make a new connection.", NULL));
  bind_function(env, "zmq-hw-delete-connection",
                env->make_function(env, 1, 1, delete_connection,
                                   "Delete a connection.", NULL));
  bind_function(
      env, "zmq-hw-send",
      env->make_function(env, 2, 2, my_zmq_send, "Send a message.", NULL));
  bind_function(
      env, "zmq-hw-receive",
      env->make_function(env, 1, 1, my_zmq_receive, "Send a message.", NULL));

  provide(env, "emacs-zmq-hw");

  return 0;
}
} // extern "C"
