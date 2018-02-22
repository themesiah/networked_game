#pragma once

#include <vector>
#include <map>

namespace sf {
	class RenderWindow;
	class Drawable;
}

typedef std::vector<sf::Drawable*> DrawablesVector;
typedef std::vector<DrawablesVector> LayerType;

class CRenderManager
{
public:
	CRenderManager();
	virtual ~CRenderManager();
	void Init();
	void Render(sf::RenderWindow* aWindow);
	void Draw(sf::Drawable* aDrawable, unsigned int layer);
private:
	void Resize();
	LayerType m_Layers;
	unsigned int m_LayerCount;
};