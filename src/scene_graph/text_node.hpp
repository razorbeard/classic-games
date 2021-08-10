#ifndef TEXT_NODE_HPP
#define TEXT_NODE_HPP

#include "scene_graph/scene_node.hpp"
#include "resources/resource_identifiers.hpp"

#include <SFML/Graphics/Text.hpp>

class TextNode : public SceneNode
{
public:
	explicit TextNode(const FontHolder& fonts, const std::string& text);
	TextNode(const FontHolder& fonts, Fonts::ID fontName, const std::string& text);
	TextNode(const FontHolder& fonts, Fonts::ID fontName, const std::string& text, unsigned int size);

	void setString(const std::string& text);
	void setColor(const sf::Color color);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Text mText;
};

#endif // TEXT_NODE_HPP