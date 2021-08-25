#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "scene_graph/scene_node.hpp"

#include <SFML/Graphics/ConvexShape.hpp>

#include <list>
#include <vector>

class Grid;

// Represent a game element in the world
class Entity : public SceneNode
{
public:
	explicit Entity(int hitpoints, Grid* grid = nullptr);

	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);
	sf::Vector2f getVelocity() const;

	int	getHitpoints() const;
	void repair(int points);
	void damage(int points);
	void destroy();
	virtual void remove();
	virtual bool isDestroyed() const override;

	virtual std::vector<sf::Vector2f> getHitboxPoints() const;
	void registerCell(std::list<Entity*>* node);
	void removeFromGrid();

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	sf::Vector2f mVelocity;
	int mHitpoints;

	Grid* mGrid;
	std::vector<std::list<Entity*>*> mCells;
};

#endif // ENTITY_HPP