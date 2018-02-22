#include "RenderManager.h"

#include <SFML\Graphics.hpp>

CRenderManager::CRenderManager()
{

}

CRenderManager::~CRenderManager()
{
	for (size_t i = 0; i < m_Layers.size(); ++i)
	{
		m_Layers[i].clear();
	}
	m_Layers.clear();
}

void CRenderManager::Init()
{
	m_LayerCount = 0;
}

void CRenderManager::Render(sf::RenderWindow* aWindow)
{
	for (size_t i = 0; i < m_Layers.size(); ++i)
	{
		for (size_t j = 0; j < m_Layers[i].size(); ++j)
		{
			aWindow->draw(*m_Layers[i][j]);
		}
		m_Layers[i].clear();
	}
}

void CRenderManager::Draw(sf::Drawable* aDrawable, unsigned int layer)
{
	if (layer > m_LayerCount)
	{
		m_LayerCount = layer;
		Resize();
	}
	m_Layers[layer].push_back(aDrawable);
}

void CRenderManager::Resize()
{
	m_Layers.resize(m_LayerCount+1);
}