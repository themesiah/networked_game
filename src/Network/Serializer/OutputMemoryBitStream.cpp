#include "OutputMemoryBitStream.h"

#include <algorithm>

#include "PacketStream.h"
#include "Debug\CustomAssert.h"

OutputMemoryBitStream::OutputMemoryBitStream() :
mClosed(false)
, mBitHead(0)
{
	ReallocBuffer(256);
}

OutputMemoryBitStream::~OutputMemoryBitStream()
{
	std::free(mBuffer);
}

const char* OutputMemoryBitStream::GetBufferPtr() const
{
	Assert(mClosed == true, "Cant use a buffer of a non closed stream");
	return mBuffer;
}

uint32_t OutputMemoryBitStream::GetBitLength() const
{
	return mBitHead;
}

uint32_t OutputMemoryBitStream::GetByteLength() const
{
	return (mBitHead + 7) >> 3;
}

void OutputMemoryBitStream::Reset()
{
	//std::free(mBuffer);
	ReallocBuffer(256);
	mBitHead = 0;
}

void OutputMemoryBitStream::Close()
{
	Assert(mClosed == false, "Cant close a closed stream");
	WriteSize();
	mClosed = true;
}

void OutputMemoryBitStream::WriteSize()
{
	PacketSize size = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;
	if (bitOffset > 0)
	{
		size++;
	}
	uint32_t nextBitHead = mBitHead + sizeof(PacketSize)*8;
	if (nextBitHead > mBitCapacity)
	{
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}
	memcpy(mBuffer + sizeof(PacketSize), mBuffer, size);
	memcpy(mBuffer, &size, sizeof(PacketSize));
	mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount)
{
	Assert(mClosed == false, "Cant write in a closed stream");
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
	if (nextBitHead > mBitCapacity)
	{
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}
	//calculate the byteOffset into our buffer
	//by dividing the head by 8
	//and the bitOffset by taking the last 3 bits
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;
	//calculate which bits of the current byte to preserve
	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);
	//calculate how many bits were not yet used in
	//our target byte in the buffer
	uint32_t bitsFreeThisByte = 8 - bitOffset;
	//if we needed more than that, carry to the next byte
	if (bitsFreeThisByte < inBitCount)
	{
		//we need another byte
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}
	mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);
	//write all the bytes
	while (inBitCount > 8)
	{
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}
	//write anything left
	if (inBitCount > 0)
	{
		WriteBits(*srcByte, inBitCount);
	}
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitCapacity)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, inNewBitCapacity));
	//handle realloc failure
	//...
	mBitCapacity = inNewBitCapacity;
}

void OutputMemoryBitStream::WriteBytes(const void* inData, size_t inByteCount)
{
	WriteBits(inData, inByteCount << 3);
}

void OutputMemoryBitStream::Serialize(void* inData, size_t inBitCount) {
	WriteBits(inData, inBitCount);
}
