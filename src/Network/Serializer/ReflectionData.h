#pragma once
#ifndef H_REFLECTION_DATA
#define H_REFLECTION_DATA

#include "../Base/Utils/TemplatedMap.h"
#include "../Base/Utils/Singleton.h"

#include <map>
#include <assert.h>

#define SET_REFLECTION_DATA(C) ReflectionData::GetInstance().SetData<C>(); \
	ObjectCreationRegistry::GetInstance().RegisterCreationFunction<C>();
#define GET_REFLECTION_DATA(id) ReflectionData::GetInstance().GetData(id)
#define GET_REFLECTION_NAME(C) C::kClassId;

class DataType;
class ReflectionData : public base::utils::CSingleton<ReflectionData> {
public:
	template <class C> void SetData() {
		uint32_t id = GET_REFLECTION_NAME(C);
		std::map<uint32_t, DataType*>::iterator lItFind = m_DataMap.find(id);
		if (lItFind != m_DataMap.end())
		{
			delete lItFind->second;
		}
		m_DataMap[id] = C::GetReflectionData();
	}

	DataType* GetData(uint32_t classId) {
		//assert(m_DataMap.find(classId) != m_DataMap.end());
		return m_DataMap[classId];
	}

	ReflectionData() : CSingleton() {
		m_DataMap = std::map<uint32_t, DataType*>();
	}

	~ReflectionData() {
		for (std::map<uint32_t, DataType*>::iterator itb = m_DataMap.begin(), ite = m_DataMap.end(); itb != ite; ++itb)
		{
			delete itb->second;
		}
		m_DataMap.clear();
	}
private:
	std::map<uint32_t, DataType*> m_DataMap;
};

#endif