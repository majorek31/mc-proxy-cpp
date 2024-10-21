#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <mutex>

class Connection;
class Buffer;

class ConnectionManager
{
public:
	ConnectionManager();
	~ConnectionManager();

	void AddConnection(std::shared_ptr<Connection> connection);
	void RemoveConnection(std::shared_ptr<Connection> connection);

	void OnPacketReceived(std::function<void(std::shared_ptr<Connection>, std::shared_ptr<Buffer>)> callback);
private:
	std::mutex m_Mutex;
	std::vector<std::shared_ptr<Connection>> m_Connections;
};

