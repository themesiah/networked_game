#ifndef _BASE_TEMPLATEDMAPPTR_MPR_2010201620428_H
#define _BASE_TEMPLATEDMAPPTR_MPR_2010201620428_H

#include <map>
#include "CheckedDelete.h"

namespace base
{
	namespace utils
	{

		template<class K, class V>
		class CTemplatedMapPtr
		{
		public:
			typedef std::map<K, V*> TMapResources;

		public:
			CTemplatedMapPtr(){}
			virtual ~CTemplatedMapPtr()
			{
				Destroy();
			}

			bool Exist(const K &aName) const
			{
				return m_ResourcesMap.find(aName) != m_ResourcesMap.end();
			}

			void Remove(const K &aName)
			{
				m_ResourcesMap.erase(aName);
			}

			virtual V * operator()(const K &aName)
			{
				return m_ResourcesMap[aName];
			}

			virtual V * Get(const K &aName)
			{
				return m_ResourcesMap[aName];
			}

			virtual bool Add(const K &aName, V *Resource)
			{
				bool lOk = false;
				if (!Exist(aName))
				{
					m_ResourcesMap.insert(std::make_pair(aName, Resource));
					lOk = true;
				}
				return lOk;
			}

			virtual void Update(const K &aName, V *aResource)
			{
				TMapResources::iterator lItFind = m_ResourcesMap.find(aName);
				if (lItFind != m_ResourcesMap.end())
				{
					base::utils::CheckedDelete(lItFind->second);
					//lItFind->second = aResource;
				}
				m_ResourcesMap[aName] = aResource;
			}

			virtual void Clear()
			{
				m_ResourcesMap.clear();
			}

			virtual void Destroy()
			{
				for (TMapResources::iterator itb = m_ResourcesMap.begin(), ite = m_ResourcesMap.end(); itb != ite; ++itb)
					base::utils::CheckedDelete(itb->second);

				Clear();
			}

			TMapResources & GetResourcesMap()
			{
				return m_ResourcesMap;
			}

		protected:
			TMapResources	 m_ResourcesMap;
		};
	}
}

#endif	// _BASE_TEMPLATEDMAP_MPR_2010201620428_H