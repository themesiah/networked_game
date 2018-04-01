#pragma once
#ifndef H_INPUT_MEMORY_BIT_STREAM
#define H_INPUT_MEMORY_BIT_STREAM
#include <memory>
#include <string>
#include <vector>
#include <map>

class InputMemoryBitStream
{
public:
	InputMemoryBitStream(const char* inBuffer, uint32_t inByteCount);
	virtual ~InputMemoryBitStream();
	uint32_t GetRemainingDataSize() const;
	void ReadBits(uint8_t& outData, size_t inBitCount);
	void ReadBits(void* outData, size_t inBitCount);
	void Read(void* outData, size_t inBitCount);
	bool IsInput() const { return true; }
	void Serialize(void* outData, size_t inBitCount);
	void Reset(const char* inBuffer, uint32_t inBitCount);

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

	void Serialize(float& ioData, const float& ioMin, const float& precision) {
		uint16_t outDataFixed;
		Serialize<uint16_t>(outDataFixed, 16);
		ioData = ConvertFromFixed(outDataFixed, ioMin, precision);
	}
	void Serialize(bool& ioData) {
		Serialize<bool>(ioData, 1);
	}
	void Serialize(std::string& ioData) {
		size_t stringSize;
		Serialize<size_t>(stringSize);
		char* buffer = NULL;
		buffer = static_cast<char*>(std::realloc(buffer, stringSize));
		Serialize<char *>(buffer, (sizeof(char) * stringSize) << 3);
		ioData = std::string(buffer, stringSize);
	}
	template <typename T> void Serialize(T& ioData, size_t inBitCount = sizeof(T) << 3) {
		Serialize(&ioData, inBitCount);
	}
	template <typename K, typename V> void Serialize(std::map<K, V>& ioMap, size_t keyBitSize = sizeof(K) << 3, size_t valueBitSize = sizeof(V) << 3) {
		ioMap.clear();
		size_t elementCount;
		Serialize<size_t>(elementCount);
		for (size_t i = 0; i < elementCount; ++i) {
			K key;
			V value;
			Serialize<K>(key, keyBitSize);
			Serialize<V>(value, valueBitSize);
			ioMap[key] = value;
		}
	}
	template <typename T> void Serialize(std::vector<T>& ioVector, size_t elementBitSize = sizeof(T) << 3) {
		size_t elementCount;
		Serialize<size_t>(elementCount);
		ioVector.resize(elementCount);
		for (T& element : ioVector) {
			Serialize<T>(element, elementBitSize);
		}
	}
	void Serialize(std::vector<bool>& ioVector) {
		size_t elementCount;
		Serialize<size_t>(elementCount);
		ioVector.resize(elementCount);
		for (size_t i = 0; i < elementCount; ++i)
		{
			bool element;
			Serialize(element, 1);
			ioVector[i] = element;
		}
	}
private:
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};
#endif