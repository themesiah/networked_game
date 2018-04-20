#pragma once

#ifndef H_GAME_OBJECT
#define H_GAME_OBJECT

#include <inttypes.h>

#include "../Serializer/SerializeUtils.h"

#define CLASS_IDENTIFICATION(inCode, inClass) \
enum{kClassId = inCode}; \
virtual uint32_t GetClassId() const {return kClassId;} \
static GameObject* CreateInstance() { return new inClass();}

class GameObject {
public:
	enum DirtyType {
		NO_DIRTY = 0,
		UPDATE,
		CREATE,
		DESTROY,
		PREPARED_TO_DESTROY
	};
	GameObject() :
		mDirty(DirtyType::CREATE) // It begins as dirty
		, mInitialized(false)
	{

	}
	virtual ~GameObject()
	{

	}
	CLASS_IDENTIFICATION('GOBJ', GameObject);

	virtual void SerializeRead(InputMemoryBitStream& ms) {
		OnBeforeSerializeRead();
		SerializeUtils::SerializeRead(ms, (uint8_t*)this, GetClassId());
		if (mInitialized == false)
		{
			Init();
			mInitialized = true;
		}
		OnAfterSerializeRead();
	}

	virtual void SerializeWrite(OutputMemoryBitStream& ms) {
		OnBeforeSerializeWrite();
		SerializeUtils::SerializeWrite(ms, (uint8_t*)this, GetClassId());
		OnAfterSerializeWrite();
	}

	virtual void Destroy() {
		mDirty = DirtyType::PREPARED_TO_DESTROY;
	}
	virtual void DestroySignal() {
		mDirty = DirtyType::DESTROY;
	}
	virtual void Init() {}
	virtual void Update(float aDeltaTime) {}
	virtual void RenderImGui() {}
	void SetDirty() {
		if (mDirty == DirtyType::NO_DIRTY)
			mDirty = DirtyType::UPDATE;
	}
	DirtyType GetDirty() const {
		return mDirty;
	}
	void Undirty() {
		mDirty = DirtyType::NO_DIRTY;
	}
	void PrepareToDestroy() {
		if (mDirty == DirtyType::DESTROY)
			mDirty = DirtyType::PREPARED_TO_DESTROY;
	}
protected:
	virtual void OnBeforeSerializeRead() {}
	virtual void OnAfterSerializeRead() {}
	virtual void OnBeforeSerializeWrite() {}
	virtual void OnAfterSerializeWrite() {}
private:
	DirtyType mDirty; // 0 not dirty, 1 updated, 2 created, 3 deleted, 4 prepared to delete
	bool mInitialized;
};

#endif