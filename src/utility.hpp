#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class CircleShape;
	class RectangleShape;
}

class Entity;

template <typename Object>
void centerOrigin(Object& object)
{
	sf::FloatRect bounds{ object.getLocalBounds() };
	object.setOrigin(std::floor(bounds.left + bounds.width / 2.0f),
		std::floor(bounds.top + bounds.height / 2.0f));
}

float toDegree(float radian);
float toRadian(float degree);

int randomInt(int exclusiveMax);
float randomFloat(int exclusiveMin, int exclusiveMax);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

void adaptVelocityOnHit(Entity& bouncingEntity, const Entity& entity);

#endif // UTILITY_HPP