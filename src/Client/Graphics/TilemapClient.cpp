#include "TilemapClient.h"

#include "../Engine/Engine.h"
#include "RenderManager.h"

#include "imgui.h"

TilemapClient::TilemapClient() :
Tilemap()
, m_loaded(false)
{

}

void TilemapClient::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

void TilemapClient::OnAfterSerializeRead()
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(mTilesetName))
		return;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(mWidth * mHeight * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < mWidth; ++i)
		for (unsigned int j = 0; j < mHeight; ++j)
		{
			// get the current tile number
			int tileNumber = mTiles[i + j * mWidth];

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / mTileWidth);
			int tv = tileNumber / (m_tileset.getSize().x / mTileWidth);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * mWidth) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * mTileWidth, j * mTileHeight);
			quad[1].position = sf::Vector2f((i + 1) * mTileWidth, j * mTileHeight);
			quad[2].position = sf::Vector2f((i + 1) * mTileWidth, (j + 1) * mTileHeight);
			quad[3].position = sf::Vector2f(i * mTileWidth, (j + 1) * mTileHeight);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * mTileWidth, tv * mTileHeight);
			quad[1].texCoords = sf::Vector2f((tu + 1) * mTileWidth, tv * mTileHeight);
			quad[2].texCoords = sf::Vector2f((tu + 1) * mTileWidth, (tv + 1) * mTileHeight);
			quad[3].texCoords = sf::Vector2f(tu * mTileWidth, (tv + 1) * mTileHeight);
		}
	m_loaded = true;
	this->setScale(2.f, 2.f);
}

void TilemapClient::Update(float aDeltaTime)
{
	CEngine::GetInstance().GetRenderManager().Draw(this, 2);
}

void TilemapClient::RenderImGui()
{
	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "TILEMAP");
	ImGui::Text("Size: %dx%d, Loaded: %s", mWidth, mHeight, m_loaded ? "true" : "false");
}