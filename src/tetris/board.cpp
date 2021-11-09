#include "tetris/board.hpp"

#include <iostream>

Board::Board(int width, int height, sf::Vector2i offset)
	: mCells(height, std::vector<Field>(width))
	, mWidth(width)
	, mHeight(height)
	, mSpriteContainer(nullptr)
{
	//for (size_t i{ 0 }; i < mHeight; ++i)
	//{
	//	for (size_t j{ 0 }; j < mWidth; ++j)
	//	{
	//		mCells[i][j].mSprite.setPosition(offset.x + j * 40, offset.y + i * 40);
	//	}
	//}
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

void Board::printGrid(sf::RenderTarget& target)
{
	for (int i{ 0 }; i < mHeight; ++i)
	{
		for (int j{ 0 }; j < mWidth; ++j)
		{
			if (mCells[i][j].mSprite != nullptr)
			{
				mCells[i][j].mSprite->setPosition(40*j, 40*i); // + offset
				target.draw(*mCells[i][j].mSprite);
			}
		}
	}
}

void Board::addBlock(std::unique_ptr<Tetromino>& tetromino)
{
	auto vTetroCoord{ tetromino->getCoordinates() };
	auto vSquaresCoord{ tetromino->getLocalCoordinates() };
	int id = tetromino->getId();

	for (size_t i{ 0 }; i < vSquaresCoord.size(); ++i)
	{
		int vx = vSquaresCoord[i].x + vTetroCoord.x;
		int vy = vSquaresCoord[i].y + vTetroCoord.y;
		mCells[vy][vx].isOccupied = true;
		mCells[vy][vx].mSprite = &mSpriteContainer->at(id);
	}
}

void Board::setSpriteContainer(std::unordered_map<int, sf::Sprite>* spriteContainer)
{
	mSpriteContainer = spriteContainer;
}