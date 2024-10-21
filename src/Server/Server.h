#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include "Connection.h"
#include <thread>
#include <chrono>
#include <mutex>
#include "ThreadPool.h"
#include "ConnectionManager/ConnectionManager.h"

class Server
{
public:
	Server(int port);
	~Server();
	SOCKET GetSocket() const { return m_Socket; }
	void Accept();
	void Tick();
	ConnectionManager& GetConnectionManager() { return m_ConnectionManager; }
private:
	ThreadPool m_ThreadPool;
	SOCKET m_Socket;
	sockaddr_in m_Addr;
	std::mutex m_Mutex;

	std::vector<std::thread> m_AcceptThreads;
	ConnectionManager m_ConnectionManager;
};

