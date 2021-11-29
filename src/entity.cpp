#include "entity.hpp"
#include "scene_graph/grid.hpp"

#include <cassert>

Entity::Entity(int hitpoints, Grid* grid)
	: mGrid(grid)
	, mCells()
	, mVelocity()
	, mHitpoints(hitpoints)
{
}

void Entity::registerCell(std::list<Entity*>* node)
{
	mCells.push_back(node);
}

void Entity::removeFromGrid()
{
	// Find in each cell the entity's pointer and remove it
	for (auto& node : mCells)
	{
		node->remove(this);
	}

	// Clear the 2D list of pointers contained by the entity
	mCells.clear();
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

	if (mHitpoints == 0)
		removeFromGrid();
}

void Entity::destroy()
{
	mHitpoints = 0;
	removeFromGrid();
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
	removeFromGrid();

	// Distance = time * speed
	move(mVelocity * dt.asSeconds());

	mGrid->insert(this);
}