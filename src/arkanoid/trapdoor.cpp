#include "arkanoid/trapdoor.hpp"
#include "resources/resource_holder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Trapdoor::Trapdoor(const TextureHolder& textures)
	: mSpriteAnimation(textures.get(Textures::Arkanoid), sf::IntRect(0, 373, 612, 72))
	, mIsActivated(false)
{
	mSpriteAnimation.setFrameSize(sf::Vector2i(102, 36));
	mSpriteAnimation.setNumFrames(12);
	mSpriteAnimation.setDuration(sf::seconds(2.0f));
}

void Trapdoor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (mIsActivated)
	{
		mSpriteAnimation.restart();
		mIsActivated = false;
	}

	mSpriteAnimation.update(dt);
}

void Trapdoor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSpriteAnimation, states);
}

void Trapdoor::activate()
{
	mIsActivated = true;
}