#include "text_node.hpp"
#include "resources/resource_holder.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	mText.setFont(fonts.get(Fonts::Sansation));
	setString(text);
}

TextNode::TextNode(const FontHolder& fonts, Fonts::ID fontName, const std::string& text)
{
	mText.setFont(fonts.get(fontName));
	setString(text);
}

TextNode::TextNode(const FontHolder& fonts, Fonts::ID fontName, const std::string& text, unsigned int size)
{
	mText.setFont(fonts.get(fontName));
	mText.setCharacterSize(size);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
	centerOrigin(mText);
}

void TextNode::setColor(const sf::Color color)
{
	mText.setFillColor(color);
	centerOrigin(mText);
}