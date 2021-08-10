#ifndef ANIMATED_BUTTON_HPP
#define ANIMATED_BUTTON_HPP

#include "button.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace GUI
{
	class AnimatedButton : public Button
	{
	public:
		enum Identifier
		{
			ArkanoidMenu,
			QuitMenu,
			IdentifierCount
		};

	public:
		AnimatedButton(Identifier id, State::Context context);

		virtual void select() override;
		virtual void activate() override;

		virtual void update(sf::Time dt) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		virtual void changeTexture(Type buttonType) override;
		void resetTransformations();

	private:
		sf::Sprite mSprite;
		Identifier mIdentifier;
	};
}

#endif // !ANIMATED_BUTTON_HPP