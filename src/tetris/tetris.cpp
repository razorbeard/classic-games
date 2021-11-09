#include "tetris/tetris.hpp"
#include "data_tables.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

namespace
{
	const std::vector<TetrominoData> Table = initializeTetrominoData();
	//const int squareLength = Table[0].textureRect.width;
}

Tetris::Tetris(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(outputTarget)
	, mSceneTexture()
	, mWorldView(outputTarget.getDefaultView())
	, mTextures()
	, mFonts(fonts)
	, mSounds(sounds)
	, mWorldBounds(0.0f, 0.0f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mElapsedTime(sf::Time::Zero)
	, mCurrentTetromino(nullptr)
	, mBoard(12, 18, { 50, 50 })
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
	
	loadTextures();

	// We store all the needed squares sprites in one map
	for (int id{ 0 }; id < Table.size(); ++id)
	{
		mSpriteContainer[id].setTexture(mTextures.get(Table[id].texture));
		mSpriteContainer[id].setTextureRect(Table[id].textureRect);
	}

	// Initialize a reference to all the squares sprites for the board
	mBoard.setSpriteContainer(&mSpriteContainer);

	// Assign a random tetromino to start and populate the tetromino's bag
	mCurrentTetromino = std::make_unique<Tetromino>(randomInt(7), &mSpriteContainer);
	for (size_t i{ 0 }; i < 6; ++i)
	{
		std::unique_ptr<Tetromino> randomTetro(std::make_unique<Tetromino>(randomInt(7), &mSpriteContainer));
		mTetrominoBag.push(std::move(randomTetro));
	}
}

void Tetris::loadTextures()
{
	mTextures.load(Textures::Tetris, "resources/textures/tetris.png");
}

void Tetris::update(sf::Time dt)
{
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(1.0f))
	{
		mElapsedTime = sf::Time::Zero;
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 0, 1))
		{
			mCurrentTetromino->shift(0, +1);
		}
		else
		{
			// Update the board, pass the current tetromino
			mBoard.addBlock(mCurrentTetromino);

			getNextTetromino();
		}
	}
}

void Tetris::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(*mCurrentTetromino);
	mBoard.printGrid(mTarget);
}

void Tetris::getNextTetromino()
{
	// Draw a new tetromino from the bag
	if (!mTetrominoBag.empty())
	{
		mCurrentTetromino = std::move(mTetrominoBag.front());
		mTetrominoBag.pop();

		// Keep the bag filled
		if (mTetrominoBag.size() < 4)
		{
			for (size_t i{ 0 }; i < 3; ++i)
			{
				std::unique_ptr<Tetromino> randomTetro(std::make_unique<Tetromino>(randomInt(7), &mSpriteContainer));
				mTetrominoBag.push(std::move(randomTetro));
			}
		}
	}
}

CommandQueue& Tetris::getCommandQueue()
{
	return mCommandQueue;
}

void Tetris::rotate()
{
	if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::Clockwise, 0, 0))
		mCurrentTetromino->applyRotation(Tetromino::Rotation::Clockwise);
}

void Tetris::moveCurrentPiece(Direction direction)
{
	switch (direction)
	{
	case Tetris::Direction::Left:
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, -1, 0))
			mCurrentTetromino->shift(-1, 0);
		break;
	case Tetris::Direction::Right:
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 1, 0))
			mCurrentTetromino->shift(+1, 0);
		break;
	case Tetris::Direction::Down:
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 0, 1))
			mCurrentTetromino->shift(0, +1);
		break;
	case Tetris::Direction::Up:
		break;
	default:
		break;
	}
}