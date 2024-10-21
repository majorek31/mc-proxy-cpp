#pragma once
#include "Protocol/Packets/Packet.h"
#include "Protocol/Packets/Handshake/HandshakeState.h"


class HandshakePacket : public Packet
{

public:
	HandshakePacket(std::shared_ptr<Buffer> buffer);
	HandshakePacket(int protocolVersion, const std::string& serverAddress, uint16_t serverPort, HandshakeState nextState);

	void Handle(Server& server, std::shared_ptr<Connection>& connection) override;
};