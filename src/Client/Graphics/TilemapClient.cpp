#include "TilemapClient.h"

#include "../Engine/Engine.h"
#include "RenderManager.h"

#include "../Engine/Engine.h"
#include "../Engine/TextureManager.h"

#include "imgui.h"
#include "../Other/ImGUILog.h"

TilemapClient::TilemapClient() :
Tilemap()
, m_loaded(false)
, m_Visible(true)
{

}

TilemapClient::~TilemapClient()
{
	m_vertices.clear();
}

void TilemapClient::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

void TilemapClient::OnAfterSerializeRead()
{
	// load the tileset texture
	mTilesetName = CEngine::GetInstance().GetResourceTable(CResourcesTable::TEXTURES)->GetResourcePath(mTilemapId);
	m_tileset = CEngine::GetInstance().GetTextureManager().LoadTexture(mTilesetName);
	if (m_tileset == nullptr)
	{
		IMLOG_WARNING("Tilemap failed loading. It won't show.");
		return;
	}
	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(mWidth * mHeight * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < mWidth; ++i)
		for (unsigned int j = 0; j < mHeight; ++j)
		{
			// get the current tile number
			int tileNumber = mTiles[i + j * mWidth];
			if (tileNumber > 0)
			{
				tileNumber--;
				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset->getSize().x / mTileWidth);
				int tv = tileNumber / (m_tileset->getSize().x / mTileWidth);

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
		}
	m_loaded = true;
	this->setScale(2.f, 2.f);
	IMLOG_INFO("Tilemap loaded.");
}

void TilemapClient::Update(float aDeltaTime)
{
	if (m_Visible) {
		CEngine::GetInstance().GetRenderManager().Draw(this, mRenderLayer);
	}
}

void TilemapClient::SetName(const std::string& aName)
{
	mTilemapName = aName;
}

void TilemapClient::RenderImGui()
{
	ImGui::PushID(this);
	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "TILEMAP");
	ImGui::Text("Size: %dx%d, Loaded: %s", mWidth, mHeight, m_loaded ? "true" : "false");
	ImGui::Checkbox("Visible", &m_Visible);
	ImGui::PopID();
}
