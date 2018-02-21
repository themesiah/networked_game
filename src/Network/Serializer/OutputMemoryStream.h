#pragma once
#include <memory>

class OutputMemoryStream
{
public:
	OutputMemoryStream();
	~OutputMemoryStream();
	//get a pointer to the data in the stream
	const char* GetBufferPtr() const;
	uint32_t GetLength() const;
	void Write(const void* inData, size_t inByteCount);
	template<typename T> void Write(T inData);
	void Write(uint32_t inData);
	void Write(int32_t inData);
private:
	void ReallocBuffer(uint32_t inNewLength);
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};