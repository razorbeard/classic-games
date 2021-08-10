#include "utility.hpp"
#include "entity.hpp"
#include "graphics/animation.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <cassert>

namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed{ static_cast<unsigned long>(std::time(nullptr)) };
		return std::default_random_engine(seed);
	}

	auto RandomEngine{ createRandomEngine() };
}

void centerOrigin(sf::Shape& shape)
{
	sf::FloatRect bounds{ shape.getLocalBounds() };
	shape.setOrigin(std::floor(bounds.left + bounds.width / 2.0f),
					std::floor(bounds.top + bounds.height / 2.0f));
}

void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds{ sprite.getLocalBounds() };
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.0f),
					 std::floor(bounds.top + bounds.height / 2.0f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds{ text.getLocalBounds() };
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.0f),
				   std::floor(bounds.top + bounds.height / 2.0f));
}

void centerOrigin(Animation& animation)
{
	sf::FloatRect bounds{ animation.getLocalBounds() };
	animation.setOrigin(std::floor(bounds.left + bounds.width / 2.0f),
						std::floor(bounds.top + bounds.height / 2.0f));
}

float toDegree(float radian)
{
	return 180.0f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree)
{
	return 3.141592653589793238462643383f / 180.0f * degree;
}

int randomInt(int exclusiveMax)
{
	std::uniform_int_distribution<> distr{ 0, exclusiveMax - 1 };
	return distr(RandomEngine);
}

float randomFloat(int exclusiveMin, int exclusiveMax)
{
	std::uniform_real_distribution<float> distr(exclusiveMin, exclusiveMax);
	return distr(RandomEngine);
}

float length(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector)
{
	assert(vector != sf::Vector2f(0.0f, 0.0f));
	return vector / length(vector);
}

bool isColliding(const sf::CircleShape& a, const sf::CircleShape& b)
{
	return length(sf::Vector2f(a.getPosition().x - b.getPosition().x, a.getPosition().y - b.getPosition().y)) <= a.getRadius() + b.getRadius();
}

bool isColliding(const sf::RectangleShape& a, const sf::CircleShape& b)
{
	// To detect if a circle is overlapping a rectangle, we first find
	// the vector of the neareast point of the surface on the rectangle
	// to the middle of the circle.
	// Then, if the distance of this vector is less than the radius of the circle, theres a collision
	sf::Vector2f nearestPoint{}; // neareast point on the surface of the rect
	nearestPoint.x = std::max(a.getGlobalBounds().top, std::min(a.getGlobalBounds().top + a.getGlobalBounds().width, b.getPosition().x));
	nearestPoint.y = std::max(a.getGlobalBounds().left, std::min(a.getGlobalBounds().left + a.getGlobalBounds().height, b.getPosition().y));

	return length(sf::Vector2f(nearestPoint.x - b.getPosition().x, nearestPoint.y - b.getPosition().y)) <= b.getRadius();
}

void adaptVelocityOnHit(Entity& bouncingEntity, const Entity& entity)
{
	float const ballRadius{ bouncingEntity.getBoundingRect().width / 2.0f };

	float const ballTop{ bouncingEntity.getPosition().y - ballRadius };
	float const ballBottom{ bouncingEntity.getPosition().y + ballRadius };
	float const ballRight{ bouncingEntity.getPosition().x + ballRadius };
	float const ballLeft{ bouncingEntity.getPosition().x - ballRadius };

	// Careful : the origin of the blocks is not center
	float const blockTop{ entity.getPosition().y };
	float const blockBottom{ entity.getPosition().y + entity.getBoundingRect().height };
	float const blockRight{ entity.getPosition().x + entity.getBoundingRect().width };
	float const blockLeft{ entity.getPosition().x };

	float const leftDistance{ ballRight - blockLeft };
	float const rightDistance{ blockRight - ballLeft };
	float const topDistance{ ballBottom - blockTop };
	float const bottomDistance{ blockBottom - ballTop };

	bool const hasHitLeftSide{ abs(leftDistance) < abs(rightDistance) };
	bool const hasHitTopSide{ abs(topDistance) < abs(bottomDistance) };

	float const minOverlapX{ hasHitLeftSide ? leftDistance : rightDistance };
	float const minOverlapY{ hasHitTopSide ? topDistance : bottomDistance };

	if (abs(minOverlapX) < abs(minOverlapY))
		if (hasHitLeftSide)
			bouncingEntity.setVelocity(-bouncingEntity.getVelocity().x, bouncingEntity.getVelocity().y);
		else
			bouncingEntity.setVelocity(-bouncingEntity.getVelocity().x, bouncingEntity.getVelocity().y);
	else
		if (hasHitTopSide)
			bouncingEntity.setVelocity(bouncingEntity.getVelocity().x, -bouncingEntity.getVelocity().y);
		else
			bouncingEntity.setVelocity(bouncingEntity.getVelocity().x, -bouncingEntity.getVelocity().y);
}