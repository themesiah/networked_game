#pragma once
#ifndef H_REFLECTION_DATA
#define H_REFLECTION_DATA

#include "../Base/Utils/TemplatedMap.h"
#include "../Base/Utils/Singleton.h"

#include <map>
#include <assert.h>

#define SET_REFLECTION_DATA(C) ReflectionData::GetInstance().SetData<C>()
#define GET_REFLECTION_DATA(C) ReflectionData::GetInstance().GetData<C>()
#define GET_REFLECTION_NAME(C) typeid(C).name()

class DataType;
class ReflectionData : public base::utils::CSingleton<ReflectionData> {
public:
	template <class C> void SetData() {
		if (m_DataMap.Exist(GET_REFLECTION_NAME(C))) {
			m_DataMap.Update(GET_REFLECTION_NAME(C), C::GetReflectionData());
		}
		else {
			m_DataMap.Add(typeid(C).name(), C::GetReflectionData());
		}
	}

	template <class C> DataType* GetData() {
		assert(m_DataMap.Exist(GET_REFLECTION_NAME(C)));
		return m_DataMap.Get(GET_REFLECTION_NAME(C));
	}

	ReflectionData() : CSingleton() {
		m_DataMap = base::utils::CTemplatedMap<DataType>();
	}

	~ReflectionData() {
		m_DataMap.Destroy();
	}
private:
	base::utils::CTemplatedMap<DataType> m_DataMap;
};

#endif