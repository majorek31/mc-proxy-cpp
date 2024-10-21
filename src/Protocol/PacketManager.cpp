#include "PacketManager.h"
#include "PacketType.h"
#include "Packets/Handshake/HandshakePacket.h"
#include "Packets/Status/StatusRequestPacket.h"
#include "Packets/Ping/PingRequestPacket.h"
#include "Packets/Login/LoginStartPacket.h"

void PacketManager::HandlePacket(Server& server, std::shared_ptr<Connection> connection, std::shared_ptr<Buffer> buffer)
{
	//std::cout << "Handling packet at " << std::this_thread::get_id() << " for " << connection->GetIP() << ":" << connection->GetPort() << "\n";
	auto buffers = SplitBuffer(buffer);
	for (auto buf : buffers) {
		HandleSinglePacket(server, connection, buf);
	}
}

void PacketManager::HandleSinglePacket(Server& server, std::shared_ptr<Connection> connection, std::shared_ptr<Buffer> buffer)
{
	int packetLength = buffer->ReadVarInt();
	PacketType packetType = (PacketType)buffer->ReadVarInt();
	if (!connection->GetClientState().HasHandshaked()) {
		if (packetType != PacketType::Handshake) {
			connection->Close();
			return;
		}
		HandshakePacket handshakePacket(buffer);
		handshakePacket.Handle(server, connection);
		return;
	}

	switch (packetType)
	{
	case PacketType::StatusRequest:
	{
		auto clientNextState = connection->GetClientState().GetNextState();
		if (clientNextState == HandshakeState::Status) {
			StatusRequestPacket statusRequestPacket(buffer);
			statusRequestPacket.Handle(server, connection);
		}
		else if (clientNextState == HandshakeState::Login) {
			LoginStartPacket loginStartPacket(buffer);
			loginStartPacket.Handle(server, connection);
			break;
		}
		else connection->Close();
		break;
	}
	case PacketType::PingRequest:
	{
		PingRequestPacket pingRequestPacket(buffer);
		pingRequestPacket.Handle(server, connection);
		break;
	}
	default:
		std::cerr << "Unhandled packet type: " << (int)packetType << "\n";
		connection->Close();
		break;
	}
}

std::vector<std::shared_ptr<Buffer>> PacketManager::SplitBuffer(std::shared_ptr<Buffer> buffer)
{
	std::vector<std::shared_ptr<Buffer>> buffers;

	while (buffer->GetCursor() < buffer->GetCapacity())
	{
		int packetLength = buffer->ReadVarInt();
		if (packetLength <= 0) {
			break;
		}

		int packetStartCursor = buffer->GetCursor();

		int packetId = buffer->ReadVarInt();

		int dataLength = packetLength - Buffer::GetVarIntSize(packetId);

		if (dataLength < 0 || packetStartCursor + packetLength > buffer->GetCapacity()) {
			break;
		}

		buffers.push_back(buffer->Split(packetStartCursor - 1, packetLength + Buffer::GetVarIntSize(packetLength)));

		buffer->SetCursor(packetStartCursor + packetLength);
	}
	return buffers;
}
