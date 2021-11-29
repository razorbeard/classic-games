#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "scene_graph/scene_node.hpp"

#include <list>

class Grid;

// Represent a game element in the world
class Entity : public SceneNode
{
public:
	explicit Entity(int hitpoints, Grid* grid = nullptr);

	int	getHitpoints() const;
	sf::Vector2f getVelocity() const;

	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);

	void repair(int points);
	void damage(int points);
	void destroy();
	virtual void remove();
	virtual bool isDestroyed() const override;

	void registerCell(std::list<Entity*>* node);
	void removeFromGrid();

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	Grid* mGrid;
	std::vector<std::list<Entity*>*> mCells;
	sf::Vector2f mVelocity;
	int mHitpoints;
};

#endif // ENTITY_HPP