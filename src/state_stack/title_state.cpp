#include "title_state.hpp"
#include "audio/sound_player.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <thread>
#include <chrono>

namespace
{
	using namespace std::chrono_literals;
	static std::thread* transitionTimer{ nullptr };
}

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mCircle(length(sf::Vector2f(context.window->getSize() / 2u)) + 15.0f, 16)
	, mTitle(context.textures->get(Textures::TitleText))
	, mText("PRESS ANY KEY !", context.fonts->get(Fonts::Sansation))
	, mShowText(true)
	, mSounds(*context.sounds)
	, mMaxBlinkingTime(sf::seconds(0.5f))
	, mTextEffectTime(sf::Time::Zero)
	, mThreadAvailable(true)
	, mStartTransition(false)
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

	// Switch to the next state when the other thread has finished to sleep
	if (mStartTransition)
	{
		mCircle.setOutlineThickness(mCircle.getOutlineThickness() - 10.0f);

		if (-mCircle.getOutlineThickness() >= mCircle.getRadius())
		{
			// Clean the memory before jumping to the next state
			transitionTimer->join();
			delete transitionTimer;
			transitionTimer = nullptr;

			requestStackPop();
			requestStackPush(States::Menu);
		}
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, launch a new thread to start a timer,
	// then trigger the next screen with the main thread
	if (event.type == sf::Event::KeyReleased && mThreadAvailable)
	{
		// Signal received => avoid the player launching multiple threads
		mThreadAvailable = false;
		mMaxBlinkingTime = sf::seconds(0.1f);

		mSounds.play(SoundEffect::StartPressed);

		// NB: when this thread goes out of scope, it will try to join
		// with the main thread, causing the program to pause
		// That's why it is defined in the namespace as a pointer
		transitionTimer = new std::thread([&]()
										  {
											  std::this_thread::sleep_for(1.5s);
											  mStartTransition = true;
										  });
	}

	return true;
}