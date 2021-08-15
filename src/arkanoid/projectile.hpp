#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "entity.hpp"
#include "graphics/animation.hpp"
#include "resources/resource_identifiers.hpp"

class Projectile : public Entity
{
public:
	enum Type
	{
		Ball,
		Bullet,
		TypeCount
	};
public:
	Projectile(Type type, const TextureHolder& textures, Grid* grid = nullptr);

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override;
	virtual bool isMarkedForRemoval() const override;
	virtual void remove() override;

	float getMaxSpeed() const;
	int getDamage() const;
	Type getType() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Type mType;
	sf::Sprite mSprite;
	Animation mDestruction;

	bool mShowDestruction;
};

#endif // PROJECTILE_HPP