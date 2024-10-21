#pragma once
#include "Protocol/Packets/Packet.h"
#include "PingResponsePacket.h"

class PingRequestPacket : public Packet
{
	public:
	PingRequestPacket(std::shared_ptr<Buffer> buffer) : Packet(buffer) {}
	PingRequestPacket(int64_t payload) : Packet(std::make_shared<Buffer>(8))
	{
	}

	void Handle(Server& server, std::shared_ptr<Connection>& connection) override
	{
		int64_t payload = m_Buffer->ReadLong();
		connection->SendPacket(PingResponsePacket(payload));
	}
};