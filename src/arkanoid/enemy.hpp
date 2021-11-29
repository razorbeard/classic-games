#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"
#include "graphics/animation.hpp"
#include "resources/resource_identifiers.hpp"

class Enemy : public Entity
{
public:
	enum Type
	{
		Ghost,
		Skull,
		TypeCount
	};

public:
	Enemy(Type type, const TextureHolder& textures, Grid* grid = nullptr);

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override;
	float getMaxSpeed() const;

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;

	virtual bool isMarkedForRemoval() const override;

private:
	Type mType;
	Animation mSpriteAnimation;
	Animation mExplosion;

	bool mShowExplosion;
};

#endif // ENEMY_HPP