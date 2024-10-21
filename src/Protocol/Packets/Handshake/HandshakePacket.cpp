#include "HandshakePacket.h"

HandshakePacket::HandshakePacket(std::shared_ptr<Buffer>buffer)
	: Packet(buffer)
{
}

HandshakePacket::HandshakePacket(int protocolVersion, const std::string& serverAddress, uint16_t serverPort, HandshakeState nextState) : Packet(std::make_shared<Buffer>(300))
{
	m_Buffer->WriteVarInt(0);
	m_Buffer->WriteVarInt(protocolVersion);
	m_Buffer->WriteString(serverAddress);
	m_Buffer->WriteUnsignedShort(serverPort);
	m_Buffer->WriteVarInt(static_cast<uint8_t>(nextState));
}

void HandshakePacket::Handle(Server& server, std::shared_ptr<Connection>& connection)
{
	ClientState& clientState = connection->GetClientState();
	clientState.SetProtocolVersion(m_Buffer->ReadVarInt());
	clientState.SetServerAddress(m_Buffer->ReadString());
	clientState.SetServerPort(m_Buffer->ReadUnsignedShort());
	clientState.SetNextState(static_cast<HandshakeState>(m_Buffer->ReadVarInt()));
	clientState.SetHandshaked(true);
}
