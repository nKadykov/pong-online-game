#include "network_client.h"

NetworkClient::NetworkClient() {}

sf::Socket::Status NetworkClient::init(unsigned short preferable_port = sf::Socket::AnyPort) {
	
	sf::Socket::Status status = m_data_socket.bind(preferable_port);

	do {
		preferable_port = sf::Socket::AnyPort;
		std::cout << "init(): Trying to bind port: " << preferable_port << std::endl;
		status = m_data_socket.bind(preferable_port);
		if (status != sf::Socket::Done) {
			std::cout << "init(): Failed to bind port. Retrying...\n";
		}
	} while (status != sf::Socket::Done);

	std::cout << "init(): Successfully binded to port: " << m_data_socket.getLocalPort() << std::endl;

	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::registerOnServer(sf::IpAddress server_ip, unsigned short server_port, std::string client_name) {

	if (connectTcpSocket(m_ip, server_port) != sf::Socket::Done) {
		return sf::Socket::Status::Error;
	}

	if (sendClientData(client_name) != sf::Socket::Status::Done) {
		return sf::Socket::Status::Error;
	}

	if (receiveDataServerPort() != sf::Socket::Status::Done) {
		return sf::Socket::Status::Error;
	}
}

sf::Socket::Status NetworkClient::receiveClientName(std::vector < std::string>& names_vector) {
	
	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}

	sf::Packet temp_packet;

	if (m_registration_socket.receive(temp_packet) != sf::Socket::Status::Done) {
		return sf::Socket::Status::Error;
	}

	if (temp_packet.getDataSize() < 1) {
		return sf::Socket::Status::Error;
	}

	while (!temp_packet.endOfPacket()) {

		std::string name;
		if (!(temp_packet >> name)) {
			std::cout << "receiveClientName(): Failed to read m_packet()\n";
			return sf::Socket::Status::Error;
		}

		if (name == "FIRST") {
			return sf::Socket::Status::Done;
		}
		names_vector.push_back(name);
	}

	std::cout << "receiveClientName(): Client names read\n";

	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::receiveData(sf::Packet& data_packet, sf::IpAddress ip, unsigned short m_data_port) {

	if (m_data_socket.isBlocking()) {
		m_data_socket.setBlocking(false);
	}

	if (m_data_socket.receive(data_packet, ip, m_data_port) != sf::Socket::Status::Done) {
		return sf::Socket::Status::NotReady;
	}

	if (data_packet.getDataSize() < 1) {
		std::cout << "receiveData(): Received m_packet is empty\n";
		return sf::Socket::Status::Error;
	}

	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::sendData(sf::Packet data_packet) {
	if (m_send_time.getElapsedTime().asMilliseconds() <= m_send_rate) {
		return sf::Socket::Status::NotReady;
	}

	if (m_data_socket.isBlocking()) {
		m_data_socket.setBlocking(false);
	}

	if (m_packet.getDataSize() == 0) {
		m_packet = data_packet;
	}

	sf::IpAddress temp_ip = m_ip;
	unsigned short temp_port = m_data_port;

	if (m_data_socket.send(m_packet, temp_ip, temp_port) != sf::Socket::Status::Done) {
		return sf::Socket::Status::NotReady;
	}

	m_packet.clear();
	m_send_time.restart();
	return sf::Socket::Status::Done;
}

void NetworkClient::setSendFrequency(sf::Int32 milliseconds) {
	m_send_rate = milliseconds;
}

sf::Socket::Status NetworkClient::connectTcpSocket(sf::IpAddress server_ip, unsigned short server_port) {

	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}

	if (m_registration_socket.connect(server_ip, server_port, sf::seconds(10)) != sf::Socket::Status::Done) {
		std::cout << "connectTcpSocket(): Error connecting to server!\n";
		return sf::Socket::Status::Error;
	}

	std::cout << "connectTcpSocket(): Connected to server\n";
	m_ip = server_ip;
	m_data_port = server_port;

	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::sendClientData(std::string client_name) {

	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}

	sf::Packet temp_packet;
	temp_packet << client_name << static_cast<sf::Uint16>(m_data_socket.getLocalPort());

	if (m_registration_socket.send(temp_packet) != sf::Socket::Status::Done) {
		std::cout << "sendClientData(): Failed to send client data\n";
		return sf::Socket::Status::Error;
	}

	std::cout << "sendClientData(): Successfully sent client data\n";
	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::receiveDataServerPort() {

	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}

	sf::Packet temp_packet;

	if (m_registration_socket.receive(temp_packet) != sf::Socket::Status::Done) {
		std::cout << "receiveDataServerPort(): Failed to receive server port\n";
		return sf::Socket::Status::Error;
	}

	if (temp_packet.getDataSize() < 1) {
		std::cout << "receiveDataServerPort(): Received packet is empty\n";
		return sf::Socket::Status::Error;
	}

	if (temp_packet.getDataSize() != sizeof(sf::Uint16)) {
		std::cout << "receiveDataServerPort(): Invalid packet size\n";
		return sf::Socket::Status::Error;
	}

	if (!(temp_packet >> m_data_port)) {
		std::cout << "receiveDataServerPort(): Failed to read from received packet\n";
		return sf::Socket::Status::Error;
	}

	std::cout << "receiveDataServerPort(): Successfully\n";
	return sf::Socket::Status::Done;
}