#include "tetris/tetromino.hpp"
#include "data_tables.hpp"
#include "resources/resource_holder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <iostream>

namespace
{
	const std::vector<TetrominoData> Table = initializeTetrominoData();
	const int squareLength = Table[0].textureRect.width;

	// 1 Tetromino = 4 squares pieces, each located with a 1D index in a NxN matrix
	// We also add an extra info about the size of the bounding box of each shape,
	// at the end of each array
	const int BlockInfo[7][5] = { { 4, 5, 6, 7, 4 }, // I-shape
								  { 0, 1, 4, 5, 2 }, // O-shape
								  { 0, 1, 5, 6, 3 }, // Z-shape
								  { 4, 5, 1, 2, 3 }, // S-shape
								  { 1, 4, 5, 6, 3 }, // T-shape
								  { 0, 4, 5, 6, 3 }, // L-shape
								  { 4, 5, 6, 2, 3 }, // J-shape
	};
}

Tetromino::Tetromino(int id, const TextureHolder& textures)
	: mId(id)
	, mSprite(textures.get(Table[id].texture), Table[id].textureRect)
{
	// Convert back to 2D each square position, and store them in a vector
	mSquaresInfo.first = BlockInfo[mId][4];
	for (size_t i{ 0 }; i < 4; ++i)
	{
		int x = BlockInfo[mId][i] % 4;
		int y = BlockInfo[mId][i] / 4;
		mSquaresInfo.second[i] = { x, y };
	}
}

void Tetromino::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// mSprite acts as a buffer to avoid stocking multiple sprites
	for (int i{ 0 }; i < mSquaresInfo.second.size(); ++i)
	{
		mSprite.setPosition(squareLength * (mSquaresInfo.second[i].x + getPosition().x), squareLength * (mSquaresInfo.second[i].y + getPosition().y));
		target.draw(mSprite);
	}
}

sf::Sprite Tetromino::getSprite() const
{
	return mSprite;
}

std::array<sf::Vector2i, 4> Tetromino::getSquaresInfo() const
{
	return mSquaresInfo.second;
}

void Tetromino::applyRotation(Rotation rotation)
{
	if (rotation == Rotation::None)
		return;

	// We're using the Super Rotation System (SRS)
	for (size_t i{ 0 }; i < mSquaresInfo.second.size(); ++i)
	{
		sf::Vector2i buffer{ mSquaresInfo.second[i] };
		if (rotation == Rotation::Clockwise)
		{
			mSquaresInfo.second[i].x = 1 - (buffer.y - (mSquaresInfo.first - 2));
			mSquaresInfo.second[i].y = buffer.x;
		}
		else if (rotation == Rotation::AntiClockwise)
		{
			mSquaresInfo.second[i].x = buffer.y;
			mSquaresInfo.second[i].y = 1 - (buffer.x - (mSquaresInfo.first - 2));
		}
	}
}