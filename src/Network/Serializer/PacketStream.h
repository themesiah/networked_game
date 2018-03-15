#pragma once

#ifndef H_PACKET_STREAM
#define H_PACKET_STREAM

#include <inttypes.h>
#include <memory>
#include <algorithm>

typedef uint16_t PacketSize;


class PacketStream {
public:
	struct Packet {
		PacketSize size;
		char* buffer;
	};

	PacketStream() : mCapacity(256), mHead(0), mReadHead(0)
	{
		mBuffer = static_cast<char*>(std::realloc(mBuffer, mCapacity));
	}

	void WriteBytes(const void* inData, size_t inByteCount)
	{
		uint32_t nextByteHead = mHead + inByteCount;
		if (nextByteHead > mCapacity)
		{
			ReallocBuffer(std::max<uint32_t>(mCapacity * 2, nextByteHead));
		}
		memcpy(mBuffer + mHead, inData, inByteCount);
		mHead = nextByteHead;
	}

	Packet ReadPacket()
	{
		Packet p = Packet();
		if (mHead - mReadHead > sizeof(PacketSize)) {
			PacketSize lPacketSize;
			memcpy(&lPacketSize, mBuffer, sizeof(PacketSize));
			uint32_t nextReadHead = mReadHead + sizeof(PacketSize);
			if (nextReadHead + lPacketSize > mHead) {
				// Buffer have the next packet length, but don't have the full packet.
				p.size = 0;
			}
			else {
				// Buffer have the full packet. We need to reallocate the buffer and set the readHead
				p.size = lPacketSize;
				p.buffer = static_cast<char*>(std::realloc(p.buffer, lPacketSize));
				memcpy(p.buffer, mBuffer + sizeof(PacketSize), lPacketSize);
				mReadHead = nextReadHead + lPacketSize;
				// Then we set the rest of the  buffer to the beginning of the buffer
				Reset();
			}
		}
		else {
			// Buffer is empty or have less than packet size
			p.size = 0;
		}
		return p;
	}

private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mReadHead;
	uint32_t mCapacity;

	void ReallocBuffer(uint32_t inNewByteCapacity)
	{
		mBuffer = static_cast<char*>(std::realloc(mBuffer, inNewByteCapacity));
		mCapacity = inNewByteCapacity;
	}

	void Reset()
	{
		if (mReadHead > 0) {
			uint32_t lastReadHead = mReadHead;
			mReadHead = 0;
			memcpy(mBuffer, mBuffer + lastReadHead, mHead - lastReadHead);
			mHead -= lastReadHead;
		}
	}
};

#endif