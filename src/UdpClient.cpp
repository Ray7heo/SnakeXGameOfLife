#include "../include/UdpClient.h"

UdpClient::UdpClient(asio::io_context& ioContext, const std::string& serverIp, const std::string& serverPort):
    socket(ioContext), serverEndpoint(asio::ip::address::from_string(serverIp), std::stoi(serverPort)),receiveBuffer()
{
    socket.open(asio::ip::udp::v4());
}

void UdpClient::receive(std::function<void(const std::string&)> messageHandler)
{
    socket.async_receive_from(asio::buffer(receiveBuffer), remoteEndpoint,
                              [this, messageHandler](const std::error_code ec, const std::size_t bytesReceived)
                              {
                                  if (!ec && bytesReceived > 0)
                                  {
                                      std::string receivedMessage(receiveBuffer.begin(),
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
void UdpClient::send(std::string& message)
{
    socket.async_send_to(asio::buffer(message), serverEndpoint,
                         [this](const std::error_code ec, std::size_t)
                         {
                             if (ec)
                             {
                                 std::cerr << "Error sending response: " << ec.message() << "\n";
                             }
                         });
}
