#include "AnimatedSprite.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include "Animation.h"


CAnimatedSprite::CAnimatedSprite(float aFrameTime, bool aPaused, bool aLooped) :
m_pAnimation(NULL)
, m_FrameTime(aFrameTime)
, m_IsPaused(aPaused)
, m_IsLooped(aLooped)
, m_CurrentFrame(0)
, m_pTexture(NULL)
{

}

void CAnimatedSprite::SetAnimation(CAnimation* aAnimation)
{
	m_pAnimation = aAnimation;
	m_pTexture = m_pAnimation->GetTexture();
	m_CurrentFrame = 0;
	SetFrame(m_CurrentFrame);
}

CAnimation* CAnimatedSprite::GetAnimation() const
{
	return m_pAnimation;
}

void CAnimatedSprite::Update(const float& aDeltaTime)
{
	// if not paused and we have a valid animation
	if (!m_IsPaused && m_pAnimation)
	{
		// add delta time
		m_CurrentTime += aDeltaTime;

		// if current time is bigger then the frame time advance one frame
		if (m_CurrentTime >= m_FrameTime)
		{
			// reset time, but keep the remainder
			m_CurrentTime = m_CurrentTime - m_FrameTime;

			// get next Frame index
			if (m_CurrentFrame + 1 < m_pAnimation->GetSize())
				m_CurrentFrame++;
			else
			{
				// animation has ended
				m_CurrentFrame = 0; // reset to start

				if (!m_IsLooped)
				{
					m_IsPaused = true;
				}

			}

			// set the current frame, not reseting the time
			SetFrame(m_CurrentFrame, false);
		}
	}
}

void CAnimatedSprite::SetFrameTime(float time)
{
	m_FrameTime = time;
}

void CAnimatedSprite::Play()
{
	m_IsPaused = false;
}

void CAnimatedSprite::Play(CAnimation* animation)
{
	if (m_pAnimation != animation) {
		SetAnimation(animation);
	}
	Play();
}

void CAnimatedSprite::Pause()
{
	m_IsPaused = true;
}

void CAnimatedSprite::Stop()
{
	Pause();
	m_CurrentFrame = 0;
	SetFrame(m_CurrentFrame);
}

void CAnimatedSprite::SetLooped(bool looped)
{
	m_IsLooped = looped;
}

void CAnimatedSprite::SetColor(const sf::Color& aColor)
{
	m_Vertices[0].color = aColor;
	m_Vertices[1].color = aColor;
	m_Vertices[2].color = aColor;
	m_Vertices[3].color = aColor;
}

bool CAnimatedSprite::IsLooped() const
{
	return m_IsLooped;
}

bool CAnimatedSprite::IsPlaying() const
{
	return !m_IsPaused;
}

float CAnimatedSprite::GetFrameTime() const
{
	return m_FrameTime;
}

void CAnimatedSprite::SetFrame(std::size_t aNewFrame, bool aResetTime)
{
	if (m_pAnimation)
	{
		//calculate new vertex positions and texture coordinates
		sf::IntRect rect = m_pAnimation->GetFrame(aNewFrame);

		m_Vertices[0].position = sf::Vector2f(0.f, 0.f);
		m_Vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
		m_Vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
		m_Vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

		float left = static_cast<float>(rect.left) + 0.0001f;
		float right = left + static_cast<float>(rect.width);
		float top = static_cast<float>(rect.top);
		float bottom = top + static_cast<float>(rect.height);

		m_Vertices[0].texCoords = sf::Vector2f(left, top);
		m_Vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_Vertices[2].texCoords = sf::Vector2f(right, bottom);
		m_Vertices[3].texCoords = sf::Vector2f(right, top);
	}

	if (aResetTime) {
		m_CurrentTime = 0.f;
	}
}

void CAnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_pAnimation && m_pTexture)
	{
		states.transform *= getTransform();
		states.texture = m_pTexture;
		target.draw(m_Vertices, 4, sf::Quads, states);
	}
};

sf::FloatRect CAnimatedSprite::GetLocalBounds() const
{
	return m_Sprite.getLocalBounds();
}

sf::FloatRect CAnimatedSprite::GetGlobalBounds() const
{
	return m_Sprite.getGlobalBounds();
}

sf::Vector2u CAnimatedSprite::GetSpriteSize() const
{
	return sf::Vector2u(m_Vertices[3].position.x - m_Vertices[0].position.x, m_Vertices[1].position.y - m_Vertices[0].position.y);
}