#pragma once
#include <memory>

#include "Common/Buffer.h"
#include "Protocol/Packets/Packet.h"

class PacketManager
{
public:
	static void HandlePacket(Server& server, std::shared_ptr<Connection> connection, std::shared_ptr<Buffer> buffer);
private:
	static void HandleSinglePacket(Server& server, std::shared_ptr<Connection> connection, std::shared_ptr<Buffer> buffer);
	static std::vector<std::shared_ptr<Buffer>> SplitBuffer(std::shared_ptr<Buffer> buffer);
};