#pragma once
#include <iostream>

enum class HandshakeState : uint8_t
{
	Status = 1,
	Login = 2,
	Transfer = 3
};