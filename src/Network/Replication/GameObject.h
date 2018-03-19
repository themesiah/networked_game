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

	void Serialize(MemoryStream* ms) {
		OnBeforeSerialize();
		SerializeUtils::Serialize(ms, (uint8_t*)this, GetClassId());
		OnAfterSerialize();
	}

	virtual void Destroy() {}
	virtual void Update(float aDeltaTime) {}
	virtual void RenderImGui() {}
protected:
	virtual void OnBeforeSerialize() {}
	virtual void OnAfterSerialize() {}
};

#endif