#include "tetris/tetris.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

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
	, mBoard(12, 18)
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
	
	loadTextures();

	// Assign a random tetromino to start and populate the tetromino's bag
	mCurrentTetromino = std::make_unique<Tetromino>(randomInt(7), mTextures);
	for (size_t i{ 0 }; i < 6; ++i)
	{
		std::unique_ptr<Tetromino> randomTetro(std::make_unique<Tetromino>(randomInt(7), mTextures));
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
			mCurrentTetromino->move(0, +1);
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
				std::unique_ptr<Tetromino> randomTetro(std::make_unique<Tetromino>(randomInt(7), mTextures));
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
			mCurrentTetromino->move(-1, 0);
		break;
	case Tetris::Direction::Right:
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 1, 0))
			mCurrentTetromino->move(+1, 0);
		break;
	case Tetris::Direction::Down:
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 0, 1))
			mCurrentTetromino->move(0, +1);
		break;
	case Tetris::Direction::Up:
		break;
	default:
		break;
	}
}