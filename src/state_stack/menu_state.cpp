#include "state_stack/menu_state.hpp"
#include "gui/animated_button.hpp"
#include "audio/music_player.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cmath>
#include <thread>
#include <chrono>

namespace
{
	using namespace std::chrono_literals;
	static std::thread* transitionTimer{ nullptr };
}

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mCircle(length(sf::Vector2f(context.window->getSize() / 2u)) + 15.0f, 16)
	, mScaleEffectTime(sf::Time::Zero)
	, mLargeRingBackground(context.textures->get(Textures::RingBackground))
	, mSmallRingBackground(context.textures->get(Textures::RingBackground))
	, mBackgroundSprite(context.textures->get(Textures::SelectText))
	, mGUIContainer()
	, mNextState(States::None)
	, mStartTransition(false)
	, mThreadAvailable(true)
{
	sf::Vector2u windowSize{ context.window->getSize() };

	// Transition effect
	centerOrigin(mCircle);
	mCircle.setFillColor(sf::Color::Transparent);
	mCircle.setOutlineColor(sf::Color::Black);
	mCircle.setPosition(sf::Vector2f(windowSize / 2u));
	mCircle.setOutlineThickness(-mCircle.getRadius());

	// Background sprites settings
	centerOrigin(mLargeRingBackground);
	mLargeRingBackground.setPosition(sf::Vector2f(windowSize / 2u));
	mLargeRingBackground.setColor(sf::Color(0, 0, 0, 125));

	centerOrigin(mSmallRingBackground);
	mSmallRingBackground.setPosition(sf::Vector2f(windowSize / 2u));
	mSmallRingBackground.setColor(sf::Color(0, 0, 0, 125));
	mSmallRingBackground.setRotation(90.0f);

	// Selection text sprite
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(sf::Vector2f(windowSize / 2u) + sf::Vector2f(0.0f, -200.0f));

	// GUI setup
	auto arkanoidButton = std::make_shared<GUI::AnimatedButton>(GUI::AnimatedButton::ArkanoidMenu, context);
	arkanoidButton->setPosition(sf::Vector2f(windowSize.x * 0.33f, windowSize.y/2u));
	arkanoidButton->setToggle(true);
	arkanoidButton->setText("Arkanoid", 40, 3.0f);
	arkanoidButton->setCallback([this]()
								{
									mThreadAvailable = false;
									transitionTimer = new std::thread([&]()
																	  {
																		  std::this_thread::sleep_for(1.5s);
																		  mStartTransition = true;
																	  });
									mNextState = States::ID::Arkanoid;
								});

	auto tetrisButton = std::make_shared<GUI::AnimatedButton>(GUI::AnimatedButton::TetrisMenu, context);
	tetrisButton->setPosition(sf::Vector2f(windowSize.x * 0.66f, windowSize.y / 2u));
	tetrisButton->setToggle(true);
	tetrisButton->setText("Tetris", 40, 3.0f);
	tetrisButton->setCallback([this]()
								{
									mThreadAvailable = false;
									transitionTimer = new std::thread([&]()
																	  {
																		  std::this_thread::sleep_for(1.5s);
																		  mStartTransition = true;
																	  });
									mNextState = States::ID::Tetris;
								});

	auto exitButton = std::make_shared<GUI::AnimatedButton>(GUI::AnimatedButton::QuitMenu, context);
	exitButton->setPosition(sf::Vector2f(windowSize / 10u) + sf::Vector2f(0.0f, 600.0f));
	exitButton->setText("Quit", 40, 3.0f);
	exitButton->setCallback([this]()
							{
								requestStackPop();
							});

	mGUIContainer.pack(arkanoidButton);
	mGUIContainer.pack(tetrisButton);
	mGUIContainer.pack(exitButton);

	// Play menu theme
	context.music->setVolume(30.0f);
	context.music->setPitch(1.0f);
	context.music->play(Music::MenuTheme);
}

void MenuState::draw()
{
	sf::RenderWindow& window{ *getContext().window };
	window.setView(window.getDefaultView());

	window.clear(sf::Color(34, 32, 52, 255));

	window.draw(mLargeRingBackground);
	window.draw(mSmallRingBackground);
	window.draw(mBackgroundSprite);

	window.draw(mGUIContainer);
	window.draw(mCircle);
}

bool MenuState::update(sf::Time dt)
{
	mScaleEffectTime += dt;
	float const largePeriod{ std::sin(mScaleEffectTime.asSeconds()) / 10.0f + 1.3f };
	float const smallPeriod{ std::sin(mScaleEffectTime.asSeconds() + 2.0f) / 8.0f + 0.7f };

	mLargeRingBackground.rotate(dt.asSeconds() * 20.0f);
	mLargeRingBackground.setScale(largePeriod, largePeriod);

	mSmallRingBackground.rotate(dt.asSeconds() * 25.0f);
	mSmallRingBackground.setScale(smallPeriod, smallPeriod);

	// Update buttons effects
	mGUIContainer.update(dt);

	// Transition update : the circle disapear to show the scene
	if (mCircle.getOutlineThickness() <= mCircle.getRadius())
		mCircle.setOutlineThickness(mCircle.getOutlineThickness() + 10.0f);

	// Launch the next state when an other thread has finished sleeping
	if (mStartTransition)
	{
		// Clean the memory before jumping to the next state
		transitionTimer->join();
		delete transitionTimer;
		transitionTimer = nullptr;

		requestStackPop();
		requestStackPush(mNextState);
	}

	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// Wait for circle animation to fade before handling events
	// If a key has been pressed, events are not handled anymore
	if (mCircle.getOutlineThickness() >= 0 && mThreadAvailable)
		mGUIContainer.handleEvent(event);

	return false;
}