#pragma once
#include "Protocol/Packets/Packet.h"
#include "Protocol/PacketType.h"

class StatusResponsePacket : public Packet
{
public:
	StatusResponsePacket(std::shared_ptr<Buffer> buffer) : Packet(buffer) {}
	StatusResponsePacket(const std::string& response)
		: Packet(std::make_shared<Buffer>(response.length() + 10))
	{
		m_Buffer->WriteVarInt(response.length() + 3);
		m_Buffer->WriteVarInt(0);
		m_Buffer->WriteString(response);
	}


	void Handle(Server& server, std::shared_ptr<Connection>& connection) override
	{
	}
};