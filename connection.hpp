//
// connection.hpp
//

#include <boost/asio.hpp>
#include <string>
#include <zmq.hpp>

namespace myzmq {

namespace asio = boost::asio;

class HWConnection {
public:
  HWConnection(long port, std::string const &ip);
  void send(std::string const &str);
  std::string receive();
  static void del(void *ptr);

private:
  long _port;
  asio::ip::address_v4 _ip;
  zmq::context_t _ctx;
  zmq::socket_t _sock;
};
}
