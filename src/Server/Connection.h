#pragma once
#include <WinSock2.h>
#include "Common/Buffer.h"
#include <optional>
#include "Protocol/Client/ClientState.h"
#include <mutex>

class Packet;
class Buffer;

class Connection
{
public:
	Connection(SOCKET socket, sockaddr_in addr);
	~Connection();

	bool IsConnected() const;
	void Close();
	SOCKET GetSocket() const { return m_Socket; }

	void SendPacket(std::shared_ptr<Buffer> buffer);
	void SendPacket(Packet&& packet);
	void SetWriteable(bool writeable) { m_Writeable = writeable; }

	std::optional<std::shared_ptr<Buffer>> RecvPacket();

	std::string GetIP() const;
	uint16_t GetPort() const;

	ClientState& GetClientState();
private:
	bool m_Writeable = false;
	SOCKET m_Socket;
	sockaddr_in m_Addr;
	ClientState m_ClientState;
};

