#include "power_up.hpp"
#include "data_tables.hpp"
#include "block.hpp"
#include "commands/command_queue.hpp"
#include "resources/resource_holder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PowerUpData> Table = initializePowerUpData();
}

PowerUp::PowerUp(Type type, const TextureHolder& textures, Grid* grid)
	: Entity(1, grid)
	, mType(type)
	, mRemovalFlag(false)
	, mSpriteAnimation(textures.get(Table[type].texture), Table[type].textureRect)
{
	mSpriteAnimation.setFrameSize(Table[type].frameSize);
	mSpriteAnimation.setNumFrames(Table[type].numFrames);
	mSpriteAnimation.setDuration(Table[type].duration);
	mSpriteAnimation.setSpacing(Table[type].spacing);
	mSpriteAnimation.setRepeating(true);

	mHitbox.setPointCount(4);
	sf::FloatRect const rect{ getBoundingRect() };

	mHitbox.setPoint(0, sf::Vector2f(rect.left, rect.top));
	mHitbox.setPoint(1, sf::Vector2f(rect.left, rect.top + rect.height));
	mHitbox.setPoint(2, sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
	mHitbox.setPoint(3, sf::Vector2f(rect.left + rect.width, rect.top));
}

void PowerUp::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Reactivate drop for all the blocks when destroyed
	if (isDestroyed() && !mRemovalFlag)
	{
		Command command{};
		command.category = Category::Block;
		command.action = derivedAction<Block>([](Block& block, sf::Time)
											  {
												  block.canDropPowerUp(true);
											  });

		commands.push(command);

		mRemovalFlag = true;
		return;
	}

	mSpriteAnimation.update(dt);

	// Apply velocity
	Entity::updateCurrent(dt, commands);
}

bool PowerUp::isMarkedForRemoval() const
{
	return isDestroyed() && mRemovalFlag;
}

void PowerUp::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSpriteAnimation, states);
}

void PowerUp::apply(Arkanoid& world) const
{
	// Apply a function object defined in data_tables.cpp
	Table[mType].action(world);
}

unsigned int PowerUp::getCategory() const
{
	return Category::PowerUp;
}

sf::FloatRect PowerUp::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSpriteAnimation.getGlobalBounds());
}

std::vector<sf::Vector2f> PowerUp::getHitboxPoints() const
{
	std::vector<sf::Vector2f> transformedPoints(mHitbox.getPointCount());

	for (std::size_t i = 0; i < mHitbox.getPointCount(); ++i)
	{
		transformedPoints[i] = getTransform().transformPoint(mHitbox.getPoint(i));
	}

	return transformedPoints;
}