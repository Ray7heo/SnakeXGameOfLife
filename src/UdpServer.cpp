#include "../include/UdpServer.h"

UdpServer::UdpServer(asio::io_context& ioContext, const std::string& listenPort):
    socket(ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), std::stoi(listenPort))), receiveBuffer(std::array<char, 1024>())
{
}

void UdpServer::send(std::string& message)
{
    socket.async_send_to(asio::buffer(message), remoteEndpoint,
                         [this](const std::error_code ec, std::size_t)
                         {
                             if (ec)
                             {
                                 std::cerr << "Error sending response: " << ec.message() << "\n";
                             }
                         });
}

void UdpServer::receive(const std::function<void(const std::string&)>& messageHandler)
{
    socket.async_receive_from(asio::buffer(receiveBuffer), remoteEndpoint,
                              [this, messageHandler](const std::error_code ec, const std::size_t bytesReceived)
                              {
                                  if (!ec && bytesReceived > 0)
                                  {
                                      const std::string receivedMessage(receiveBuffer.begin(),
                                                                        receiveBuffer.begin() + bytesReceived);
                                      if (messageHandler)
                                      {
                                          messageHandler(receivedMessage);
                                      }
                                      receive(messageHandler);
                                  }
                                  else
                                  {
                                      std::cerr << "Error receiving message: " << ec.message() << "\n";
                                      receive(messageHandler);
                                  }
                              });
}
