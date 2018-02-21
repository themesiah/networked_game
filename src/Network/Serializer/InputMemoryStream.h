#pragma once
#include <memory>

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount);
	~InputMemoryStream();
	uint32_t GetRemainingDataSize() const;
	void Read(void* outData, size_t inByteCount);
	template<typename T> void Read(T& outData);
	void Read(uint32_t& outData);
	void Read(int32_t& outData);
private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};