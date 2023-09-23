#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool.hpp"


void handle_client(boost::asio::ip::tcp::socket& socket) {
  // Zpracování příchozího požadavku a odeslání odpovědi
  std::string response =
      "HTTP/1.1 200 OK\r\nContent-Length: 14\r\n\r\nHello, World!\n";
  boost::asio::write(socket, boost::asio::buffer(response));
}

int main() {
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::acceptor acceptor(
      io_context,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

  aurora_nexus::ThreadPool pool(4);

  while (true) {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    acceptor.accept(*socket);

    // Vytvoření Tasku a přidání do ThreadPoolu
    aurora_nexus::Task const task([socket] {
      handle_client(*socket);
    });

    pool.Enqueue(task);
  }

  return 0;
}
