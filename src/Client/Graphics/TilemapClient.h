#pragma once

#ifndef H_TILEMAP_CLIENT
#define H_TILEMAP_CLIENT

#include <SFML\Graphics.hpp>

#include "CommonClasses\Tilemap.h"

class TilemapClient : public Tilemap, public sf::Drawable, public sf::Transformable
{
public:
	TilemapClient();
	void Update(float aDeltaTime);
	virtual void RenderImGui();
	CLASS_IDENTIFICATION('TLMP', TilemapClient);
protected:
	virtual void OnAfterSerializeRead() override;
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	bool m_loaded;
};

#endif