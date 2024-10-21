#include "Server.h"
#include "Connection.h"
#include "Protocol/PacketType.h"
#include <Protocol/PacketManager.h>
#include <future>

Server::Server(int port)
	: m_ThreadPool(std::thread::hardware_concurrency())
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		std::cout << "Error creating socket: " << WSAGetLastError() << std::endl;
	}

	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(port);
	m_Addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(m_Socket, (sockaddr*)&m_Addr, sizeof(m_Addr)) == SOCKET_ERROR)
	{
		std::cout << "Error binding socket: " << WSAGetLastError() << std::endl;
	}

	if (listen(m_Socket, 1024) == SOCKET_ERROR)
	{
		std::cout << "Error listening on socket: " << WSAGetLastError() << std::endl;
	}

	u_long nonBlocking = 1;
	ioctlsocket(m_Socket, FIONBIO, &nonBlocking);


	std::cout << "Server started on port " << port << std::endl;

}

Server::~Server()
{
	closesocket(m_Socket);
	WSACleanup();
}


void Server::Accept()
{
	std::shared_ptr<Connection> connection;
	sockaddr_in clientAddr;
	SOCKET clientSocket;
	int clientAddrSize = sizeof(clientAddr);

	clientSocket = accept(m_Socket, (sockaddr*)&clientAddr, &clientAddrSize);

	if (clientSocket == INVALID_SOCKET) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			std::cout << "Error accepting connection: " << error << std::endl;
		}
		return;
	}

	connection = std::make_shared<Connection>(clientSocket, clientAddr);
	if (connection.get()) {
		GetConnectionManager().AddConnection(connection);
	}
}
void Server::Tick()
{
	std::vector<std::future<void>> futures;

	GetConnectionManager().OnPacketReceived([this](std::shared_ptr<Connection> connection, std::shared_ptr<Buffer> buffer) {
		m_ThreadPool.AddTask([this, connection, buffer = std::move(buffer)]() mutable {
			PacketManager::HandlePacket(*this, connection, buffer);
		});
	});



/*	for (auto connection : m_Connections)
	{
		//std::cout << connection->GetIP() << std::endl;
		futures.push_back(m_ThreadPool.AddTask([this, connection] {

			if (!connection->IsConnected()) {
				connection->Close();
				return;
			}
			if (!connection)
				return;
			auto data = connection->RecvPacket();
			if (data.has_value()) {
				PacketManager::HandlePacket(*this, connection, std::move(data.value()));
			}
		}));
	}

	for (auto& future : futures)
	{
		try
		{
			future.get();
		}
		catch (const std::exception& e)
		{
		}
	}




	//std::cout << "Number of clients: " << m_Connections.size() << std::endl; */
}