#ifndef ARKANOID_HPP
#define ARKANOID_HPP

#include "commands/command_queue.hpp"
#include "block.hpp"
#include "vaus.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <array>
#include <queue>

class Enemy;
class SpriteNode;
class Trapdoor;
class SoundPlayer;
class Projectile;
class TextNode;

class Arkanoid : private sf::NonCopyable
{
public:
	Arkanoid(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
	void update(sf::Time dt);
	void draw();

	CommandQueue& getCommandQueue();

	bool hasLivesRemaining() const;
	bool hasPlayerReachedEnd() const;
	bool hasClearStage() const;
	bool hasBallsRemaining() const;

	void setNextStage(sf::Time dt);
	void setLostBallScene(sf::Time dt);

	// Power ups calls
	void resetBlockCounter();
	void earnExtraLife();
	void slowBall();
	void setVausMode(Vaus::Mode mode);
	void multiBalls(int number);

private:
	// Setup methods
	void loadTextures();
	void buildScene();
	void setBackgroundSprite();
	void setFieldSprite();
	void setStatisticsText();

	void generateStage(bool isRandom);
	void addBlock(Block::Color color, float relX, float relY);

	// Entities management
	void adaptVausPosition();
	void adaptBallsPosition();
	void adaptEnnemiesPosition();
	void destroyEntitiesOutsideView();
	void spawnEnemy(sf::Time dt);
	void spawnNewBall();

	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);
	void handleCollisions();

	void updateTexts();
	void updateBackgroundColor();
	void updateSounds();

	sf::FloatRect getFieldBounds() const;

private:
	enum Layer
	{
		Background,
		Main,
		Upper,
		LayerCount
	};

private:
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mWorldView;
	sf::FloatRect mWorldBounds;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	CommandQueue mCommandQueue;

	// Nodes trackers
	Vaus* mPlayerVaus;
	std::vector<Projectile*> mBalls;
	std::vector<Enemy*> mEnemies;
	SpriteNode* mBackgroundSprite;
	TextNode* mPlayerScore;
	TextNode* mPlayerLives;
	Trapdoor* mLeftTrapdoor;
	Trapdoor* mRightTrapdoor;

	sf::Vector2f mSpawnPosition;
	sf::Time mElapsedTime;

	int mBlocksRemaining;
	int mScore;
	int mLives;
	int mStage;
};

#endif // ARKANOID_HPP