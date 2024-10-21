#include "Connection.h"
#include <WS2tcpip.h>
#include "Protocol/Packets/Packet.h"

Connection::Connection(SOCKET socket, sockaddr_in addr) : m_Socket(socket), m_Addr(addr)
{
	u_long nonBlocking = 1;
	//ioctlsocket(m_Socket, FIONBIO, &nonBlocking);
}

Connection::~Connection()
{
	Close();
}

bool Connection::IsConnected() const
{
	return (this) && m_Socket != INVALID_SOCKET;
}

void Connection::Close()
{
	if (this && m_Socket != INVALID_SOCKET)
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
}

void Connection::SendPacket(std::shared_ptr<Buffer> buffer)
{
	send(m_Socket, buffer->GetBuffer(), buffer->GetSize(), 0);
	return;
}

void Connection::SendPacket(Packet&& packet)
{
	SendPacket(packet.GetBuffer());
}

std::optional<std::shared_ptr<Buffer>> Connection::RecvPacket()
{
	char buffer[1024] = { 0 };
	int bytesReceived = recv(m_Socket, buffer, sizeof(buffer), 0);
	if (bytesReceived > 0)
	{
		return std::make_shared<Buffer>(buffer, bytesReceived);
	}
	else if (bytesReceived == 0) {
		Close();
	}
	if (WSAGetLastError() != 0)
		Close();
	return std::nullopt;
	/*
	std::unique_lock<std::mutex> lock(m_Mutex);
	WSAPOLLFD pollfd = { m_Socket, POLLRDNORM, 0 };
	int ret = WSAPoll(&pollfd, 1, -1);
	if (ret <= 0) {
		std::cout << "WSAPoll error or timeout.\n";
		return std::nullopt;
	}
	if (pollfd.revents & POLLHUP) {
		std::cout << "Poll hang up.\n";
		Close();
	}
	if (!(pollfd.revents & POLLRDNORM)) {
		std::cout << "No data to read.\n";
		return std::nullopt;
	}
	char buffer[1024] = { 0 };
	std::cout << "No bytes received.\n";
	int bytesReceived = recv(m_Socket, buffer, sizeof(buffer), 0);
	if (bytesReceived > 0)
	{
		std::cout << "Received " << bytesReceived << " bytes.\n";
		return Buffer(buffer, bytesReceived);
	}
	return std::nullopt;
	*/
}

std::string Connection::GetIP() const
{
	char ip[INET_ADDRSTRLEN] = { 0 };
	inet_ntop(m_Addr.sin_family, &(m_Addr.sin_addr), ip, INET_ADDRSTRLEN);
	return std::string(ip);
}

uint16_t Connection::GetPort() const
{
	return ntohs(m_Addr.sin_port);
}

ClientState& Connection::GetClientState()
{
	return m_ClientState;
}
