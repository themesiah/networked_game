#include "InputMemoryBitStream.h"

#include "OutputMemoryBitStream.h"

InputMemoryBitStream::InputMemoryBitStream(const char* inBuffer, uint32_t inBitCount)
{
	mBitCapacity = inBitCount;
	mBitHead = 0;
	mBuffer = const_cast<char*>(inBuffer);
}

InputMemoryBitStream::~InputMemoryBitStream()
{
	std::free(mBuffer);
}

void InputMemoryBitStream::Reset(const char* inBuffer, uint32_t inBitCount) {
	//std::free(mBuffer);
	mBitCapacity = inBitCount*8;
	mBitHead = 0;
	mBuffer = const_cast<char*>(inBuffer);
}

uint32_t InputMemoryBitStream::GetRemainingDataSize() const
{
	return mBitCapacity - mBitHead;
}

void InputMemoryBitStream::ReadBits(uint8_t& outData, size_t inBitCount)
{
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	outData = static_cast< uint8_t >(mBuffer[byteOffset]) >> bitOffset;

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount)
	{
		//we need another byte
		outData |= static_cast< uint8_t >(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
	}

	//don't forget a mask so that we only read the bit we wanted...
	outData &= (~(0x00ff << inBitCount));

	mBitHead += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, size_t inBitCount)
{
	uint8_t* destByte = reinterpret_cast< uint8_t* >(outData);
	//write all the bytes
	while (inBitCount > 8)
	{
		ReadBits(*destByte, 8);
		++destByte;
		inBitCount -= 8;
	}
	//write anything left
	if (inBitCount > 0)
	{
		ReadBits(*destByte, inBitCount);
	}
}

void InputMemoryBitStream::Read(void* outData, size_t inBitCount)
{
	ReadBits(outData, inBitCount);
}

void InputMemoryBitStream::Serialize(void* inData, size_t inBitCount) {
	ReadBits(inData, inBitCount);
}