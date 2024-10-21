#pragma once
#include "Protocol/Packets/Packet.h"

class DisconnectPacket : public Packet
{
public:
	DisconnectPacket(std::shared_ptr<Buffer> buf) : Packet(buf) {}
	DisconnectPacket(const std::string& reason) : Packet(std::make_shared<Buffer>(reason.length() + 16)) {
		int strLen = reason.length() + 1;
		int strLenVarLength = Buffer::GetVarIntSize(strLen);
		m_Buffer->WriteVarInt(strLen + strLenVarLength);
		m_Buffer->WriteVarInt(0);
		m_Buffer->WriteString(reason);
	}

	void Handle(Server& server, std::shared_ptr<Connection>& connection) override {
		connection->Close();
	}
};