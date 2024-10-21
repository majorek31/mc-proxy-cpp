#pragma once
#include <Protocol/Packets/Handshake/HandshakeState.h>

class ClientState
{
public:
	ClientState();
	~ClientState();

	bool HasHandshaked() const { return m_HasHandshaked; }
	int GetProtocolVersion() const { return m_ProtocolVersion; }

	void SetHandshaked(bool handshaked) { m_HasHandshaked = handshaked; }
	void SetProtocolVersion(int version) { m_ProtocolVersion = version; }

	const std::string& GetServerAddress() const { return m_ServerAddress; }
	void SetServerAddress(const std::string& address) { m_ServerAddress = address; }

	uint16_t GetServerPort() const { return m_ServerPort; }
	void SetServerPort(uint16_t port) { m_ServerPort = port; }

	HandshakeState GetNextState() const { return m_NextState; }
	void SetNextState(HandshakeState nextState) { m_NextState = nextState; }

private:
	bool m_HasHandshaked = false;
	int m_ProtocolVersion = 0;
	std::string m_ServerAddress;
	uint16_t m_ServerPort = 0;
	HandshakeState m_NextState = HandshakeState::Status;
};

