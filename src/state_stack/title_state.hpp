#ifndef TITLE_STATE_HPP
#define TITLE_STATE_HPP

#include "state_stack/state.hpp"
#include "graphics/animation.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <array>

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	std::array<Animation, 6> mShinning;
	sf::CircleShape mCircle;
	sf::Sprite mTitle;
	sf::Text mText;
	SoundPlayer& mSounds;
	sf::Time mMaxBlinkingTime;
	sf::Time mTextEffectTime;
	sf::Time mTransitionTime;
	bool mShowText;
	bool mHasPressedKey;
};

#endif // TITLE_STATE_HPP