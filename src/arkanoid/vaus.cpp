#include "vaus.hpp"
#include "data_tables.hpp"
#include "utility.hpp"
#include "commands/command_queue.hpp"
#include "scene_graph/sound_node.hpp"
#include "scene_graph/grid.hpp"
#include "projectile.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<VausData> Table = initializeVausData();
}

Vaus::Vaus(Mode mode, const TextureHolder& textures, Grid* grid)
	: Entity(1, grid)
	, mMode(mode)
	, mSpriteAnimation(textures.get(Table[mode].texture), Table[mode].textureRect)
	, mFireCommand()
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsBallAttached(true)
	, mHasLaunchedBall(false)
	, mGrid(grid)
{
	mSpriteAnimation.setFrameSize(Table[mode].frameSize);
	mSpriteAnimation.setNumFrames(Table[mode].numFrames);
	mSpriteAnimation.setDuration(Table[mode].duration);
	mSpriteAnimation.setSpacing(Table[mode].spacing);
	mSpriteAnimation.setRepeating(true);

	centerOrigin(mSpriteAnimation);

	mFireCommand.category = Category::DefaultLayer;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, -0.3f, -1.0f, textures);
		createProjectile(node, 0.3f, -1.0f, textures);
	};
}

void Vaus::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSpriteAnimation, states);
}

void Vaus::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Check if bullets or balls are fired
	checkProjectileLaunch(dt, commands);

	mSpriteAnimation.update(dt);
	Entity::updateCurrent(dt, commands);
}

void Vaus::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f const worldPosition{ getWorldPosition() };

	Command command{};
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition](SoundNode& node, sf::Time)
		{
			node.playSound(effect, worldPosition);
		});

	commands.push(command);
}

void Vaus::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Check for automatic laser fire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: we can fire new bullets
		commands.push(mFireCommand);

		playLocalSound(commands, SoundEffect::VausLaserFire);

		mFireCountdown += sf::seconds(0.5f); // Fire interval to adjust
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}
	else if (mHasLaunchedBall)
	{
		int const orientation = getVelocity().x / getMaxSpeed();
		sf::Transform rotation;

		if (orientation == 0)
			rotation.rotate(randomFloat(-60.0f, -30.0f));
		else if (orientation < 0)
			rotation.rotate(-90.0f);

		// Launch the ball from the Vaus
		Command command{};
		command.category = Category::Projectile;
		command.action = derivedAction<Projectile>([=](Projectile& node, sf::Time)
												   {
													   if (node.getType() == Projectile::Ball)
														   node.accelerate(rotation.transformPoint(node.getMaxSpeed(), -node.getMaxSpeed()));
												   });

		commands.push(command);
		playLocalSound(commands, SoundEffect::VausBallHit);

		mHasLaunchedBall = false;
	}
}

void Vaus::createProjectile(SceneNode& node, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile{ new Projectile(Projectile::Bullet, textures, mGrid) };
	sf::Vector2f const offset{ xOffset * mSpriteAnimation.getGlobalBounds().width, yOffset * mSpriteAnimation.getGlobalBounds().height };
	sf::Vector2f const velocity{ 0.0f, projectile->getMaxSpeed() };

	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(-velocity);
	mGrid->insert(projectile.get());

	node.attachChild(std::move(projectile));
}

unsigned int Vaus::getCategory() const
{
	return Category::PlayerVaus;
}

void Vaus::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (mMode == Laser && sf::seconds(0.5f) != sf::Time::Zero)
		mIsFiring = true;

	// Ready to detach and launch the ball
	if (mIsBallAttached)
	{
		mHasLaunchedBall = true;
		mIsBallAttached = false;
	}
}

void Vaus::setMode(Mode mode)
{
	mMode = mode;
	mSpriteAnimation.setTextureRect(Table[mode].textureRect);

	mSpriteAnimation.setFrameSize(Table[mode].frameSize);
	mSpriteAnimation.setNumFrames(Table[mode].numFrames);
	mSpriteAnimation.setDuration(Table[mode].duration);
	mSpriteAnimation.setSpacing(Table[mode].spacing);
	mSpriteAnimation.setRepeating(true);

	mSpriteAnimation.restart();

	centerOrigin(mSpriteAnimation);
}

Vaus::Mode Vaus::getMode() const
{
	return mMode;
}

void Vaus::attachBall() noexcept
{
	mHasLaunchedBall = false;
	mIsBallAttached = true;
}

bool Vaus::isBallAttached() const noexcept
{
	return mIsBallAttached;
}

float Vaus::getMaxSpeed() const
{
	return Table[mMode].speed;
}

sf::FloatRect Vaus::getBoundingRect() const
{
	// Gives the entity's shape (hitbox for collision detection)
	// Take into account any transformation (rotation mainly)
	return getWorldTransform().transformRect(mSpriteAnimation.getGlobalBounds());
}