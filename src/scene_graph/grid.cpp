#include "grid.hpp"
#include "entity.hpp"

Grid::Grid(int width, int height, float cellSize)
	: mCellSize(cellSize)
	, mEntities()
	, mCells(gaugeCellSize(width, cellSize),
			 std::vector<Entity*>(gaugeCellSize(height, cellSize), nullptr))
{
}

int Grid::gaugeCellSize(int length, float cellSize)
{
	// For testing, we only use one cell
	// When collisions works, replace with : return (int)(length / cellSize);
	return 1;
}

void Grid::add(Entity* entity)
{
	int x{ (int)(entity->getPosition().x / mCellSize) };
	int y{ (int)(entity->getPosition().y / mCellSize) };

	mEntities.push_back(entity);
	mCells[x][y] = entity;
}

void Grid::checkCollisions(std::set<EntityPair>& collisionPairs)
{
	for (auto it1 = mCells.begin(); it1 != mCells.end(); it1++)
	{
		for (auto it2 = it1->begin(); it2 != it1->end(); it2++)
		{
			handleCell(*it2, collisionPairs);
		}
	}
}

void Grid::updateCells()
{
	// Look each cell and remove entities from the linked-list that have been destroyed
	auto cellBegin = std::remove_if(mEntities.begin(), mEntities.end(), [](Entity* entity) { return entity->isDestroyed(); });
	mEntities.erase(cellBegin, mEntities.end());
}

void Grid::handleCell(Entity* entity, std::set<EntityPair>& collisionPairs)
{
	auto it1 = mEntities.begin();

	while (it1 != mEntities.end())
	{
		auto it2 = std::next(it1, 1);

		while (it2 != mEntities.end())
		{
			Entity* other{ *it2 };
			if ((*it1)->getBoundingRect().intersects(other->getBoundingRect()))
			{
				// std::minmax helps to avoid duplicates
				collisionPairs.insert((std::minmax((*it1), other)));
			}
			++it2;
		}
		++it1;
	}
}