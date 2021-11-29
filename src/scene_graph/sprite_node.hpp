#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "scene_graph/scene_node.hpp"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

	void setColor(sf::Color);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite mSprite;
};

#endif // SPRITE_NODE_HPP