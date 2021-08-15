#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <set>
#include <list>

class Entity;

class Grid
{
public:
	using EntityPair = std::pair<Entity*, Entity*>;

public:
	Grid(int width, int height, float cellSize);

	void add(Entity* entity);
	void updateCells();
	void checkCollisions(std::set<EntityPair>& collisionPairs);

private:
	int gaugeCellSize(int length, float cellSize);
	void handleCell(Entity* entity, std::set<EntityPair>& collisionPairs);

private:
	std::vector<std::vector<Entity*>> mCells;
	std::list<Entity*> mEntities;
	float mCellSize;
};

#endif // GRID_HPP