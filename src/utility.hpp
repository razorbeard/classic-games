#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Shape;
	class Sprite;
	class Text;
	class CircleShape;
	class RectangleShape;
}

class Animation;
class Entity;

// Call setOrigin() with the center of the object
void centerOrigin(sf::Shape& shape);
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

// Degree/radian conversion
float toDegree(float radian);
float toRadian(float degree);

// Random number generation
int randomInt(int exclusiveMax);
float randomFloat(int exclusiveMin, int exclusiveMax);

// Vector operations
float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

// Collision detection
bool isColliding(const sf::CircleShape& a, const sf::CircleShape& b);
bool isColliding(const sf::RectangleShape& a, const sf::CircleShape& b);

void adaptVelocityOnHit(Entity& bouncingEntity, const Entity& entity);

#endif // UTILITY_HPP