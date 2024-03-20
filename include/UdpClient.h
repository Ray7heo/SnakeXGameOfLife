#pragma once

#include <iostream>
#include <asio.hpp>


class UdpClient
{
public:
    UdpClient(asio::io_context& ioContext, const std::string& serverIp, const std::string& serverPort);

    void send(std::string& data, const std::function<void(std::error_code ec, std::size_t)>& errorHandler);
    void receive(const std::function<void(const std::string&)>& messageHandler);

private:
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint serverEndpoint;
    asio::ip::udp::endpoint remoteEndpoint;
    std::array<char, 1024> receiveBuffer;
};
