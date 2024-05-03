#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

class NetworkClient
{
	sf::UdpSocket m_data_socket;
	sf::TcpSocket m_registration_socket;
	sf::Packet m_packet;
	sf::Clock m_send_time;
	sf::Int16 m_send_rate;
	sf::IpAddress m_ip;
	unsigned short m_registration_port;
	unsigned short m_data_port;
	
	sf::Socket::Status connectTcpSocket(sf::IpAddress, unsigned short);
	sf::Socket::Status sendClientData(std::string);
	sf::Socket::Status receiveDataServerPort();
public:
	NetworkClient();
	sf::Socket::Status init(unsigned short);
	sf::Socket::Status registerOnServer(sf::IpAddress, unsigned short, std::string);
	sf::Socket::Status receiveClientName(std::vector<std::string>&);
	sf::Socket::Status receiveData(sf::Packet&, sf::IpAddress, unsigned short);
	sf::Socket::Status sendData(sf::Packet);
	void setSendFrequency(sf::Int32);
};

