#include "block.hpp"
#include "data_tables.hpp"
#include "resources/resource_holder.hpp"
#include "utility.hpp"
#include "power_up.hpp"
#include "commands/command_queue.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<BlockData> Table = initializeBlockData();
}

Block::Block(Color color, const TextureHolder& textures, Grid* grid)
	: Entity(Table[color].hitpoints, grid)
	, mColor(color)
	, mSpriteAnimation(textures.get(Table[color].texture), Table[color].textureRect)
	, mOldHitpoints(Table[color].hitpoints)
	, mRemovalFlag(false)
	, mShowShinning(false)
	, mCanDropPowerUp(true)
	, mSpawnedPowerUp(false)
	, mDropPowerUpCommand()
	, mGrid(grid)
{
	mSpriteAnimation.setFrameSize(Table[color].frameSize);
	mSpriteAnimation.setNumFrames(Table[color].numFrames);
	mSpriteAnimation.setDuration(Table[color].duration);
	mSpriteAnimation.setSpacing(Table[color].spacing);

	mDropPowerUpCommand.category = Category::DefaultLayer;
	mDropPowerUpCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPowerUp(node, textures);
	};

	mHitbox.setPointCount(4);
	sf::FloatRect const rect{ getBoundingRect() };

	mHitbox.setPoint(0, sf::Vector2f(rect.left, rect.top));
	mHitbox.setPoint(1, sf::Vector2f(rect.left, rect.top + rect.height));
	mHitbox.setPoint(2, sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
	mHitbox.setPoint(3, sf::Vector2f(rect.left + rect.width, rect.top));
}

void Block::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSpriteAnimation, states);
}

void Block::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Block has been destroyed : random power up drop and mark for removal
	// If not marked for removal, this step is not checked ! Careful
	if (isDestroyed())
	{
		checkPowerUpDrop(commands);

		return;
	}

	// The block has take a hit : replay the animation from start
	// Gives a shinning effect for gold and silver blocks
	if (mOldHitpoints > getHitpoints())
	{
		mSpriteAnimation.restart();
		mOldHitpoints = getHitpoints();
	}

	mSpriteAnimation.update(dt);
}

void Block::canDropPowerUp(bool flag) noexcept
{
	mCanDropPowerUp = flag;
}

void Block::checkPowerUpDrop(CommandQueue& commands)
{
	// A block has 1/8 chance to drop a power up
	if (randomInt(8) == 0 && !mRemovalFlag && mCanDropPowerUp)
	{
		// When a block has drop a power up, we cannot drop more power ups
		// until it is destroyed or catched by the Vaus
		Command command{};
		command.category = Category::Block;
		command.action = derivedAction<Block>([](Block& block, sf::Time)
											  {
												  block.canDropPowerUp(false);
											  });

		commands.push(command);
		commands.push(mDropPowerUpCommand);
	}

	mRemovalFlag = true;
}

void Block::remove()
{
	// Calling a remove on a block avoids the creation of a power up
	mRemovalFlag = true;
	Entity::remove();
}

bool Block::isMarkedForRemoval() const
{
	return isDestroyed() && mRemovalFlag;
}

unsigned int Block::getCategory() const
{
	return Category::Block;
}

int Block::getValue() const
{
	return Table[mColor].value;
}

Block::Color Block::getColor() const
{
	return mColor;
}

sf::FloatRect Block::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSpriteAnimation.getGlobalBounds());
}

std::vector<sf::Vector2f> Block::getHitboxPoints() const
{
	std::vector<sf::Vector2f> transformedPoints(mHitbox.getPointCount());

	for (std::size_t i = 0; i < mHitbox.getPointCount(); ++i)
	{
		transformedPoints[i] = getTransform().transformPoint(mHitbox.getPoint(i));
	}

	return transformedPoints;
}

void Block::createPowerUp(SceneNode& node, const TextureHolder& textures) const
{
	// Choose a random number between 0 and 99, then select a power up
	int const randomNum{ randomInt(100) };
	PowerUp::Type type{ PowerUp::Enlarge }; // By default

	if (randomNum <= 4) type = PowerUp::Break;
	else if (5 <= randomNum && randomNum <= 12) type = PowerUp::Player;
	else if (13 <= randomNum && randomNum <= 25) type = PowerUp::Disruption;
	else if (26 <= randomNum && randomNum <= 42) type = PowerUp::Catch;
	else if (43 <= randomNum && randomNum <= 59) type = PowerUp::Slow;
	else if (60 <= randomNum && randomNum <= 79) type = PowerUp::Laser;
	else if (80 <= randomNum && randomNum <= 99) type = PowerUp::Enlarge;

	std::unique_ptr<PowerUp> powerUp{ new PowerUp(type, textures, mGrid) };
	powerUp->setPosition(getWorldPosition());
	powerUp->setVelocity(0.0f, 200.0f);
	node.attachChild(std::move(powerUp));
}