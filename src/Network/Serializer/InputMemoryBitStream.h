#pragma once
#ifndef H_INPUT_MEMORY_BIT_STREAM
#define H_INPUT_MEMORY_BIT_STREAM
#include <memory>
#include <string>
#include <vector>

#include "MemoryStream.h"

class InputMemoryBitStream : public MemoryStream
{
public:
	InputMemoryBitStream(const char* inBuffer, uint32_t inByteCount);
	virtual ~InputMemoryBitStream();
	uint32_t GetRemainingDataSize() const;
	void ReadBits(uint8_t& outData, size_t inBitCount);
	void ReadBits(void* outData, size_t inBitCount);
	void Read(void* outData, size_t inBitCount);
	virtual bool IsInput() const override { return true; }
	virtual void Serialize(void* outData, size_t inBitCount) override;
};
#endif