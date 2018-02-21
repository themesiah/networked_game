#pragma once

#ifndef H_MEMORY_STREAM
#define H_MEMORY_STREAM

#include <type_traits>
#include <map>

class MemoryStream
{
public:
	virtual ~MemoryStream() {};
	virtual bool IsInput() const = 0;
	const char* MemoryStream::GetBufferPtr() const
	{
		return mBuffer;
	}

	uint32_t MemoryStream::GetBitLength() const
	{
		return mBitHead;
	}

	uint32_t MemoryStream::GetByteLength() const
	{
		return (mBitHead + 7) >> 3;
	}
	float ConvertFromFixed(uint16_t inNumber, const float& outMin, const float& precision)
	{
		return static_cast<float>(inNumber)* precision + outMin;
	}
	uint16_t ConvertToFixed(const float& inNumber, const float& inMin, const float& precision)
	{
		return static_cast<uint16_t>((inNumber - inMin) / precision);
	}
	virtual void Serialize(void *ioData, size_t inBitCount) = 0;
	void Serialize(float& ioData, const float& ioMin, const float& precision) {
		if (IsInput() == true) {
			uint16_t outDataFixed;
			Serialize<uint16_t>(outDataFixed, 16);
			ioData = ConvertFromFixed(outDataFixed, ioMin, precision);
		}
		else {
			uint16_t inDataFixed = ConvertToFixed(ioData, ioMin, precision);
			Serialize<uint16_t>(inDataFixed);
		}
	}
	void Serialize(bool& ioData) {
		Serialize<bool>(ioData, 1);
	}
	void Serialize(std::string& ioData) {
		if (IsInput() == true) {
			size_t stringSize;
			Serialize<size_t>(stringSize);
			char* buffer = NULL;
			buffer = static_cast<char*>(std::realloc(buffer, stringSize));
			Serialize<char *>(buffer, (sizeof(char) * stringSize) << 3);
			ioData = std::string(buffer, stringSize);
		}
		else {
			size_t size = ioData.size();
			Serialize<size_t>(size);
			const char * buffer = ioData.c_str();
			Serialize<const char *>(buffer, (sizeof(char) * size) << 3);
		}
	}
	template <typename T> void Serialize(T& ioData, size_t inBitCount = sizeof(T) << 3) {
		Serialize(&ioData, inBitCount);
	}
	template <typename K, typename V> void Serialize(std::map<K,V>& ioMap, size_t keyBitSize = sizeof(K) << 3, size_t valueBitSize = sizeof(V) << 3) {
		if (IsInput() == true) {
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
		else {
			size_t elementCount = ioMap.size();
			Serialize<size_t>(elementCount);
			for (std::map<K, V>::iterator it = ioMap.begin(); it != ioMap.end(); ++it)
			{
				K key = it->first;
				Serialize<K>(key, keyBitSize);
				Serialize<V>(it->second, valueBitSize);
			}
		}
	}
	template <typename T> void Serialize(std::vector<T>& ioVector, size_t elementBitSize = sizeof(T) << 3) {
		if (IsInput() == true)
		{
			size_t elementCount;
			Serialize<size_t>(elementCount);
			ioVector.resize(elementCount);
			for (T& element : ioVector) {
				Serialize<T>(element, elementBitSize);
			}
		}
		else {
			size_t elementCount = ioVector.size();
			Serialize<size_t>(elementCount);
			for (T element : ioVector) {
				Serialize<T>(element, elementBitSize);
			}
		}
	}
	void Serialize(std::vector<bool>& ioVector) {
		if (IsInput() == true) {

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
		else {
			size_t elementCount = ioVector.size();
			Serialize<size_t>(elementCount);
			for (bool element : ioVector) {
				Serialize(element, 1);
			}
		}
	}
protected:
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};
#endif