#pragma once

#ifndef H_GAME_OBJECT
#define H_GAME_OBJECT

#include <inttypes.h>

#include "../Serializer/SerializeUtils.h"

#define CLASS_IDENTIFICATION(inCode, inClass) \
enum{kClassId = inCode}; \
virtual uint32_t GetClassId() const {return kClassId;} \
static GameObject* CreateInstance() {return new inClass();}

class GameObject {
public:
	virtual ~GameObject()
	{

	}
	CLASS_IDENTIFICATION('GOBJ', GameObject);

	void SerializeRead(InputMemoryBitStream& ms) {
		OnBeforeSerializeRead();
		SerializeUtils::SerializeRead(ms, (uint8_t*)this, GetClassId());
		OnAfterSerializeRead();
	}

	void SerializeWrite(OutputMemoryBitStream& ms) {
		OnBeforeSerializeWrite();
		SerializeUtils::SerializeWrite(ms, (uint8_t*)this, GetClassId());
		OnAfterSerializeWrite();
	}

	virtual void Destroy() {}
	virtual void Update(float aDeltaTime) {}
	virtual void RenderImGui() {}
protected:
	virtual void OnBeforeSerializeRead() {}
	virtual void OnAfterSerializeRead() {}
	virtual void OnBeforeSerializeWrite() {}
	virtual void OnAfterSerializeWrite() {}
};

#endif