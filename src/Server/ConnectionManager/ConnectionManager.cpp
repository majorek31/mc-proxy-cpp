#include "ConnectionManager.h"

#include <algorithm>
#include <WinSock2.h>
#include <Server/Connection.h>
#include <Common/Buffer.h>

ConnectionManager::ConnectionManager()
{
	
}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::AddConnection(std::shared_ptr<Connection> connection)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_Connections.push_back(connection);
}

void ConnectionManager::RemoveConnection(std::shared_ptr<Connection> connection)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	if (connection->IsConnected())
		connection->Close();
	m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), connection), m_Connections.end());
}

void ConnectionManager::OnPacketReceived(std::function<void(std::shared_ptr<Connection>, std::shared_ptr<Buffer>)> callback)
{
	if (m_Connections.size() == 0)
		return;
	FD_SET readSet;
	FD_ZERO(&readSet);
	FD_SET writeSet;
	FD_ZERO(&writeSet);
	FD_SET exceptSet;
	FD_ZERO(&exceptSet);
	TIMEVAL timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	for (auto& connection : m_Connections)
	{
		if (connection->IsConnected()) {
			FD_SET(connection->GetSocket(), &readSet);
			FD_SET(connection->GetSocket(), &exceptSet);
			FD_SET(connection->GetSocket(), &writeSet);
		}
		else {
			connection->Close();
		}
	}
	for (auto& connection : m_Connections)
	{
		if (!connection->IsConnected())
		{
			RemoveConnection(connection);
		}
	}
	if (!(readSet.fd_count || writeSet.fd_count || exceptSet.fd_count))
		return;
	int result = select(0, &readSet, &writeSet, &exceptSet, 0);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{
			std::cout << "Error in select: " << error << std::endl;
		}
		return;
	}

	for (auto& connection : m_Connections)
	{
		if (FD_ISSET(connection->GetSocket(), &readSet))
		{
			auto buffer = connection->RecvPacket();
			if (buffer.has_value())
			{
				callback(connection, buffer.value());
			}
		}
		if (FD_ISSET(connection->GetSocket(), &exceptSet))
		{
			connection->Close();
		}
	}
}
