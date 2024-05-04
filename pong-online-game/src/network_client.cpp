#include "network_client.h"

NetworkClient::NetworkClient() {}

sf::Socket::Status NetworkClient::init(unsigned short preferablePort) {
	sf::Socket::Status status = m_data_socket.bind(preferablePort);
	if (status == sf::Socket::Status::Done)
	{
		std::cout << "init(): Successfully binded to port: " << m_data_socket.getLocalPort() << std::endl;
		return sf::Socket::Status::Done;
	}
	std::cout << "init(): Failed to bind passed preferred port\n";
	do
	{
		unsigned short newPort = sf::Socket::AnyPort;
		std::cout << "init(): Trying to bind other port - " << newPort << std::endl;
		status = m_data_socket.bind(newPort);
		if (status != sf::Socket::Status::Done) {
			std::cout << "init(): Failed to bind other port. Retrying...\n";
		}

	} while (status != sf::Socket::Done);
	std::cout << "init(): Successfully binded to other port - " << m_data_socket.getLocalPort() << std::endl;
	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::registerOnServer(sf::IpAddress serverIp, unsigned short serverRegPort, std::string clientName) {
	if (connectTcpSocket(serverIp, serverRegPort) != sf::Socket::Status::Done)
		return sf::Socket::Status::Error;
	if (sendClientData(clientName) != sf::Socket::Status::Done)
		return sf::Socket::Status::Error;
	if (receiveDataServerPort() != sf::Socket::Status::Done)
		return sf::Socket::Status::Error;
}

sf::Socket::Status NetworkClient::receiveConnectedClientName(std::vector<std::string>& namesVec) {
	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}
	sf::Packet temp_packet;
	if (m_registration_socket.receive(temp_packet) != sf::Socket::Status::Done) {
		std::cout << "receiveClientName(): Failed to receive clients names\n";
		return sf::Socket::Status::Error;
	}
	if (temp_packet.getDataSize() < 1) {
		std::cout << "receiveClientName(): Receives packet is empty, ensure that packet contains: (string name1 << string name2 << ...) or \"FIRST\" if it's first connected client\n";
		return sf::Socket::Status::Error;
	}
	while (!temp_packet.endOfPacket()) {
		std::string name;
		if (temp_packet >> name) {
			if (name == "FIRST") {
				std::cout << "receiveClientName(): No clients are connected, you are first\n";
				return sf::Socket::Status::Done;
			}
			namesVec.push_back(name);
		}
		else {
			std::cout << "receiveClientName() : Failed to read packet\n";
			return sf::Socket::Status::Error;
		}
	}
	std::cout << "receiveClientName(): Client names read\n";
	return sf::Socket::Status::Done;
}

sf::Socket::Status NetworkClient::receiveData(sf::Packet& data_packet, sf::IpAddress ip, unsigned short data_port) {
	if (m_data_socket.isBlocking()) {
		m_data_socket.setBlocking(false);
	}
	if (m_data_socket.receive(data_packet, ip, data_port) != sf::Socket::Status::Done) {
		return sf::Socket::Status::NotReady;
	}
	if (data_packet.getDataSize() > 0) {
		return sf::Socket::Status::Done;
	}
	std::cout << "receiveData(): Received packet is empty\n";
	return sf::Socket::Status::Error;
}

sf::Socket::Status NetworkClient::sendData(sf::Packet dataPacket) {
	if (m_send_time.getElapsedTime().asMilliseconds() > m_send_rate) {
		if (m_data_socket.isBlocking()) {
			m_data_socket.setBlocking(false);
		}
		if (m_send_packet.getDataSize() == 0) {
			m_send_packet = dataPacket;
		}
		sf::IpAddress temp_ip = m_ip;
		unsigned short tempDataPort = m_data_port;
		if (m_data_socket.send(m_send_packet, temp_ip, tempDataPort) == sf::Socket::Status::Done) {
			m_send_packet.clear();
			m_send_time.restart();
			return sf::Socket::Status::Done;
		}
		else {
			return sf::Socket::Status::NotReady;
		}
	}
	else {
		return sf::Socket::Status::NotReady;
	}
}

void NetworkClient::setSendFrequency(sf::Int32 milliseconds) {
	m_send_rate = milliseconds;
}

sf::Socket::Status NetworkClient::connectTcpSocket(sf::IpAddress serverIp, unsigned short serverRegPort) {
	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}
	if (m_registration_socket.connect(serverIp, serverRegPort, sf::seconds(10)) == sf::Socket::Status::Done) {
		std::cout << "connectTcpSocket(): Connected to server\n";
		m_ip = serverIp;
		m_data_port = serverRegPort;
		return sf::Socket::Status::Done;
	}
	else {
		std::cout << "connectTcpSocket(): Error connecting to server!\n";
		return sf::Socket::Status::Error;
	}
}

sf::Socket::Status NetworkClient::sendClientData(std::string clientName) {
	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}
	sf::Packet temp_packet;
	temp_packet << clientName << static_cast<sf::Uint16>(m_data_socket.getLocalPort());
	if (m_registration_socket.send(temp_packet) != sf::Socket::Status::Done) {
		std::cout << "sendClientData(): Failed to send client recipient data\n";
		return sf::Socket::Status::Error;
	}
	{
		std::cout << "sendClientData(): Successfully sent client recipient data\n";
		return sf::Socket::Status::Done;
	}
}

sf::Socket::Status NetworkClient::receiveDataServerPort() {
	if (!m_registration_socket.isBlocking()) {
		m_registration_socket.setBlocking(true);
	}
	sf::Packet temp_packet;
	if (m_registration_socket.receive(temp_packet) != sf::Socket::Status::Done) {
		std::cout << "receiveDataServerPort(): Failed to receive client port of a server\n";
		return sf::Socket::Status::Error;
	}
	if (temp_packet.getDataSize() < 1) {
		std::cout << "receiveDataServerPort(): Received packet is empty\n";
		return sf::Socket::Status::Error;
	}
	if (temp_packet.getDataSize() != sizeof(sf::Uint16)) {
		std::cout << "receiveDataServerPort(): Invalid packet size, ensure that server sends only Uint16 var\n";
		return sf::Socket::Status::Error;
	}
	if (temp_packet >> m_data_port) {
		std::cout << "receiveDataServerPort(): Successfully received data client port of a server - " << m_data_port << std::endl;
		return sf::Socket::Status::Done;
	}
	else {
		std::cout << "receiveDataServerPort: Failed to read from packet\n";
	}
	return sf::Socket::Status::Error;
}