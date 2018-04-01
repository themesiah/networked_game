#pragma once
#ifndef H_OUTPUT_MEMORY_BIT_STREAM
#define H_OUTPUT_MEMORY_BIT_STREAM
#include <memory>
#include <string>
#include <vector>
#include <map>

class OutputMemoryBitStream
{
public:
	OutputMemoryBitStream();
	virtual ~OutputMemoryBitStream();
	void WriteBits(uint8_t inData, size_t inBitCount);
	void WriteBits(const void* inData, size_t inBitCount);
	void WriteBytes(const void* inData, size_t inByteCount);
	bool IsInput() const { return false; }
	void Serialize(void* inData, size_t inBitCount);
	void Reset();
	void Close();

	const char* GetBufferPtr() const;
	uint32_t GetBitLength() const;
	uint32_t GetByteLength() const;

	float ConvertFromFixed(uint16_t inNumber, const float& outMin, const float& precision)
	{
		return static_cast<float>(inNumber)* precision + outMin;
	}

	uint16_t ConvertToFixed(const float& inNumber, const float& inMin, const float& precision)
	{
		return static_cast<uint16_t>((inNumber - inMin) / precision);
	}

	void Serialize(float ioData, const float& ioMin, const float& precision) {
		uint16_t inDataFixed = ConvertToFixed(ioData, ioMin, precision);
		Serialize<uint16_t>(inDataFixed);
	}

	void Serialize(bool ioData) {
		Serialize<bool>(ioData, 1);
	}

	void Serialize(std::string ioData) {
		size_t size = ioData.size();
		Serialize<size_t>(size);
		const char * buffer = ioData.c_str();
		Serialize<const char *>(buffer, (sizeof(char) * size) << 3);
	}

	template <typename T> void Serialize(T ioData, size_t inBitCount = sizeof(T) << 3) {
		WriteBits(&ioData, inBitCount);
	}

	template <typename K, typename V> void Serialize(std::map<K, V>& ioMap, size_t keyBitSize = sizeof(K) << 3, size_t valueBitSize = sizeof(V) << 3) {
		size_t elementCount = ioMap.size();
		Serialize<size_t>(elementCount);
		for (std::map<K, V>::iterator it = ioMap.begin(); it != ioMap.end(); ++it)
		{
			K key = it->first;
			Serialize<K>(key, keyBitSize);
			Serialize<V>(it->second, valueBitSize);
		}
	}

	template <typename T> void Serialize(std::vector<T>& ioVector, size_t elementBitSize = sizeof(T) << 3) {
		size_t elementCount = ioVector.size();
		Serialize<size_t>(elementCount);
		for (T element : ioVector) {
			Serialize<T>(element, elementBitSize);
		}
	}

	void Serialize(std::vector<bool>& ioVector) {
		size_t elementCount = ioVector.size();
		Serialize<size_t>(elementCount);
		for (bool element : ioVector) {
			Serialize(element, 1);
		}
	}
private:
	void WriteSize();
	void ReallocBuffer(uint32_t inNewBitCapacity);
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
	bool mClosed;
};

#endif