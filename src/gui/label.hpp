#ifndef LABEL_HPP
#define LABEL_HPP

#include "gui/component.hpp"
#include "resources/resource_identifiers.hpp"
#include "resources/resource_holder.hpp"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class Label : public Component
	{
	public:
		typedef std::shared_ptr<Label> Ptr;

	public:
		Label(const std::string& text, const FontHolder& fonts);

		virtual bool isSelectable() const override;
		void setText(const std::string& text);

		virtual void handleEvent(const sf::Event& event) override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Text mText;
	};
}

#endif // LABEL_HPP