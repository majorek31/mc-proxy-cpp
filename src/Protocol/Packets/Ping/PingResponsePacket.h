#pragma once

#include "Protocol/Packets/Packet.h"

class PingResponsePacket : public Packet
{
public:
	PingResponsePacket(std::shared_ptr<Buffer> buffer) : Packet(buffer) {}
	PingResponsePacket(int64_t payload) : Packet(std::make_shared<Buffer>(16))
	{
		m_Buffer->WriteVarInt(9);
		m_Buffer->WriteVarInt(1);
		m_Buffer->WriteLong(payload);
	}

	void Handle(Server& server, std::shared_ptr<Connection>& connection) override
	{
	}
};