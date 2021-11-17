#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "state.hpp"
#include "gui/container.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <atomic>

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
	bool mThreadAvailable;
	States::ID mNextState;
	std::atomic_bool mStartTransition;

	sf::Sprite mLargeRingBackground;
	sf::Sprite mSmallRingBackground;
	sf::Sprite mBackgroundSprite;

	GUI::Container mGUIContainer;
};

#endif // MENU_STATE_HPP