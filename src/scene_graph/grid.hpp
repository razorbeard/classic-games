#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <set>
#include <list>

class Entity;

class Cell
{
public:
	void registerEntity(Entity* entity);

	std::list<Entity*> getEntities();

private:
	std::list<Entity*> mEntities;
};

class Grid
{
public:
	using EntityPair = std::pair<Entity*, Entity*>;

public:
	Grid(int width, int height, float cellSize);

	void insert(Entity* entity);
	void checkCollisions(std::set<EntityPair>& collisionPairs);

private:
	int gaugeCellSize(int length, float cellSize);
	void handleCell(Cell cell, std::set<EntityPair>& collisionPairs);

private:
	std::vector<std::vector<Cell>> mCells;
	float mCellSize;
};

#endif // GRID_HPP