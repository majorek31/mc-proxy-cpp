#include "Buffer.h"

Buffer::Buffer(size_t size)
{
    m_Buffer = new char[size];
	m_Size = size;
	m_Capacity = size;
	m_CursorPosition = 0;
}

Buffer::Buffer(char* data, size_t size) : Buffer(size)
{
    memcpy(m_Buffer, data, size);
	m_Size = size;
	m_Capacity = size;
	m_CursorPosition = 0;
}

Buffer::Buffer(const Buffer& buffer)
{
    m_Buffer = new char[buffer.m_Size];
	memcpy(m_Buffer, buffer.m_Buffer, buffer.m_Size);
	m_Size = buffer.m_Size;
	m_Capacity = buffer.m_Capacity;
	m_CursorPosition = buffer.m_CursorPosition;
}

Buffer::Buffer(Buffer&& buffer) noexcept
{
    m_Buffer = buffer.m_Buffer;
	m_Size = buffer.m_Size;
	m_Capacity = buffer.m_Capacity;
	m_CursorPosition = buffer.m_CursorPosition;
	buffer.m_Buffer = nullptr;
	buffer.m_Size = 0;
	buffer.m_Capacity = 0;
	buffer.m_CursorPosition = 0;
}

Buffer::~Buffer()
{
    delete[] m_Buffer;
}

int Buffer::GetVarIntSize(int value)
{
	int size = 0;
	do {
		value >>= 7;
		size++;
	} while (value != 0);
	return size;
}

std::shared_ptr<Buffer> Buffer::Split(int start, int length)
{
    if (start + length <= m_Size)
	{
		char* data = m_Buffer + start;
		return std::make_shared<Buffer>(data, length);
	}
}

void Buffer::Clear()
{
    m_CursorPosition = 0;
    m_Size = 0;
	memset(m_Buffer, 0, m_Size);
}

int Buffer::ReadVarInt()
{
    int value = 0, position = 0;
    int start = m_CursorPosition;
    while (true)
    {
        char current = ReadChar();
        value |= (current & 0x7F) << position;

        if ((current & 0x80) == 0) break;

        position += 7;
        if (position >= 32) {
            return -1;
        }
    }
    return value;
}

void Buffer::WriteVarInt(int value)
{
	while (true)
	{
		if ((value & 0xFFFFFF80) == 0)
		{
			WriteChar(value);
			return;
		}

		WriteChar((value & 0x7F) | 0x80);
		value >>= 7;
	}

}

void Buffer::WriteString(std::string value)
{
    WriteVarInt(value.length());
	memcpy(m_Buffer + m_CursorPosition, value.c_str(), value.length());
	m_CursorPosition += value.length();
}

std::string Buffer::ReadString()
{
    int length = ReadVarInt();
    std::string value(m_Buffer + m_CursorPosition, length);
    m_CursorPosition += length;
    return value;
}

void Buffer::WriteChar(char value)
{
	*(char*)(m_Buffer + m_CursorPosition) = value;
	m_CursorPosition += sizeof(char);
}

char Buffer::ReadChar()
{
    char value = *(char*)(m_Buffer + m_CursorPosition);
    m_CursorPosition += sizeof(char);
    return value;
}

void Buffer::WriteUnsignedShort(uint16_t value)
{
    *(uint16_t*)(m_Buffer + m_CursorPosition) = _byteswap_ushort(value);
	m_CursorPosition += sizeof(uint16_t);
}

uint16_t Buffer::ReadUnsignedShort()
{
    uint16_t value = *(uint16_t*)(m_Buffer + m_CursorPosition);
    m_CursorPosition += sizeof(uint16_t);
    return ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
}

void Buffer::WriteInt(int value)
{
    *(int*)(m_Buffer + m_CursorPosition) = value;
	m_CursorPosition += sizeof(int);
}

int Buffer::ReadInt()
{
    int value = *(int*)(m_Buffer + m_CursorPosition);
    m_CursorPosition += sizeof(int);
    return value;
}

void Buffer::WriteLong(long long value)
{
    *(long long*)(m_Buffer + m_CursorPosition) = _byteswap_uint64(value);
    m_CursorPosition += sizeof(long long);
}

long long Buffer::ReadLong()
{
    long long value = *(long long*)(m_Buffer + m_CursorPosition);
    m_CursorPosition += sizeof(long long);
    return _byteswap_uint64(value);
}
