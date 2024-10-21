#pragma once
#include <iostream>

#include "Protocol/Packets/Packet.h"
#include "DisconnectPacket.h"

class LoginStartPacket : public Packet
{
	public:
	LoginStartPacket(std::shared_ptr<Buffer> buffer) : Packet(buffer) {}
	LoginStartPacket(const std::string& name, uint64_t uuidLow, uint64_t uuidHigh) : Packet(std::make_shared<Buffer>(128)) {
		m_Buffer->WriteVarInt(name.length() + 16 + 1);
		m_Buffer->WriteVarInt(0);
		m_Buffer->WriteString(name);
		m_Buffer->WriteLong(uuidHigh);
		m_Buffer->WriteLong(uuidLow);
	}

	void Handle(Server& server, std::shared_ptr<Connection>& connection) override {
		std::string name = m_Buffer->ReadString();
		uint64_t uuidLow = m_Buffer->ReadLong();
		uint64_t uuidHigh = m_Buffer->ReadLong();
		connection->SendPacket(DisconnectPacket(
R"(
{
	"text": "",
	"extra": [
		{
			"text": "MC Protect doesn't allow direct server connections!\n",
			"color": "red",
			"bold": true
		},
		{
			"text": "Please try with correct server address",
			"color": "gold"
		}
	]
}
)"));
	}
};