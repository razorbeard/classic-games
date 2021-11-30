#include "state_stack/title_state.hpp"
#include "audio/sound_player.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mCircle(length(sf::Vector2f(context.window->getSize() / 2u)) + 15.0f, 16)
	, mTitle(context.textures->get(Textures::TitleText))
	, mText("PRESS ANY KEY !", context.fonts->get(Fonts::Sansation))
	, mSounds(*context.sounds)
	, mMaxBlinkingTime(sf::seconds(0.5f))
	, mTextEffectTime(sf::Time::Zero)
	, mTransitionTime(sf::Time::Zero)
	, mShowText(true)
	, mHasPressedKey(false)
{
	// Convex hull of the window: we add an extra margin for the radius to avoid
	// empty corners when rendering (we prefer a smaller number of segments)
	centerOrigin(mCircle);
	mCircle.setFillColor(sf::Color::Transparent);
	mCircle.setOutlineColor(sf::Color::Black);
	mCircle.setPosition(sf::Vector2f(context.window->getSize() / 2u));

	// Add the title sprite
	centerOrigin(mTitle);
	mTitle.setPosition(sf::Vector2f(context.window->getSize() / 2u) + sf::Vector2f(0.0f, -100.0f));

	// Add the text for pressing any button
	centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u) + sf::Vector2f(0.0f, 200.0f));

	// Add some animated stars on the background
	for (auto& animation : mShinning)
	{
		animation.setTexture(context.textures->get(Textures::Particle), sf::IntRect(0, 34, 84, 28));
		animation.setFrameSize(sf::Vector2i(28, 28));
		animation.setNumFrames(3);
		animation.setDuration(sf::seconds(randomFloat(1, 2)));
		animation.setRepeating(true);
		centerOrigin(animation);
	}

	mShinning[0].setPosition(sf::Vector2f(sf::Vector2f(773.0f, 84.0f)));
	mShinning[1].setPosition(sf::Vector2f(sf::Vector2f(225.0f, 568.0f)));
	mShinning[2].setPosition(sf::Vector2f(sf::Vector2f(114.0f, 660.0f)));
	mShinning[3].setPosition(sf::Vector2f(sf::Vector2f(697.0f, 473.0f)));
	mShinning[4].setPosition(sf::Vector2f(sf::Vector2f(128.0f, 73.0f)));
	mShinning[5].setPosition(sf::Vector2f(sf::Vector2f(939.0f, 700.0f)));
}

void TitleState::draw()
{
	sf::RenderWindow& window{ *getContext().window };
	window.clear(sf::Color(34, 32, 52, 255)); // Gives a dark-purple background

	for (const auto& animation : mShinning)
	{
		window.draw(animation);
	}

	if (mShowText)
	{
		window.draw(mText);
	}

	window.draw(mTitle);
	window.draw(mCircle);
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	if (mHasPressedKey) mTransitionTime += dt;

	// Induce blinking effect to the sf::Text object
	if (mTextEffectTime >= mMaxBlinkingTime)
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	// Update stars sprites
	for (auto& animation : mShinning)
	{
		animation.update(dt);
	}

	// Switch to the next state when the player has pressed any key
	if (mTransitionTime >= sf::seconds(1.5f))
	{
		mCircle.setOutlineThickness(mCircle.getOutlineThickness() - 10.0f);

		if (-mCircle.getOutlineThickness() >= mCircle.getRadius())
		{
			requestStackPop();
			requestStackPush(States::Menu);
		}
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased && !mHasPressedKey)
	{
		mHasPressedKey = true;
		mMaxBlinkingTime = sf::seconds(0.1f);

		mSounds.play(SoundEffect::StartPressed);
	}

	return true;
}