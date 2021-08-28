#include "utility.hpp"
#include "entity.hpp"
#include "graphics/animation.hpp"
#include "arkanoid/block.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <cassert>
#include <algorithm>

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

float dotProduct(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
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
	// "Pseudo" Circle-AABB collision resolution
	// We assume the origin of both entities is centered

	sf::Vector2f const distance{ bouncingEntity.getPosition() - entity.getPosition() };
	sf::Vector2f const halfLength(entity.getBoundingRect().width / 2.0f, entity.getBoundingRect().height / 2.0f);

	// Find closest point on the entity's rectangle
	float const clampedX{ std::clamp(distance.x, -halfLength.x, halfLength.x) };
	float const clampedY{ std::clamp(distance.y, -halfLength.y, halfLength.y) };
	sf::Vector2f const closestPoint{ entity.getPosition() + sf::Vector2f(clampedX, clampedY) };

	// Find how much the bouncingEntity has penetrated in the entity
	sf::Vector2f const penetration{ closestPoint - bouncingEntity.getPosition() };

	// Find the best normal vector that is "similar" to the penetration vector
	std::vector<sf::Vector2f> normals{ {1.0f, 0.0f},   // Right
									   {0.0f, -1.0f},  // Top
									   {-1.0f, 0.0f},  // Left
									   {0.0f, 1.0f} }; // Bottom

	// Find the best direction where the hit occurs
	int direction{ -1 };
	float max{ 0.0f };
	for (int i{ 0 }; i < normals.size(); ++i)
	{
		float const vectProd = dotProduct(penetration, normals[i]);
		if (vectProd > max)
		{
			max = vectProd;
			direction = i;
		}
	}

	if (direction == 0 || direction == 2)
	{
		// Left or right hit: flip horizontal velocity
		bouncingEntity.setVelocity(-bouncingEntity.getVelocity().x, bouncingEntity.getVelocity().y);

		// Relocate the boucingEntity with an extra distance
		float offset{ (bouncingEntity.getBoundingRect().width / 2.0f) - std::abs(penetration.x) };
		if (direction == 2)
			bouncingEntity.move(offset, 0);
		else
			bouncingEntity.move(-offset, 0);
	}
	else
	{
		// Top or bottom hit: flip vertical velocity
		bouncingEntity.setVelocity(bouncingEntity.getVelocity().x, -bouncingEntity.getVelocity().y);

		float offset{ (bouncingEntity.getBoundingRect().width / 2.0f) - std::abs(penetration.y) };
		if (direction == 1)
			bouncingEntity.move(0, offset);
		else
			bouncingEntity.move(0, -offset);
	}
}