#include "tetris/board.hpp"

#include <iostream>
Board::Board(int width, int height)
	: mCells(height, std::vector<Field>(width))
	, mWidth(width)
	, mHeight(height)
{
}

bool Board::doesPieceFit(Tetromino tetromino, Tetromino::Rotation rotation, int xShift, int yShift)
{
	tetromino.applyRotation(rotation);
	auto vSquaresCoord{ tetromino.getSquaresInfo() };
	
	for (size_t i{ 0 }; i < vSquaresCoord.size(); ++i)
	{
		int vx = vSquaresCoord[i].x + xShift + tetromino.getPosition().x;
		int vy = vSquaresCoord[i].y + yShift + tetromino.getPosition().y;
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

void Board::printGrid(sf::RenderTarget& target)
{
	if (!mTetrominos.empty())
	{
		for (size_t i{ 0 }; i < mTetrominos.size(); ++i)
		{
			target.draw(*mTetrominos[i]);
		}
	}
}

void Board::addBlock(std::unique_ptr<Tetromino>& tetromino)
{
	auto vSquaresCoord{ tetromino->getSquaresInfo() };

	for (size_t i{ 0 }; i < vSquaresCoord.size(); ++i)
	{
		int vx = vSquaresCoord[i].x + tetromino->getPosition().x;
		int vy = vSquaresCoord[i].y + tetromino->getPosition().y;
		mCells[vy][vx].isOccupied = true;
	}

	mTetrominos.push_back(std::move(tetromino));
}