#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "resources/resource_identifiers.hpp"

#include <vector>
#include <array>
#include <unordered_map>

class Tetromino : public sf::Drawable
{
public:
	enum class Rotation
	{
		None,
		Clockwise,
		AntiClockwise
	};

public:
	Tetromino(int id, std::unordered_map<int, sf::Sprite>* spriteContainer);

	void applyRotation(Rotation rotation);
	void shift(int x, int y);

	std::array<sf::Vector2i, 4> getLocalCoordinates() const;
	sf::Vector2i getCoordinates() const;
	void setCoordinates(int x, int y);
	void setOffset(sf::Vector2i offset);
	int getId() const;

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::unordered_map<int, sf::Sprite>* mSpriteContainer;
	sf::Vector2i mCoordinates;
	sf::Vector2i mOffset;
	int mId;
	std::pair<int, std::array<sf::Vector2i, 4>> mSquaresInfo;
};

#endif // TETROMINO_HPP