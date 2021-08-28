#include "arkanoid.hpp"
#include "data_tables.hpp"
#include "power_up.hpp"
#include "scene_graph/text_node.hpp"
#include "scene_graph/sprite_node.hpp"
#include "scene_graph/sound_node.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include "trapdoor.hpp"
#include "audio/sound_player.hpp"
#include "utility.hpp"

#include <algorithm>
#include <cmath>

namespace
{
	const std::vector<std::vector<int>> StageTable = initializeStageData();
}

Arkanoid::Arkanoid(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(outputTarget)
	, mSceneTexture()
	, mWorldView(outputTarget.getDefaultView())
	, mTextures()
	, mFonts(fonts)
	, mSounds(sounds)
	, mSceneGraph()
	, mSceneLayers()
	, mGrid(mWorldView.getSize().x, mWorldView.getSize().y + 100.0f, 100.0f)
	, mBackgroundSprite(nullptr)
	, mPlayerScore(nullptr)
	, mPlayerLives(nullptr)
	, mWorldBounds(0.0f, 0.0f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mSpawnPosition(mWorldView.getSize().x / 2.0f, mWorldView.getSize().y * 0.95f)
	, mElapsedTime(sf::Time::Zero)
	, mPlayerVaus(nullptr)
	, mBlocksRemaining(0)
	, mScore(0)
	, mLives(3)
	, mStage(0)
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

	loadTextures();
	buildScene();
}

void Arkanoid::loadTextures()
{
	// To load tilesets, we need a call for every tile since the method
	// sf::Texture::setRepeated() is not available through sf::Sprite
	mTextures.load(Textures::Arkanoid, "resources/textures/arkanoid.png");
	mTextures.load(Textures::BackgroundTile, "resources/textures/arkanoid.png", sf::IntRect(287, 0, 48, 48));
	mTextures.load(Textures::PipeBackground, "resources/textures/pipe_background.png");
	mTextures.load(Textures::Particle, "resources/textures/particles.png");
}

void Arkanoid::update(sf::Time dt)
{
	mElapsedTime += dt;
	mPlayerVaus->setVelocity(0.0f, 0.0f); // Avoid "snow" effect movement

	destroyEntitiesOutsideView();

	// Forward commands to scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	handleCollisions();

	mSceneGraph.removeWrecks();

	// Regular update step, edit vaus and ball(s) positions
	mSceneGraph.update(dt, mCommandQueue);
	adaptVausPosition();
	adaptBallsPosition();
	adaptEnnemiesPosition();

	spawnEnemy(dt);

	updateBackgroundColor();
	updateTexts();
	updateSounds();
}

void Arkanoid::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(mSceneGraph);
}

void Arkanoid::updateSounds()
{
	mSounds.setListenerPosition(mPlayerVaus->getWorldPosition());
	mSounds.removeStoppedSounds(); // Remove unused sounds
}

void Arkanoid::adaptVausPosition()
{
	sf::FloatRect const fieldBounds{ getFieldBounds() };
	sf::Vector2f position{ mPlayerVaus->getPosition() };
	float const vausLength{ mPlayerVaus->getBoundingRect().width / 2.0f };

	// Keep player's position inside the field bounds
	position.x = std::max(position.x, fieldBounds.left + vausLength);
	position.x = std::min(position.x, fieldBounds.left + fieldBounds.width - vausLength);

	mPlayerVaus->setPosition(position);
}

void Arkanoid::adaptBallsPosition()
{
	// Check if the ball container is empty before pursuing
	if (mBalls.empty())
		return;

	sf::FloatRect const fieldBounds{ getFieldBounds() };
	float const ballRadius{ mBalls.back()->getBoundingRect().width / 2.0f };

	// Follow Vaus movement on the x-axis when attached
	if (mPlayerVaus->isBallAttached())
		mBalls.back()->setPosition(mPlayerVaus->getPosition().x, mBalls.back()->getPosition().y);

	// Keep the balls inside the defined field
	for (Projectile* ball : mBalls)
	{
		if (ball->getPosition().x - ballRadius < fieldBounds.left)
			ball->setVelocity(std::abs(ball->getVelocity().x), ball->getVelocity().y);

		else if (ball->getPosition().x + ballRadius > fieldBounds.width + fieldBounds.left)
			ball->setVelocity(-std::abs(ball->getVelocity().x), ball->getVelocity().y);

		else if (ball->getPosition().y - ballRadius < fieldBounds.top)
			ball->setVelocity(ball->getVelocity().x, std::abs(ball->getVelocity().y));
	}
}

void Arkanoid::adaptEnnemiesPosition()
{
	// Check if there's any ennemies remaining before pursuing
	if (mEnemies.empty())
		return;

	sf::FloatRect const fieldBounds{ getFieldBounds() };
	float const enemyRadius{ mEnemies.back()->getBoundingRect().width / 2.0f };

	// Keep the enemies inside the defined field
	for (Enemy* enemy : mEnemies)
	{
		if (enemy->getPosition().x - enemyRadius < fieldBounds.left)
			enemy->setVelocity(-enemy->getVelocity().x, enemy->getVelocity().y);

		else if (enemy->getPosition().x + enemyRadius > fieldBounds.width + fieldBounds.left)
			enemy->setVelocity(-enemy->getVelocity().x, enemy->getVelocity().y);

		else if (enemy->getPosition().y - enemyRadius < fieldBounds.top)
			enemy->setVelocity(enemy->getVelocity().x, -enemy->getVelocity().y);
	}
}

void Arkanoid::buildScene()
{
	for (std::size_t i{ 0 }; i < LayerCount; ++i)
	{
		// Set a category for each node
		Category::Type category = (i == Main) ? Category::DefaultLayer : Category::None;

		SceneNode::Ptr layer{ new SceneNode{category} };
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	setBackgroundSprite();
	setFieldSprite();
	setStatisticsText();

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode{ new SoundNode{mSounds} };
	mSceneGraph.attachChild(std::move(soundNode));
}

void Arkanoid::setBackgroundSprite()
{
	sf::Texture& texture{ mTextures.get(Textures::BackgroundTile) };
	sf::IntRect textureRect{ mWorldBounds };
	textureRect.height -= 84;
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite{ new SpriteNode{texture, textureRect} };
	mBackgroundSprite = backgroundSprite.get();
	backgroundSprite->setPosition(0.0f, 84.0f);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
}

void Arkanoid::setFieldSprite()
{
	std::unique_ptr<SpriteNode> pipeBackground{ new SpriteNode{mTextures.get(Textures::PipeBackground)} };
	pipeBackground->setPosition(0.0f, 84.0f);
	mSceneLayers[Main]->attachChild(std::move(pipeBackground));

	// Set the trapdoors : enemies spawns from there
	std::unique_ptr<Trapdoor> leftTrapdoor{ new Trapdoor(mTextures) };
	mLeftTrapdoor = leftTrapdoor.get();
	leftTrapdoor->setPosition(251.0f, 84.0f);
	mSceneLayers[Main]->attachChild(std::move(leftTrapdoor));

	std::unique_ptr<Trapdoor> rightTrapdoor{ new Trapdoor(mTextures) };
	mRightTrapdoor = rightTrapdoor.get();
	rightTrapdoor->setPosition(683.0f, 84.0f);
	mSceneLayers[Main]->attachChild(std::move(rightTrapdoor));
}

void Arkanoid::setStatisticsText()
{
	std::unique_ptr<TextNode> scoreText{ new TextNode{mFonts, Fonts::EightBitArcade, "Score" , 64} };
	scoreText->setPosition(mWorldView.getSize().x / 5.0f, 22.0f);
	mSceneLayers[Background]->attachChild(std::move(scoreText));

	std::unique_ptr<TextNode> livesText{ new TextNode{mFonts, Fonts::EightBitArcade, "Lives" , 64} };
	livesText->setPosition(mWorldView.getSize().x / 2.0f, 22.0f);
	mSceneLayers[Background]->attachChild(std::move(livesText));

	std::unique_ptr<TextNode> actionText{ new TextNode{mFonts, Fonts::EightBitArcade, "Action" , 64} };
	actionText->setPosition(mWorldView.getSize().x - mWorldView.getSize().x / 5.0f, 22.0f);
	mSceneLayers[Background]->attachChild(std::move(actionText));

	std::unique_ptr<TextNode> score{ new TextNode{mFonts, Fonts::EightBitArcade, std::to_string(mScore) , 64} };
	mPlayerScore = score.get();
	score->setColor(sf::Color(172, 50, 50));
	score->setPosition(mWorldView.getSize().x / 5.0f, 54.0f);
	mSceneLayers[Background]->attachChild(std::move(score));

	std::unique_ptr<TextNode> lives{ new TextNode{mFonts, Fonts::EightBitArcade, std::to_string(mLives) , 64} };
	mPlayerLives = lives.get();
	lives->setColor(sf::Color(172, 50, 50));
	lives->setPosition(mWorldView.getSize().x / 2.0f, 54.0f);
	mSceneLayers[Background]->attachChild(std::move(lives));

	std::unique_ptr<TextNode> action{ new TextNode{mFonts, Fonts::EightBitArcade, "Space Bar" , 64} };
	action->setColor(sf::Color(172, 50, 50));
	action->setPosition(mWorldView.getSize().x - mWorldView.getSize().x / 5.0f, 54.0f);
	mSceneLayers[Background]->attachChild(std::move(action));
}

void Arkanoid::generateStage(bool isRandom = false)
{
	sf::FloatRect const bounds{ getFieldBounds() };

	// Blocks should have the same size
	int blockWidth{ 58 };
	int blockHeight{ 32 };

	// Define the block layout
	// NB: modify nxBlocks and nyBlocks to see a fps impact due to the collision testing method
	int nxBlocks{ 14 };
	int nyBlocks{ 5 };
	assert(StageTable[mStage - 1].size() == nxBlocks * nyBlocks);

	// Starting point coordinates, taken arbitrarly
	sf::Vector2f startPoint{ 135.0f, 210.0f };

	// Generate blocks by travelling along a subrectangle of the field bounds
	int counter{ 0 };
	for (float y{ startPoint.y }; y < (nyBlocks * blockHeight) + startPoint.y; y += blockHeight)
	{
		for (float x{ startPoint.x }; x < (nxBlocks * blockWidth) + startPoint.x; x += blockWidth)
		{
			// Known data: skip when a -1 value is found in the table
			if (!isRandom && StageTable[mStage - 1][counter] != -1)
			{
				addBlock(static_cast<Block::Color>(StageTable[mStage - 1][counter]), x, y);
			}

			// Random generation: sometimes we create empty spaces by skipping addBlock()
			else if (isRandom && randomInt(10) != 0)
			{
				addBlock(static_cast<Block::Color>(randomInt(Block::ColorCount)), x, y);
			}

			++counter;
		}
	}
}

void Arkanoid::addBlock(Block::Color color, float relX, float relY)
{
	// Add a block in accordance with the field bounds
	std::unique_ptr<Block> block{ new Block(color, mTextures, &mGrid) };
	block->setPosition(relX, relY);
	mGrid.insert(block.get());

	// Silver blocks : hitpoints increase by one every eight stages
	if (color == Block::Silver && mStage >= 8)
		block->repair(mStage / 8);

	// Gold blocks : do not count them for the remaining blocks
	if (!(color == Block::Gold))
		++mBlocksRemaining;

	mSceneLayers[Main]->attachChild(std::move(block));
}

bool Arkanoid::matchesCategories(Grid::EntityPair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1{ colliders.first->getCategory() };
	unsigned int category2{ colliders.second->getCategory() };

	// Make sure first pair entry has category type1 and second has type2
	// Swapping is needed to avoid type1 and type2 to influence the result
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void Arkanoid::handleCollisions()
{
	// Check every combinaison possible between pairs of nodes
	std::set<Grid::EntityPair> collisionPairs;
	mGrid.checkCollisions(collisionPairs);

	// Avoid the ball to collide with multiple blocks at the same time
	bool hasResolvedBallBlock{ false };

	// Interactions between pairs of nodes
	for (Grid::EntityPair pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerVaus, Category::Projectile))
		{
			auto& vaus = static_cast<Vaus&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			if (projectile.getType() == Projectile::Ball)
			{
				if (vaus.getMode() == Vaus::Catch)
				{
					projectile.setVelocity(0.0f, 0.0f);

					// Snap the ball's bottom to vaus' position
					sf::Vector2f const offset{ 0.0f, -(projectile.getBoundingRect().height + vaus.getBoundingRect().height) / 2.0f };
					projectile.setPosition(vaus.getPosition() + offset);
					vaus.attachBall();
				}

				float const distance{ projectile.getPosition().x - vaus.getPosition().x };
				float ratio{ (distance * 2.0f) / vaus.getBoundingRect().width };
				sf::Vector2f const oldVelocity{ projectile.getVelocity() };

				// Exclude interval [-0.3, 0.3] to avoid significant horizontal movement
				if (ratio <= 0.3f && ratio >= -0.3f)
					ratio = (ratio > 0 ? 1 : -1) * 0.3f;

				// Keep constant speed after modifying the horizontal component
				float constexpr boost{ 1.4f };
				projectile.setVelocity(projectile.getMaxSpeed() * ratio * boost, projectile.getVelocity().y);
				projectile.setVelocity(unitVector(projectile.getVelocity()) * length(oldVelocity));

				// To avoid an unknown behavior when the ball is stuck in the Vaus,
				// we always flip the y velocity with a positive value
				projectile.setVelocity(projectile.getVelocity().x, -std::abs(projectile.getVelocity().y));

				vaus.playLocalSound(mCommandQueue, SoundEffect::VausBallHit);
			}
		}

		else if (matchesCategories(pair, Category::PlayerVaus, Category::PowerUp))
		{
			auto& vaus = static_cast<Vaus&>(*pair.first);
			auto& powerUp = static_cast<PowerUp&>(*pair.second);

			powerUp.apply(*this);
			powerUp.destroy();
			vaus.playLocalSound(mCommandQueue, SoundEffect::CollectPowerUp);
		}

		else if (matchesCategories(pair, Category::Enemy, Category::Projectile))
		{
			auto& enemy = static_cast<Enemy&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			enemy.destroy();

			// Remove the ennemy from the container
			mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
						   [](Enemy* enemy) { return enemy->isDestroyed(); }), mEnemies.end());

			if (projectile.getType() == Projectile::Bullet)
				projectile.destroy();

			if (projectile.getType() == Projectile::Ball)
				adaptVelocityOnHit(projectile, enemy);

			mPlayerVaus->playLocalSound(mCommandQueue, SoundEffect::Explosion);
		}

		else if (matchesCategories(pair, Category::Block, Category::Projectile) && !hasResolvedBallBlock)
		{
			auto& block = static_cast<Block&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			block.damage(projectile.getDamage());
			mPlayerVaus->playLocalSound(mCommandQueue, SoundEffect::BlockHit);

			// Update the score: silver blocks value increase depending the stage number
			if (block.isDestroyed())
			{
				(block.getColor() == Block::Silver) ? mScore += block.getValue() * mStage : mScore += block.getValue();
				--mBlocksRemaining;
			}

			if (projectile.getType() == Projectile::Bullet)
				projectile.destroy();

			else if (projectile.getType() == Projectile::Ball)
			{
				adaptVelocityOnHit(projectile, block);
				hasResolvedBallBlock = true;
			}
		}
	}
}

void Arkanoid::destroyEntitiesOutsideView()
{
	// Destroy projectiles (bullets + balls), power ups and enemies if they're out of the field bounds
	Command command{};
	command.category = Category::Projectile | Category::PowerUp | Category::Enemy;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
										   {
											   if (!getFieldBounds().intersects(e.getBoundingRect()))
												   e.remove();

											   // Clean the ball container before any updates
											   if (e.getCategory() == Category::Projectile)
											   {
												   mBalls.erase(std::remove_if(mBalls.begin(), mBalls.end(),
																[](Projectile* ball) { return ball->isDestroyed(); }), mBalls.end());
											   }

											   // Clean the enemies container also
											   if (e.getCategory() == Category::Enemy)
											   {
												   mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
																  [](Enemy* enemy) { return enemy->isDestroyed(); }), mEnemies.end());
											   }
										   });

	mCommandQueue.push(command);
}

void Arkanoid::updateTexts()
{
	mPlayerScore->setString(std::to_string(mScore));
	mPlayerLives->setString(std::to_string(mLives));
}

void Arkanoid::updateBackgroundColor()
{
	// Oscillate the alpha value of the sprite between 0 and 255
	int const alphaValue{ (int)((255 / 2) * std::cos(mElapsedTime.asSeconds()) + (255 / 2)) };
	mBackgroundSprite->setColor(sf::Color(255, 255, 255, alphaValue));
}

void Arkanoid::setVausMode(Vaus::Mode mode)
{
	mPlayerVaus->setMode(mode);
}

void Arkanoid::spawnEnemy(sf::Time dt)
{
	static sf::Time timer{ sf::Time::Zero };
	timer += dt;

	// Each ~10s elapsed, pick a random integer to check if an enemy can spawn from a trapdoor
	// Maximum 5 enemies allowed on the screen; spawn rate can be definied also base on the stage number
	if (mEnemies.size() <= 5 && timer > sf::seconds(10.0f))
	{
		for (auto& trapdoor : { mLeftTrapdoor, mRightTrapdoor })
		{
			if (randomInt(1) == 0)
			{
				trapdoor->activate();

				// Take a random ennemy
				std::unique_ptr<Enemy> enemy{ new Enemy{static_cast<Enemy::Type>(randomInt(Enemy::TypeCount)), mTextures, &mGrid} };

				// Shift the spawn position a bit to the left, due to the shadow from the trapdoor's sprite
				int const orientation{ randomInt(2) == 0 ? 1 : -1 };
				enemy->setPosition(trapdoor->getPosition().x + 45.0f, trapdoor->getPosition().y + 50.0f);
				enemy->setVelocity(orientation * enemy->getMaxSpeed(), enemy->getMaxSpeed());
				mGrid.insert(enemy.get());

				mEnemies.push_back(enemy.get());
				mSceneLayers[Main]->attachChild(std::move(enemy));
			}
		}

		timer = sf::Time::Zero;
	}
}

void Arkanoid::spawnNewBall()
{
	std::unique_ptr<Projectile> ball{ new Projectile{Projectile::Ball, mTextures, &mGrid} };

	// Snap the ball's bottom to vaus' position
	sf::Vector2f const offset{ 0.0f, -(ball->getBoundingRect().height + mPlayerVaus->getBoundingRect().height) / 2.0f };
	ball->setPosition(mPlayerVaus->getPosition() + offset);
	mGrid.insert(ball.get());

	mBalls.push_back(ball.get());
	mSceneLayers[Main]->attachChild(std::move(ball));

	// Connect the ball to the Vaus
	mPlayerVaus->attachBall();
}

void Arkanoid::multiBalls(int number)
{
	// "Disruption" power up: add any amount of balls to the scene graph
	for (int i{ 0 }; i < number; ++i)
	{
		std::unique_ptr<Projectile> ball{ new Projectile{Projectile::Ball, mTextures, &mGrid} };
		ball->setPosition(mBalls.back()->getPosition());
		mGrid.insert(ball.get());

		// Spawn the new ball, by rotating the first ball's velocity vector between -45 and 45 degrees
		float const angle{ toRadian(randomFloat(-45, 45)) };

		float const xVel{ mBalls.front()->getVelocity().x };
		float const yVel{ mBalls.front()->getVelocity().y };
		ball->setVelocity(xVel * std::cos(angle) - std::sin(angle) * yVel,
						  xVel * std::sin(angle) + std::cos(angle) * yVel);
		mBalls.push_back(ball.get());
		mSceneLayers[Main]->attachChild(std::move(ball));
	}
}

void Arkanoid::earnExtraLife()
{
	++mLives;
}

void Arkanoid::slowBall()
{
	// Only one ball is affected by the power up "Disruption",
	// since it disable others powers up
	sf::Vector2f velocity{ mBalls.back()->getVelocity() };
	mBalls.back()->setVelocity(velocity / 2.0f);
}

void Arkanoid::resetBlockCounter()
{
	// Used when the player get the "Break" power-up and in setting the next stage
	mBlocksRemaining = 0;
}

void Arkanoid::setNextStage(sf::Time dt)
{
	++mStage;

	// Remove all the remaining blocks, projectiles, enemies or power ups in the scene
	Command command{};
	command.category = Category::PowerUp | Category::Projectile | Category::Block | Category::Enemy;
	command.action = derivedAction<Entity>([this](Entity& entity, sf::Time)
										   {
											   entity.remove();
										   });
	mCommandQueue.push(command);

	// Execute any remaining commands
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	// Remove any nodes marked for removal (especially blocks)
	mSceneGraph.removeWrecks();

	// Add player's vaus if it doesn't already exist
	if (!mPlayerVaus)
	{
		std::unique_ptr<Vaus> player{ new Vaus{Vaus::Normal, mTextures, &mGrid} };
		mPlayerVaus = player.get();
		mSceneLayers[Main]->attachChild(std::move(player));
	}

	// Reset Vaus status and positioning
	mPlayerVaus->setMode(Vaus::Normal);
	mPlayerVaus->setPosition(mSpawnPosition);
	mGrid.insert(mPlayerVaus);

	// Clear the balls container, add a new ball
	mBalls.clear();
	spawnNewBall();

	// Clear the enemies container
	mEnemies.clear();

	resetBlockCounter();
	generateStage();
}

void Arkanoid::setLostBallScene(sf::Time dt)
{
	--mLives;

	// Remove all projectiles, enemies and power ups in the scene
	Command command{};
	command.category = Category::PowerUp | Category::Projectile | Category::Enemy;
	command.action = derivedAction<Entity>([this](Entity& entity, sf::Time)
										   {
											   entity.remove();
										   });
	mCommandQueue.push(command);

	// Execute any remaining commands
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	// Reset Vaus status and positioning
	mPlayerVaus->setMode(Vaus::Normal);
	mPlayerVaus->setPosition(mSpawnPosition);

	// Clear the enemies container
	mEnemies.clear();

	spawnNewBall();
}

bool Arkanoid::hasClearStage() const
{
	// The player has clear the stage when there is no more blocks (except gold ones)
	return mBlocksRemaining == 0;
}

bool Arkanoid::hasBallsRemaining() const
{
	return mBalls.empty();
}

bool Arkanoid::hasLivesRemaining() const
{
	return mLives > 0;
}

bool Arkanoid::hasPlayerReachedEnd() const
{
	return mStage == 11;
}

CommandQueue& Arkanoid::getCommandQueue()
{
	return mCommandQueue;
}

sf::FloatRect Arkanoid::getFieldBounds() const
{
	float const pipeTileWidth{ 22.0f };
	float const pipeTileHeight{ 22.0f };

	// Returns the limits where the action takes place
	sf::FloatRect const bounds(pipeTileWidth,
							   pipeTileHeight + 84.0f,
							   mWorldBounds.width - 2 * pipeTileWidth,
							   mWorldBounds.height - 84.0f - pipeTileHeight);

	return bounds;
}