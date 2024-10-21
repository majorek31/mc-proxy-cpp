#pragma once
#include "Protocol/Packets/Packet.h"
#include "StatusResponsePacket.h"

class StatusRequestPacket : public Packet
{
public:
	StatusRequestPacket(std::shared_ptr<Buffer> buffer) : Packet(buffer) {}
	StatusRequestPacket() : Packet(std::make_shared<Buffer>(10)) {}

	void Handle(Server& server, std::shared_ptr<Connection>& connection) override
	{
		if (connection->GetClientState().GetNextState() != HandshakeState::Status) {
			connection->Close();
			return;
		}
		char buf[0x1000] = { 0 };
		sprintf_s(buf, R"(
			{
				"version": {
					"name": "mc-proxy-server",
					"protocol": 0
				},
				"players": {
					"max": 0,
					"online": 0,
					"sample": []
				},
				"description": {
					"text": "",
					"extra": [
						{
							"text": "MC Protect Network \n",
							"color": "red",
							"bold": true
						},
						{
							"text": "%s is not found in database!",
							"color": "yellow",
							"italic": true
						}
					]
				},
				"favicon": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAIAAAAlC+aJAAADc0lEQVR4Xu2VMW4UQRBFOyElQ0KCgGRJEAkih5jIIkJCZETkXIAIiZiIE3AGJ4gTcAQirrE0fLao/b+qt6enx7CSv0owrq6pfs9rQ/nx5eZZV/n11/uzLRMoZ5hrgSvM7vN3lG9eCzTz8tF9Kz5bnqsW8PRTNK5OQLmnaCwTsGktP0bRYaWnPdlaXRVOjgjolpPzoYB/3rmF+jqVTZZZAoSiFQpQ2TY9yiZLj4Bv+j6OlCljDZv+NNzvE/YXCxR3pEyePjuyTnueb02QJgs0Khz+BwL+Dr21UTqs9NsK6B3aaZQOK72e/gWKkEqnQPuOsNOo9rCehkiWyQK+mVV7MlzleXYDAk9u3wjvCJtZ6aR/vX+Vhyz9AqgGgd5EpZOK3rPKQ5alAnCwo0V364y+bnXx4J5u8HiWVMDHC/iPYqOq9FQM5DIosJGGoqOyb39ZKTDRQaFJIHNYKzBFQ4lDgdBhjsCwhrJaffv0xp7vvL289epjLSabK7BIQ4mJfhMB+yiVu99BcVG2HPQmUB8mCNS9dYvdcdIh01BuVF1u+03Aaq0A9vo7BjQU2gqIRunR9/v9BAFw444BDSU2bv89JoH9IXMEbMWAhqL7t4CLbxCuePb0Qy0T8EdMNiBADqSh9Crg57GQvjQB668VAEd932NlDqoRohsuBOwZAtkkky0S0Gp8FDun0UAPK5tkrN9ZJdCpkQFR2c9JOMlMh0wQ6NFQIEJH6RGKgVymCfh/DdWBgOpPuSduCGAtA7lMFiAHaCg9Cbx78biWCthOBnKZL6Aanqn+45gJeA3axkAuXQJ1hRK3BbyGoSNtgYuHd2vNF0Ap95gAHQH96+vntbYVQHnopQLhEdA3FED8RoLuEaAj/1vbEGAIyQKBIh/FycoEKqKHJnoI8N1JlgkgCppVKADEDB3FV+YZEUAUV6tHYBgdGRdAFLpfYCU6ciTQWbRCuXsE1qMjEwQQpTcH/Q2eRV9Cgdq1/3QoDQFEBbRmoSOTBRCFJgF+YUWOBMoBnacO6RQouQPPrU4sYJ9GWLwjz9b0ZWsBZCN0ZJUAhhH6Eh2d8R0dQNP/aTM6jM64gL9AO3jumcme2x3LZAELdcIZ6uszHvyX9FbJBLKcFKDnnpnGMy3RbWWlgA86ttc6J2cWPfugyQLteIH/JEcC51p/BM65fgKysZL3HebXJQAAAABJRU5ErkJggg==",
				"enforcesSecureChat": false
			}
		)", connection->GetClientState().GetServerAddress().c_str());
		connection->SendPacket(StatusResponsePacket(buf));
	}
};

