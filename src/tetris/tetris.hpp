#ifndef TETRIS_HPP
#define TETRIS_HPP

#include "commands/command_queue.hpp"
#include "resources/resource_identifiers.hpp"
#include "resources/resource_holder.hpp"
#include "tetris/tetromino.hpp"
#include "tetris/board.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>
#include <queue>
#include <unordered_map>

class SoundPlayer;
class MusicPlayer;

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
	Tetris(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, MusicPlayer& music);
	void update(sf::Time dt);
	void draw();
	void rotate(Tetromino::Rotation rotation);
	void hardDrop();
	void moveCurrentPiece(Direction dir);
	bool hasLost() const;

	CommandQueue& getCommandQueue();

private:
	void loadTextures();
	void buildScene();
	void updateSounds();

	void getNextTetromino();
	void initializeText(sf::Text& text, sf::Vector2f pos);
	void updateScore(int nLinesCleared, int dropPoints);
	void updateTexts();

private:
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mWorldView;
	sf::FloatRect mWorldBounds;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;
	MusicPlayer& mMusic;

	sf::Time mElapsedTime;

	std::unique_ptr<Tetromino> mCurrentTetromino;
	std::queue<std::unique_ptr<Tetromino>> mTetrominoBag;
	std::unordered_map<int, sf::Sprite> mSpriteContainer;
	sf::Sprite mBackground;
	sf::Text mScoreText;
	sf::Text mLevelText;
	sf::Text mLinesClearedText;
	int mScore;
	int mLevel;
	int mLinesCleared;
	sf::Vector2i mOffset;
	sf::Vector2i mBagOffset;
	Board mBoard;
	CommandQueue mCommandQueue;
	bool mHasLost;
};

#endif // TETRIS_HPP