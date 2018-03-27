#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include "Utils\Defines.h"

class CAnimation;
class CAnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
	CAnimatedSprite(float aFrameTime = 0.2f, bool aPaused = false, bool aLooped = true);
	
	void SetAnimation(CAnimation* aAnimation);
	CAnimation* GetAnimation() const;
	void Update(const float& aDeltaTime);
	void SetFrameTime(float time);
	void Play();
	void Play(CAnimation* animation);
	void Pause();
	void Stop();
	void SetLooped(bool looped);
	void SetColor(const sf::Color& aColor);
	sf::FloatRect GetLocalBounds() const;
	sf::FloatRect GetGlobalBounds() const;
	sf::Vector2u GetSpriteSize() const;
	bool IsLooped() const;
	bool IsPlaying() const;
	float GetFrameTime() const;
	void SetFrame(std::size_t aNewFrame, bool aResetTime = true);
protected:
	virtual void draw(sf::RenderTarget& aTarget, sf::RenderStates aStates) const;
private:
	const sf::Sprite m_Sprite;
	CAnimation* m_pAnimation;
	sf::Texture* m_pTexture;
	float m_FrameTime;
	float m_CurrentTime;
	size_t m_CurrentFrame;
	bool m_IsPaused;
	bool m_IsLooped;
	sf::Vertex m_Vertices[4];
};