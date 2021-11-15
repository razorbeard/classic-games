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
	, mHasLost(false)
	, mScore(0)
	, mLevel(0)
	, mLinesCleared(0)
	, mOffset(272, 24)
	, mBagOffset(810, 108)
	, mBoard(12, 18)
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
	
	loadTextures();
	buildScene();
}

void Tetris::loadTextures()
{
	mTextures.load(Textures::Tetris, "resources/textures/tetris.png");
	mTextures.load(Textures::TetrisBackground, "resources/textures/tetris_background.png");
}

void Tetris::update(sf::Time dt)
{
	mElapsedTime += dt;
	sf::Time dropTime{ sf::seconds(0.9f / (0.06f * mLevel + 1)) };

	if (mElapsedTime > dropTime)
	{
		mElapsedTime = sf::Time::Zero;
		if (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 0, 1))
		{
			mCurrentTetromino->shift(0, +1);
		}
		else
		{
			// Update the board, pass the current tetromino
			auto [lineIndexes, dropPoints] = mBoard.addBlock(mCurrentTetromino);
			int nLinesCleared = mBoard.removeLines(lineIndexes);

			updateScore(nLinesCleared, dropPoints);

			getNextTetromino();
		}
	}

	updateTexts();
}

void Tetris::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(mBackground);
	mTarget.draw(mScoreText);
	mTarget.draw(mLevelText);
	mTarget.draw(mLinesClearedText);
	mTarget.draw(*mCurrentTetromino);
	mTarget.draw(*mTetrominoBag.front());
	mBoard.display(mTarget);
}

void Tetris::buildScene()
{
	// Background
	mBackground.setTexture(mTextures.get(Textures::TetrisBackground));

	// Statistics texts
	initializeText(mScoreText, { 132.0f, 133.0f });
	initializeText(mLevelText, { 132.0f, 288.0f });
	initializeText(mLinesClearedText, { 132.0f, 454.0f });

	// We store all the needed squares sprites in one map, this is
	// our buffer when we want to draw the tetrominos
	for (int id{ 0 }; id < Table.size(); ++id)
	{
		mSpriteContainer[id].setTexture(mTextures.get(Table[id].texture));
		mSpriteContainer[id].setTextureRect(Table[id].textureRect);
	}

	// Board: give a reference to the previous map created
	mBoard.setSpriteContainer(&mSpriteContainer);
	mBoard.setOffset(mOffset);

	// Assign a random tetromino to start and populate the tetromino's bag
	mCurrentTetromino = std::make_unique<Tetromino>(randomInt(7), &mSpriteContainer);
	mCurrentTetromino->setCoordinates(mBoard.getSpawnLocation(), 0);
	mCurrentTetromino->setOffset(mOffset);

	for (size_t i{ 0 }; i < 6; ++i)
	{
		std::unique_ptr<Tetromino> randomTetro(std::make_unique<Tetromino>(randomInt(7), &mSpriteContainer));
		randomTetro->setOffset(mBagOffset);
		mTetrominoBag.push(std::move(randomTetro));
	}
}

void Tetris::getNextTetromino()
{
	// Draw a new tetromino from the top of the bag
	if (!mTetrominoBag.empty())
	{
		mCurrentTetromino = std::move(mTetrominoBag.front());
		mCurrentTetromino->setCoordinates(mBoard.getSpawnLocation(), 0);
		mCurrentTetromino->setOffset(mOffset);
		mTetrominoBag.pop();
		
		// Check if we can fit the next Tetromino, else the player has lost
		if (!mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 0, 0))
		{
			mHasLost = true;
			return;
		}

		// Keep the bag filled
		if (mTetrominoBag.size() < 4)
		{
			for (size_t i{ 0 }; i < 3; ++i)
			{
				std::unique_ptr<Tetromino> randomTetro(std::make_unique<Tetromino>(randomInt(7), &mSpriteContainer));
				randomTetro->setOffset(mBagOffset);
				mTetrominoBag.push(std::move(randomTetro));
			}
		}
	}
}

CommandQueue& Tetris::getCommandQueue()
{
	return mCommandQueue;
}

void Tetris::updateScore(int nLinesCleared, int dropPoints)
{
	mScore += dropPoints; // Points for dropping a piece
	mLinesCleared += nLinesCleared;
	
	// Each 10 lines cleared, the level increase
	mLevel = mLinesCleared / 10;

	// Original Nintendo scoring system
	switch (nLinesCleared)
	{
	case 1:
		mScore += 40 * (mLevel + 1);
		break;
	case 2:
		mScore += 100 * (mLevel + 1);
		break;
	case 3:
		mScore += 300 * (mLevel + 1);
		break;
	case 4:
		mScore += 1200 * (mLevel + 1);
		break;
	default:
		break;
	}
}

void Tetris::rotate(Tetromino::Rotation rotation)
{
	if (mBoard.doesPieceFit(*mCurrentTetromino, rotation, 0, 0))
		mCurrentTetromino->applyRotation(rotation);
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

void Tetris::hardDrop()
{
	while (mBoard.doesPieceFit(*mCurrentTetromino, Tetromino::Rotation::None, 0, 1))
	{
		mCurrentTetromino->shift(0, +1);
	}

	auto [lineIndexes, dropPoints] = mBoard.addBlock(mCurrentTetromino);
	int nLinesCleared = mBoard.removeLines(lineIndexes);

	updateScore(nLinesCleared, dropPoints);

	getNextTetromino();
}

void Tetris::initializeText(sf::Text& text, sf::Vector2f pos)
{
	centerOrigin(text);

	text.setFont(mFonts.get(Fonts::ID::EightBitArcade));
	text.setCharacterSize(64);
	text.setFillColor(sf::Color(172, 50, 50));
	text.setPosition(pos);
}

void Tetris::updateTexts()
{
	mScoreText.setString(std::to_string(mScore));
	centerOrigin(mScoreText);

	mLevelText.setString(std::to_string(mLevel));
	centerOrigin(mLevelText);

	mLinesClearedText.setString(std::to_string(mLinesCleared));
	centerOrigin(mLinesClearedText);
}

bool Tetris::hasLost() const
{
	return mHasLost;
}