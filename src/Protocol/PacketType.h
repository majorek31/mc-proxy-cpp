#pragma once

enum class PacketType {
	Handshake = 0,

	StatusRequest = 0,
	StatusResponse = 0,
	LoginRequest = 0,
	LoginResponse = 0,

	PingRequest = 1,
	PingResponse = 1,
};