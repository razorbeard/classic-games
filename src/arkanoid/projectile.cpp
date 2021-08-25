#include "projectile.hpp"
#include "utility.hpp"
#include "resources/resource_holder.hpp"
#include "data_tables.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures, Grid* grid)
	: Entity(1, grid)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mDestruction(textures.get(Table[type].destructionTexture), Table[type].destructionTextureRect)
	, mShowDestruction(true)
{
	mDestruction.setFrameSize(Table[type].frameSize);
	mDestruction.setNumFrames(Table[type].numFrames);
	mDestruction.setDuration(Table[type].duration);
	mDestruction.setSpacing(Table[type].spacing);

	centerOrigin(mSprite);
	centerOrigin(mDestruction);

	// Correct impact position for bullets
	if (mType == Projectile::Bullet)
		mDestruction.move(0.0f, -10.0f);

	// Redefining the hitbox for the ball
	sf::FloatRect const rect{ getBoundingRect() };

	if (mType == Projectile::Ball)
	{
		float const radius{ rect.width / 2.0f };
		float const pi{ 3.14159f };

		mHitbox.setPointCount(8);

		for (std::size_t idx{ 0 }; idx < mHitbox.getPointCount(); ++idx)
		{
			float const angle{ (idx * 2 * pi) / mHitbox.getPointCount() + pi / 2 };
			float const x = std::cos(angle) * radius;
			float const y = std::sin(angle) * radius;

			mHitbox.setPoint(idx, sf::Vector2f(x + radius, y + radius) + sf::Vector2f(rect.left, rect.top));
		}
	}
	else
	{
		mHitbox.setPointCount(4);

		mHitbox.setPoint(0, sf::Vector2f(rect.left, rect.top));
		mHitbox.setPoint(1, sf::Vector2f(rect.left, rect.top + rect.height));
		mHitbox.setPoint(2, sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
		mHitbox.setPoint(3, sf::Vector2f(rect.left + rect.width, rect.top));
	}
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDestroyed())
	{
		mDestruction.update(dt);
		return;
	}

	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowDestruction)
	{
		target.draw(mDestruction, states);
	}
	else
	{
		target.draw(mSprite, states);
	}
}

bool Projectile::isMarkedForRemoval() const
{
	return isDestroyed() && (mDestruction.isFinished() || !mShowDestruction);
}

void Projectile::remove()
{
	Entity::remove();

	//mShowDestruction = false;
}

unsigned int Projectile::getCategory() const
{
	return Category::Projectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

Projectile::Type Projectile::getType() const
{
	return mType;
}

float Projectile::getMaxSpeed() const
{
	return Table[mType].speed;
}

int Projectile::getDamage() const
{
	return Table[mType].damage;
}

std::vector<sf::Vector2f> Projectile::getHitboxPoints() const
{
	std::vector<sf::Vector2f> transformedPoints(mHitbox.getPointCount());

	for (std::size_t i = 0; i < mHitbox.getPointCount(); ++i)
	{
		transformedPoints[i] = getTransform().transformPoint(mHitbox.getPoint(i));
	}

	return transformedPoints;
}