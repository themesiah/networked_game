#ifndef __H_TEMPLATED_PAIR_MAP_VECTOR__
#define __H_TEMPLATED_PAIR_MAP_VECTOR__

#include <string>
#include <map>
#include <assert.h>

#include "CheckedDelete.h"

namespace base{
	namespace utils{
		template<class K, class T>
		class CTemplatedPairMapVector
		{
		public:
			class CMapResourceValue
			{
			public:
				T *m_Value;
				size_t m_Id;
				CMapResourceValue(){ assert(!"Method must not be called"); }
				CMapResourceValue(T *aValue, size_t aId) : m_Value(aValue), m_Id(aId){}
			};

			typedef std::vector<T *> TVectorResources;
			typedef std::vector<K> TKeyResources;
			typedef std::map<K, CMapResourceValue> TMapResources;

		public:
			CTemplatedPairMapVector(){}
			virtual ~CTemplatedPairMapVector()
			{
				Destroy();
			}

			size_t GetCount()
			{
				return m_ResourcesVector.size();
			}

			bool Exist(K aKey)
			{
				return m_ResourcesMap.find(aKey) != m_ResourcesMap.end();
			}

			void Remove(K aKey)
			{
				CMapResourceValue l_ResourceValue = m_ResourcesMap[aKey];
				size_t index = l_ResourceValue.m_Id;
				CheckedDelete(m_ResourcesVector[index]);
				CheckedDelete(m_ResourcesMap[aKey]);
				CheckedDelete(m_KeysVector[index]);
				m_ResourcesMap.erase(aKey);
				m_ResourcesVector.erase(m_ResourcesVector.begin() + index);
				m_KeysVector.erase(m_KeysVector.begin() + index);
				for (TMapResources::iterator lItb = m_ResourcesMap.begin(), lIte = m_ResourcesMap.end(); lItb != lIte; ++lItb)
				{
					if (lItb->second->m_Id > index)
					{
						lItb->second->m_Id--;
					}
				}
			}

			virtual T * operator[](size_t aId)
			{
				return m_ResourcesVector[aId];
			}

			virtual T * GetIndex(size_t aId)
			{
				return m_ResourcesVector[aId];
			}

			virtual K GetKey(size_t aId)
			{
				return m_KeysVector[aId];
			}

			virtual T * operator()(K aKey)
			{
				TMapResources::const_iterator lItfind = m_ResourcesMap.find(aKey);
				return (lItfind != m_ResourcesMap.end()) ? lItfind->second.m_Value : nullptr;
			}

			virtual T * Get(K aKey)
			{
				TMapResources::const_iterator lItfind = m_ResourcesMap.find(aKey);
				return (lItfind != m_ResourcesMap.end()) ? lItfind->second.m_Value : nullptr;
			}

			virtual bool Add(K aKey, T *Resource)
			{
				bool lOk = false;
				if (m_ResourcesMap.find(aKey) == m_ResourcesMap.end())
				{
					m_ResourcesMap.insert(std::pair<std::string, CMapResourceValue>(aKey, CMapResourceValue(Resource, m_ResourcesVector.size())));
					m_ResourcesVector.push_back(Resource);
					m_KeysVector.push_back(aKey);
					lOk = true;
				}
				return lOk;
			}

			virtual void Clear()
			{
				m_ResourcesMap.clear();
				m_ResourcesVector.clear();
				m_KeysVector.clear();
			}

			virtual void Destroy()
			{
				CheckedDelete(m_ResourcesVector);
				Clear();
			}

		protected:
			TVectorResources m_ResourcesVector;
			TMapResources	 m_ResourcesMap;
			TKeyResources	 m_KeysVector;
		};
	}
}

#endif // __H_TEMPLATED_KEY_MAP_VECTOR__