#pragma once

#include <iostream>
#include <asio.hpp>


class UdpServer
{
public:
    UdpServer(asio::io_context& ioContext, const std::string& listenPort);

    void receive(std::function<void(const std::string&)> messageHandler);
    void send(std::string& message);

private:
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint remoteEndpoint;
    std::array<char, 1024> receiveBuffer;
};


