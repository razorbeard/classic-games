#ifndef POWER_UP_HPP
#define POWER_UP_HPP

#include "entity.hpp"
#include "graphics/animation.hpp"
#include "resources/resource_identifiers.hpp"

class Arkanoid;

class PowerUp : public Entity
{
public:
	enum Type
	{
		Laser,
		Enlarge,
		Catch,
		Slow,
		Break,
		Disruption,
		Player,
		TypeCount
	};

public:
	PowerUp(Type type, const TextureHolder& textures);

	virtual bool isMarkedForRemoval() const override;
	void apply(Arkanoid& world) const;

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Type mType;
	bool mRemovalFlag;
	Animation mSpriteAnimation;
};

#endif // POWER_UP_HPP