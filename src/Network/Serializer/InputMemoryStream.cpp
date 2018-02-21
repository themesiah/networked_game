#include "InputMemoryStream.h"

InputMemoryStream::InputMemoryStream(char* inBuffer, uint32_t inByteCount) : mCapacity(inByteCount), mHead(0), mBuffer(inBuffer)
{
}

InputMemoryStream::~InputMemoryStream()
{
	std::free(mBuffer);
}

uint32_t InputMemoryStream::GetRemainingDataSize() const
{
	return mCapacity - mHead;
}

void InputMemoryStream::Read(void* outData, size_t inByteCount)
{
	if (GetRemainingDataSize() >= inByteCount)
	{
		outData = std::memcpy(mBuffer + mHead, outData, inByteCount);
		mHead += static_cast<uint32_t>(inByteCount);
	}
	else {
		static_assert(true, "Stream has not that much data");
	}
}

template<typename T> void InputMemoryStream::Read(T& outData)
{
	Read(&outData, sizeof(outData));
}

void InputMemoryStream::Read(uint32_t& outData)
{
	Read(&outData, sizeof(outData));
}

void InputMemoryStream::Read(int32_t& outData) {
	Read(&outData, sizeof(outData));
}