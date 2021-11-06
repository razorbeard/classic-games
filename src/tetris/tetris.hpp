#ifndef TETRIS_HPP
#define TETRIS_HPP

#include "commands/command_queue.hpp"
#include "resources/resource_identifiers.hpp"
#include "resources/resource_holder.hpp"
#include "tetris/tetromino.hpp"
#include "tetris/board.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <array>
#include <queue>

class SoundPlayer;

class Tetris : private sf::NonCopyable
{
public:
	enum class Direction
	{
		Left,
		Right,
		Down,
		Up
	};

public:
	Tetris(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
	void update(sf::Time dt);
	void draw();
	void rotate();
	void moveCurrentPiece(Direction dir);

	CommandQueue& getCommandQueue();

private:
	void loadTextures();

	void getNextTetromino();

private:
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mWorldView;
	sf::FloatRect mWorldBounds;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;

	sf::Time mElapsedTime;

	std::unique_ptr<Tetromino> mCurrentTetromino;
	std::queue<std::unique_ptr<Tetromino>> mTetrominoBag;
	Board mBoard;
	CommandQueue mCommandQueue;
};

#endif // TETRIS_HPP