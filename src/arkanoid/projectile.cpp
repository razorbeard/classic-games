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