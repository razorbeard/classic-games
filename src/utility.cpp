#include "utility.hpp"
#include "entity.hpp"

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
	float dotProdMax{ 0.0f };
	for (int i{ 0 }; i < normals.size(); ++i)
	{
		float const vectProd = dotProduct(penetration, normals[i]);
		if (vectProd > dotProdMax)
		{
			dotProdMax = vectProd;
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