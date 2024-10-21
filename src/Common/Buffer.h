#pragma once
#include <iostream>

class Buffer
{
public:
	Buffer(size_t size);
	Buffer(char* data, size_t size);
	Buffer(const Buffer& buffer);
	Buffer(Buffer&& buffer) noexcept;
	~Buffer();

	char& operator[] (int index) { return m_Buffer[index]; }

	static int GetVarIntSize(int value);

	int GetCursor() const { return m_CursorPosition; }
	void SetCursor(int position) { m_CursorPosition = position; }

	char* GetBuffer() const { return m_Buffer; }
	int GetSize() const { return m_CursorPosition; }
	int GetCapacity() const { return m_Capacity; }

	std::shared_ptr<Buffer> Split(int start, int length);

	void Clear();

	int ReadVarInt();
	void WriteVarInt(int value);

	void WriteString(std::string value);
	std::string ReadString();

	void WriteChar(char value);
	char ReadChar();

	void WriteUnsignedShort(uint16_t value);
	uint16_t ReadUnsignedShort();

	void WriteInt(int value);
	int ReadInt();

	void WriteLong(long long value);
	long long ReadLong();
private:
	char* m_Buffer;
	int m_Size;
	int m_Capacity;
	int m_CursorPosition;
};

