#pragma once

#ifndef H_OBJECT_CREATION_REGISTRY
#define H_OBJECT_CREATION_REGISTRY

#include <unordered_map>
#include <assert.h>

#include "GameObject.h"
#include "Utils\Singleton.h"

typedef GameObject* (*GameObjectCreationFunc)();

class ObjectCreationRegistry : public base::utils::CSingleton<ObjectCreationRegistry>
{
public:
	~ObjectCreationRegistry(){}
	template <class T>
	void RegisterCreationFunction()
	{
		assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId) == mNameToGameObjectCreationFunctionMap.end());
		mNameToGameObjectCreationFunctionMap[T::kClassId] =	T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t inClassId)
	{
		assert(mNameToGameObjectCreationFunctionMap.find(inClassId) != mNameToGameObjectCreationFunctionMap.end());
		GameObjectCreationFunc creationFunc = mNameToGameObjectCreationFunctionMap[inClassId];
		GameObject* gameObject = creationFunc();
		return gameObject;
	}

protected:
	ObjectCreationRegistry(){}
	friend class base::utils::CSingleton<ObjectCreationRegistry>;

private:
	std::unordered_map<uint32_t, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap;
};

#endif