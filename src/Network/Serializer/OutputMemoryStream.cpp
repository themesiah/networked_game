#include "OutputMemoryStream.h"

#include <algorithm>

#include "Endianness.h"

OutputMemoryStream::OutputMemoryStream() : mBuffer(nullptr), mHead(0), mCapacity(0)
{
	ReallocBuffer(32);
}

OutputMemoryStream::~OutputMemoryStream()
{
	std::free(mBuffer);
}

uint32_t OutputMemoryStream::GetLength() const
{
	return mHead;
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, inNewLength));
	//handle realloc failure
	//...
	mCapacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	//make sure we have space...
	uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);
	if (resultHead > mCapacity)
	{
		ReallocBuffer(std::max(mCapacity * 2, resultHead));
	}
	//copy into buffer at head
	std::memcpy(mBuffer + mHead, inData, inByteCount);
	//increment head for next write
	mHead = resultHead;
}

template<typename T> void OutputMemoryStream::Write(T inData)
{
	static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types");
	if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
	{
		Write(&inData, sizeof(inData));
	}
	else
	{
		T swappedData = ByteSwap(inData);
		Write(&swappedData, sizeof(swappedData));
	}
}

void OutputMemoryStream::Write(uint32_t inData)
{
	Write(&inData, sizeof(inData));
}

void OutputMemoryStream::Write(int32_t inData)
{
	Write(&inData, sizeof(inData));
}

const char* OutputMemoryStream::GetBufferPtr() const
{
	return mBuffer;
}