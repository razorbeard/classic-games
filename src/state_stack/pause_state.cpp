#include "pause_state.hpp"
#include "gui/button.hpp"
#include "audio/music_player.hpp"
#include "audio/sound_player.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, mPausedText()
	, mGUIContainer()
	, mRectangleBackground()
{
	const sf::Font& font{ context.fonts->get(Fonts::Sansation) };
	sf::Vector2u const windowSize{ context.window->getSize() };

	mRectangleBackground.setFillColor(sf::Color::Black);
	mRectangleBackground.setOutlineThickness(2.0f);
	mRectangleBackground.setSize(sf::Vector2f(windowSize / 3u));
	centerOrigin(mRectangleBackground);
	mRectangleBackground.setPosition(sf::Vector2f(windowSize / 2u));

	mPausedText.setFont(font);
	mPausedText.setString("Game Paused");
	mPausedText.setCharacterSize(50);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	// Buttons settings
	auto resumeButton = std::make_shared<GUI::Button>(context);
	resumeButton->setPosition(0.5f * windowSize.x, 0.53f * windowSize.y);
	resumeButton->setText("Resume");
	resumeButton->setCallback([this]()
							  {
								  requestStackPop();
								  getContext().sounds->play(SoundEffect::PauseOut);
							  });

	auto backToMenuButton = std::make_shared<GUI::Button>(context);
	backToMenuButton->setPosition(0.5f * windowSize.x, 0.6f * windowSize.y);
	backToMenuButton->setText("Back to menu");
	backToMenuButton->setCallback([this]()
								  {
									  requestStateClear();
									  requestStackPush(States::Menu);
								  });

	mGUIContainer.pack(resumeButton);
	mGUIContainer.pack(backToMenuButton);

	getContext().music->setPaused(true);
	getContext().sounds->play(SoundEffect::PauseIn);
}

PauseState::~PauseState()
{
	getContext().music->setPaused(false);
}

void PauseState::draw()
{
	sf::RenderWindow& window{ *getContext().window };
	window.setView(window.getDefaultView());

	// Transparent screen, we can see the game in the background
	sf::RectangleShape tranparentBackground{};
	tranparentBackground.setFillColor(sf::Color(0, 0, 0, 150));
	tranparentBackground.setSize(window.getView().getSize());

	window.draw(tranparentBackground);
	window.draw(mRectangleBackground);
	window.draw(mPausedText);
	window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time dt)
{
	// Update buttons effects
	mGUIContainer.update(dt);

	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}