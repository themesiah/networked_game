#pragma once

#ifndef H_SERIALIZABLE_OBJECT
#define H_SERIALIZABLE_OBJECT

#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "SerializeUtils.h"

class SerializableObject {
public:
	void Serialize(MemoryStream* ms) {
		SerializeUtils::Serialize(ms, (uint8_t*)this, GetClassId());
	}

	virtual uint32_t GetClassId() = 0;
};

#endif