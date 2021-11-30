#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "state_stack/state.hpp"
#include "gui/container.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	sf::CircleShape mCircle;
	sf::Time mScaleEffectTime;
	sf::Time mTransitionTime;
	sf::Sprite mLargeRingBackground;
	sf::Sprite mSmallRingBackground;
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	States::ID mNextState;
	bool mHasPressedKey;
};

#endif // MENU_STATE_HPP