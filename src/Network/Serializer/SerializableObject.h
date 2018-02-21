#pragma once

#ifndef H_SERIALIZABLE_OBJECT
#define H_SERIALIZABLE_OBJECT

#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "SerializeUtils.h"

template<class C> class SerializableObject {
public:
	MemoryStream* Serialize() {
		MemoryStream *ms = new OutputMemoryBitStream();
		SerializeUtils::Serialize<C>(ms, (uint8_t*)this);
		return ms;
	}

	void Unserialize(MemoryStream* ms) {
		assert(ms->IsInput());
		SerializeUtils::Serialize<C>(ms, (uint8_t*)this);
	}
};

#endif