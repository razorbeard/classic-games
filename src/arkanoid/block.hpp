#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "entity.hpp"
#include "commands/command.hpp"
#include "resources/resource_identifiers.hpp"
#include "graphics/animation.hpp"

class Block : public Entity
{
public:
	enum Color
	{
		White,
		Orange,
		Cyan,
		Green,
		Red,
		Blue,
		Violet,
		Yellow,
		Silver,
		Gold,
		ColorCount
	};

public:
	Block(Color color, const TextureHolder& textures, Grid* grid = nullptr);

	virtual bool isMarkedForRemoval() const override;
	virtual void remove() override;

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override;

	void canDropPowerUp(bool flag) noexcept;

	int getValue() const;
	Color getColor() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void createPowerUp(SceneNode& node, const TextureHolder& textures) const;
	void checkPowerUpDrop(CommandQueue& commands);

private:
	Color mColor;
	Animation mSpriteAnimation;
	Command mDropPowerUpCommand;
	Grid* mGrid;
	int mOldHitpoints;
	bool mCanDropPowerUp;
	bool mSpawnedPowerUp;
	bool mShowShinning;
	bool mRemovalFlag;
};

#endif // BLOCK_HPP