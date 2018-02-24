#pragma once

#ifndef H_RESOURCES_MANAGER
#define H_RESOURCES_MANAGER

#include <SFML/Graphics/Sprite.hpp>

#include "Utils\TemplatedMapVector.h"

enum EDestructorOptions {
	DoNothing = 0,
	ClearOnDestructor,
	DestroyOnDestructor
};

template <typename T>
class CResourcesManager {
public:
	CResourcesManager()
	{
		m_Resources = base::utils::CTemplatedMapVector<T>();
		m_OnDestructor = 0;
	}

	~CResourcesManager()
	{
		switch (m_OnDestructor)
		{
		case EDestructorOptions::DoNothing:
			break;
		case EDestructorOptions::ClearOnDestructor:
			Clear();
			break;
		case EDestructorOptions::DestroyOnDestructor:
			Destroy();
			break;
		}
	}

	void SetOnDestructor(int option)
	{
		m_OnDestructor = option;
	}

	T* Get(std::string aResourceName)
	{
		if (m_Resources.Exist(aResourceName)) {
			return m_Resources.Get(aResourceName);
		}
		return nullptr;
	}

	void Update(std::string aResourceName, T* aResource, bool deleteOverriden = false)
	{
		if (m_Resources.Exist(aResourceName))
		{
			if (deleteOverriden)
			{
				delete m_Resources.Get(aResourceName);
			}
			m_Resources.Add(aResourceName, aResource);
		}
		else {
			m_Resources.Add(aResourceName, aResource);
		}
	}

	void Clear()
	{
		m_Resources.Clear();
	}

	void Destroy()
	{
		m_Resources.Destroy();
	}

	size_t GetCount()
	{
		return m_Resources.GetCount();
	}

	template <typename F>
	void ForEach(F func)
	{
		for (size_t i = 0; i < m_Resources.GetCount(); ++i)
		{
			func(m_Resources.GetIndex(i));
		}
	}

protected:
	base::utils::CTemplatedMapVector<T> m_Resources;
private:
	int m_OnDestructor;
};

typedef CResourcesManager<sf::Sprite> CSpriteManager;

#endif