#include "PlayerControllerServer.h"

#include "../Scenario/CityMap.h"

void CPlayerControllerServer::Move(float x, float y, const float& dt)
{
	if (x != 0.f || y != 0.f)
	{
		m_PosX += x * PLAYER_SPEED * dt;
		m_PosY += y * PLAYER_SPEED * dt;
		SetDirty();
	}
	else {
		m_CurrentTile = m_NextTile;
	}
}

void CPlayerControllerServer::SetAnimationId(const uint16_t& aAnimationId)
{
	m_AnimationId = aAnimationId;
}

void CPlayerControllerServer::Update(float dt)
{
	if (m_CurrentTile != m_NextTile)
	{
		CPlayerControllerServer::CoordinatePair currentCoord = IndexToMap(m_CurrentTile);
		CPlayerControllerServer::CoordinatePair nextCoord = IndexToMap(m_NextTile);
		CPlayerControllerServer::CoordinatePair resultCoord;
		resultCoord.x = nextCoord.x - currentCoord.x;
		resultCoord.y = nextCoord.y - currentCoord.y;
		float xDif = resultCoord.x * m_CityMap->GetTileWidth();
		float yDif = resultCoord.y * m_CityMap->GetTileHeight();
		if (xDif == 0.f && yDif == 0.f)
		{
			m_CurrentTile = m_NextTile;
		}
		else
		{
			CPlayerControllerServer::PositionPair targetPosition = MapToPosition(nextCoord.x, nextCoord.y);
			float xDifReal = targetPosition.x - m_PosX;
			float yDifReal = targetPosition.y - m_PosY;
			if (abs(xDifReal) - PLAYER_SPEED * dt <= 0.f)
			{
				m_PosX = targetPosition.x;
			}
			else {
				m_PosX += (xDifReal / abs(xDifReal)) * PLAYER_SPEED * dt;
			}
			if (abs(yDifReal) - PLAYER_SPEED * dt <= 0.f)
			{
				m_PosY = targetPosition.y;
			}
			else
			{
				m_PosY += (yDifReal / abs(yDifReal)) * PLAYER_SPEED * dt;
			}
			if (m_PosX == targetPosition.x && m_PosY == targetPosition.y)
			{
				m_CurrentTile = m_NextTile;
			}
		}
		SetDirty();
	}
}

void CPlayerControllerServer::Init(CityMap* aCityMap)
{
	m_CityMap = aCityMap;
	m_CurrentTile = m_CityMap->GetMapHeight() / 2 * m_CityMap->GetMapWidth() + m_CityMap->GetMapWidth() / 2;
	while (m_CityMap->IsTrespassable(m_CurrentTile))
	{
		++m_CurrentTile;
	}
	m_NextTile = m_CurrentTile;
	CPlayerControllerServer::CoordinatePair currentCoord = IndexToMap(m_CurrentTile);
	CPlayerControllerServer::PositionPair targetPosition = MapToPosition(currentCoord.x, currentCoord.y);
	m_PosX = targetPosition.x;
	m_PosY = targetPosition.y;
	SetDirty();
}

void CPlayerControllerServer::MoveTile(Direction aDirection)
{
	if (m_CurrentTile == m_NextTile) // Currently stopped
	{
		CPlayerControllerServer::CoordinatePair coord = IndexToMap(m_CurrentTile);
		switch (aDirection)
		{
		case Direction::BOTTOM:
			if (coord.y < m_CityMap->GetMapHeight() - 1)
			{
				coord.y += 1;
			}
			break;
		case Direction::LEFT:
			if (coord.x > 0)
			{
				coord.x -= 1;
			}
			break;
		case Direction::RIGHT:
			if (coord.x < m_CityMap->GetMapWidth() - 1)
			{
				coord.x += 1;
			}
			break;
		case Direction::TOP:
			if (coord.y > 0)
			{
				coord.y -= 1;
			}
			break;
		}
		int l_PossibleNextTile = MapToIndex(coord.x, coord.y);
		if (!m_CityMap->IsTrespassable(l_PossibleNextTile)) {
			m_NextTile = l_PossibleNextTile;
		}
	}
}

int CPlayerControllerServer::MapToIndex(int x, int y)
{
	int width = m_CityMap->GetMapWidth();
	return y * width + x;
}

CPlayerControllerServer::CoordinatePair CPlayerControllerServer::IndexToMap(int index)
{
	CPlayerControllerServer::CoordinatePair result;
	int width = m_CityMap->GetMapWidth();
	result.y = index / width;
	result.x = index % width;
	return result;
}

CPlayerControllerServer::PositionPair CPlayerControllerServer::MapToPosition(int x, int y)
{
	CPlayerControllerServer::PositionPair result;
	result.x = (float)(x * m_CityMap->GetTileWidth()*2 + m_CityMap->GetTileWidth());
	result.y = (float)(y * m_CityMap->GetTileHeight()*2 + m_CityMap->GetTileHeight()/2);
	return result;
}