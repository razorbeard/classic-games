#include "grid.hpp"
#include "entity.hpp"
#include "utility.hpp"

Grid::Grid(int width, int height, float cellSize)
	: mCellSize(cellSize)
	, mCells(gaugeCellSize(width, cellSize),
			 std::vector<Cell>(gaugeCellSize(height, cellSize)))
{
}

int Grid::gaugeCellSize(int length, float cellSize)
{
	// Add an extra cell to take the whole width/height into account
	return (int)(length / cellSize) + 1;
}

void Grid::insert(Entity* entity)
{
	int const xStart{ (int)(entity->getBoundingRect().left / mCellSize) };
	int const xEnd{ (int)((entity->getBoundingRect().left + entity->getBoundingRect().width) / mCellSize) };
	int const yStart{ (int)(entity->getBoundingRect().top / mCellSize) };
	int const yEnd{ (int)((entity->getBoundingRect().top + entity->getBoundingRect().height) / mCellSize) };

	for (auto x{ xStart }; x <= xEnd; x++)
	{
		for (auto y{ yStart }; y <= yEnd; y++)
		{
			mCells[x][y].registerEntity(entity);
		}
	}
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

void Grid::handleCell(Cell cell, std::set<EntityPair>& collisionPairs)
{
	auto entities = cell.getEntities();

	// No entity found in the cell: skip the test
	if (entities.empty())
		return;

	auto it1 = entities.begin();

	// Detect collision by comparing all existing entities two-by-two
	while (it1 != entities.end())
	{
		auto it2 = std::next(it1, 1);

		while (it2 != entities.end())
		{
			Entity* other{ *it2 };
			if (isColliding((*it1)->getHitboxPoints(), other->getHitboxPoints()))
			{
				// std::minmax helps to avoid duplicates
				collisionPairs.insert((std::minmax((*it1), other)));
			}
			++it2;
		}
		++it1;
	}
}

void Cell::registerEntity(Entity* entity)
{
	// The cell keep a track of the entity
	mEntities.push_back(entity);

	// The entity keep a track of the current entities in the cell
	entity->registerCell(&mEntities);
}

std::list<Entity*> Cell::getEntities()
{
	return mEntities;
}