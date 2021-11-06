#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "resources/resource_identifiers.hpp"

#include <vector>
#include <array>

class Tetromino : public sf::Transformable, public sf::Drawable
{
public:
	enum class Rotation
	{
		None,
		Clockwise,
		AntiClockwise
	};

public:
	Tetromino(int id, const TextureHolder& textures);

	std::array<sf::Vector2i, 4> getSquaresInfo() const;
	void applyRotation(Rotation rotation);
	sf::Sprite getSprite() const;

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int mId;
	std::pair<int, std::array<sf::Vector2i, 4>> mSquaresInfo;
	mutable sf::Sprite mSprite;
};

#endif // TETROMINO_HPP