#include "entity.hpp"
#include "scene_graph/grid.hpp"

#include <cassert>

Entity::Entity(int hitpoints, Grid* grid)
	: mVelocity()
	, mHitpoints(hitpoints)
	, mGrid(grid)
{
	// Add the scene node to the subdivision only if the user want it
	// Thus, the user can avoid unwanted entities in the grid
	if (mGrid)
		mGrid->add(this);
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}

int Entity::getHitpoints() const
{
	return mHitpoints;
}

void Entity::repair(int points)
{
	assert(points > 0);

	mHitpoints += points;
}

void Entity::damage(int points)
{
	assert(points > 0);

	mHitpoints -= points;
}

void Entity::destroy()
{
	mHitpoints = 0;
}

void Entity::remove()
{
	destroy();
}

bool Entity::isDestroyed() const
{
	// Tells whether entities are logically dead
	return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	// Distance = time * speed
	move(mVelocity * dt.asSeconds());
}