#ifndef VAUS_HPP
#define VAUS_HPP

#include "entity.hpp"
#include "commands/command.hpp"
#include "graphics/animation.hpp"
#include "resources/resource_identifiers.hpp"
#include "resources/resource_holder.hpp"

class Vaus : public Entity
{
public:
	enum Mode
	{
		Normal,
		Catch,
		Laser,
		Enlarge,
		ModeCount
	};

public:
	Vaus(Mode mode, const TextureHolder& textures, Grid* grid = nullptr);

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override;
	float getMaxSpeed() const;

	void fire();
	void setMode(Mode mode);
	Vaus::Mode getMode() const;
	void attachBall() noexcept;
	bool isBallAttached() const noexcept;
	void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void createProjectile(SceneNode& node, float xOffset, float yOffset, const TextureHolder& textures) const;

private:
	Mode mMode;
	Animation mSpriteAnimation;
	Command mFireCommand;
	sf::Time mFireCountdown;
	Grid* mGrid;

	bool mIsFiring;
	bool mIsBallAttached;
	bool mHasLaunchedBall;
};

#endif // VAUS_HPP