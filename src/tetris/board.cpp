#include "tetris/board.hpp"

#include <iostream>
#include <algorithm>

Board::Board(int width, int height)
	: mCells(height, std::vector<Field>(width))
	, mWidth(width)
	, mHeight(height)
	, mOffset(0, 0)
	, mSpriteContainer(nullptr)
{
}

bool Board::doesPieceFit(Tetromino tetromino, Tetromino::Rotation rotation, int xShift, int yShift)
{
	tetromino.applyRotation(rotation);
	auto vTetroCoord{ tetromino.getCoordinates() };
	auto vSquaresCoord{ tetromino.getLocalCoordinates() };
	
	for (size_t i{ 0 }; i < vSquaresCoord.size(); ++i)
	{
		int vx = vSquaresCoord[i].x + vTetroCoord.x + xShift;
		int vy = vSquaresCoord[i].y + vTetroCoord.y + yShift;
		if (vx < 0 || vx >= mWidth || vy < 0 || vy >= mHeight)
		{
			return false;
		}
		if (vx >= 0 && vx < mWidth && vy >= 0 && vy < mHeight)
		{
			if (mCells[vy][vx].isOccupied)
			{
				return false;
			}
		}
	}
	
	return true;
}

void Board::display(sf::RenderTarget& target)
{
	for (int i{ 0 }; i < mHeight; ++i)
	{
		for (int j{ 0 }; j < mWidth; ++j)
		{
			if (mCells[i][j].mSprite != nullptr)
			{
				mCells[i][j].mSprite->setPosition(40*j + mOffset.x, 40*i + mOffset.y);
				target.draw(*mCells[i][j].mSprite);
			}
		}
	}
}

std::tuple<std::set<int>, int> Board::addBlock(std::unique_ptr<Tetromino>& tetromino)
{
	auto vTetroCoord{ tetromino->getCoordinates() };
	auto vSquaresCoord{ tetromino->getLocalCoordinates() };
	int id = tetromino->getId();
	std::set<int> yCoordinates;
	
	for (size_t i{ 0 }; i < vSquaresCoord.size(); ++i)
	{
		int vx = vSquaresCoord[i].x + vTetroCoord.x;
		int vy = vSquaresCoord[i].y + vTetroCoord.y;
		yCoordinates.insert(vy);

		mCells[vy][vx].isOccupied = true;
		mCells[vy][vx].mSprite = &mSpriteContainer->at(id);
	}

	// Scoring: reward the player for taking risks
	int dropPoints{ mHeight - *(yCoordinates.rbegin()) };

	// These coordinates are the lines to check (ascending order)
	return { yCoordinates, dropPoints };
}

int Board::removeLines(std::set<int>& lineIndexes)
{
	int nLinesCleared{ 0 };

	for (int idx : lineIndexes)
	{
		if (std::all_of(mCells[idx].begin(), mCells[idx].end(), [](Field cell) {return cell.isOccupied == true; }))
		{
			for (size_t y = idx; y > 0; --y)
			{
				for (size_t x = 0; x < mWidth; ++x)
				{
					mCells[y][x].isOccupied = mCells[y-1][x].isOccupied;
					mCells[y][x].mSprite = mCells[y-1][x].mSprite;
				}
			}

			nLinesCleared++;
		}
	}

	return nLinesCleared;
}

void Board::setSpriteContainer(std::unordered_map<int, sf::Sprite>* spriteContainer)
{
	mSpriteContainer = spriteContainer;
}

void Board::setOffset(sf::Vector2i offset)
{
	mOffset = offset;
}

int Board::getSpawnLocation() const
{
	return mWidth/2 - 2;
}