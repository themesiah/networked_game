#pragma once

#ifndef H_TILEMAP_CLIENT
#define H_TILEMAP_CLIENT

#include <SFML\Graphics.hpp>

#include "CommonClasses\Tilemap.h"

class TilemapClient : public Tilemap, public sf::Drawable, public sf::Transformable
{
public:
	TilemapClient();
	virtual ~TilemapClient();
	void Update(float aDeltaTime);
	virtual void RenderImGui();
	void SetName(const std::string& aName);
	CLASS_IDENTIFICATION('TLMP', TilemapClient);
protected:
	virtual void OnAfterSerializeRead() override;
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture* m_tileset;
	std::string mTilesetName;
	bool m_loaded;
	bool m_Visible;
	std::string mTilemapName;
};

#endif