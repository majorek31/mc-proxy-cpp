#include "Server/Server.h"

int main(int argc, const char* argv[]) {
	Server server(25575);
	while (true) {

		server.Accept();
		server.Tick();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}