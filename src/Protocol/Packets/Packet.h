#pragma once
#include <memory>
#include "Server/Connection.h"
#include "Server/Server.h"

class Packet
{
public:
	virtual ~Packet() = default;
	Packet(std::shared_ptr<Buffer> buffer) : m_Buffer(buffer) {}
	std::shared_ptr<Buffer> GetBuffer() { return m_Buffer; }
	virtual void Handle(Server& server, std::shared_ptr<Connection>& connection) = 0;
protected:
	std::shared_ptr<Buffer> m_Buffer;
};