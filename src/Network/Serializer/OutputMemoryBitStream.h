#pragma once
#ifndef H_OUTPUT_MEMORY_BIT_STREAM
#define H_OUTPUT_MEMORY_BIT_STREAM
#include <memory>
#include <string>
#include <vector>

#include "MemoryStream.h"

class OutputMemoryBitStream : public MemoryStream
{
public:
	OutputMemoryBitStream();
	virtual ~OutputMemoryBitStream();
	void WriteBits(uint8_t inData, size_t inBitCount);
	void WriteBits(const void* inData, size_t inBitCount);
	void WriteBytes(const void* inData, size_t inByteCount);
	virtual bool IsInput() const override { return false; }
	virtual void Serialize(void* inData, size_t inBitCount) override;
	void Reset();
private:
	void ReallocBuffer(uint32_t inNewBitCapacity);
};

#endif