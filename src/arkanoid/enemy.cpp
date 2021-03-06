#include "arkanoid/enemy.hpp"
#include "data_tables.hpp"
#include "resources/resource_holder.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<EnemyData> Table = initializeEnemyData();
}

Enemy::Enemy(Type type, const TextureHolder& textures, Grid* grid)
	: Entity(1, grid)
	, mType(type)
	, mSpriteAnimation(textures.get(Table[type].texture), Table[type].textureRect)
	, mExplosion(textures.get(Textures::Particle), sf::IntRect(0, 0, 202, 32))
	, mShowExplosion(true)
{
	mSpriteAnimation.setFrameSize(Table[type].frameSize);
	mSpriteAnimation.setNumFrames(Table[type].numFrames);
	mSpriteAnimation.setDuration(Table[type].duration);
	mSpriteAnimation.setSpacing(Table[type].spacing);
	mSpriteAnimation.setRepeating(true);

	mExplosion.setFrameSize(sf::Vector2i(32, 32));
	mExplosion.setSpacing(2);
	mExplosion.setNumFrames(6);
	mExplosion.setDuration(sf::seconds(0.3f));

	centerOrigin(mSpriteAnimation);
	centerOrigin(mExplosion);
}

void Enemy::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
	{
		target.draw(mExplosion, states);
	}
	else
	{
		target.draw(mSpriteAnimation, states);
	}
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDestroyed())
	{
		mExplosion.update(dt);
		return;
	}

	mSpriteAnimation.update(dt);
	Entity::updateCurrent(dt, commands);
}

bool Enemy::isMarkedForRemoval() const
{
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

sf::FloatRect Enemy::getBoundingRect() const
{
	// Gives the entity's shape (hitbox for collision detection)
	// Take into account any transformation (rotation mainly)
	return getWorldTransform().transformRect(mSpriteAnimation.getGlobalBounds());
}

unsigned int Enemy::getCategory() const
{
	return Category::Enemy;
}

float Enemy::getMaxSpeed() const
{
	return Table[mType].speed;
}