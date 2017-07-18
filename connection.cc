#include "connection.hpp"
#include <boost/format.hpp>

namespace myzmq {

HWConnection::HWConnection(long port, std::string const &ip)
    : _port(port), _ip(boost::asio::ip::address_v4::from_string(ip)),
      _ctx(zmq::context_t(1)), _sock(_ctx, ZMQ_REQ) {
  _sock.connect((boost::format("tcp://%s:%d") % _ip.to_string() % port).str());
}

void HWConnection::send(std::string const &str) {
  zmq::message_t msg(str.length());
  memcpy(msg.data(), str.c_str(), str.length());
  _sock.send(msg);
}

std::string HWConnection::receive() {
  zmq::message_t msg;
  _sock.recv(&msg);
  std::string str;
  str.assign(static_cast<char *>(msg.data()), msg.size());
  return str;
}

// dirty, but works?
void HWConnection::del(void *ptr) { delete static_cast<HWConnection *>(ptr); }
} // namespace myzmq
