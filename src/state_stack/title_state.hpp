#ifndef TITLE_STATE_HPP
#define TITLE_STATE_HPP

#include "state.hpp"
#include "graphics/animation.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <array>
#include <atomic>

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	std::array<Animation, 6> mShinning;
	SoundPlayer& mSounds;

	sf::Sprite mTitle;
	sf::Text mText;
	sf::CircleShape mCircle;
	sf::Time mMaxBlinkingTime;

	bool mShowText;
	bool mThreadAvailable;
	sf::Time mTextEffectTime;
	std::atomic_bool mStartTransition;
};

#endif // TITLE_STATE_HPP