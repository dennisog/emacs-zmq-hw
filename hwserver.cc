//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  modified to ping back whatever it received
//
#include <iostream>
#include <string>
#include <zmq.hpp>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n) Sleep(n)
#endif

int main() {
  //  Prepare our context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REP);
  socket.bind("tcp://127.0.0.1:5555");

  while (true) {
    zmq::message_t request;

    //  Wait for next request from client
    socket.recv(&request);
    std::string str;
    str.assign(static_cast<char *>(request.data()), request.size());
    std::cout << "Received: " << str << std::endl;

    //  Do some 'work'
    sleep(1);

    //  Send reply back to client
    zmq::message_t reply(str.length());
    memcpy(reply.data(), str.data(), str.length());
    socket.send(reply);
  }
  return 0;
}
