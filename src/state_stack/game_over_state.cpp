#include "game_over_state.hpp"
#include "commands/player.hpp"
#include "audio/sound_player.hpp"
#include "audio/music_player.hpp"
#include "utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
	, mGameOverText()
	, mElapsedTime(sf::Time::Zero)
	, mPlayer(context.player)
	, mMusic(*context.music)
{
	const sf::Font& font{ context.fonts->get(Fonts::Sansation) };
	sf::Vector2f const windowSize{ context.window->getSize() };

	mGameOverText.setFont(font);

	if (mPlayer->getMissionStatus() == Player::MissionFailure)
	{
		mMusic.stop();
		mGameOverText.setString("GAME OVER");
		context.sounds->play(SoundEffect::GameOverJingle);
	}
	else if (mPlayer->getMissionStatus() == Player::MissionIntro)
	{
		mMusic.stop();
		mGameOverText.setString("Ready ?");
		context.sounds->play(SoundEffect::IntroJingle);
	}
	else
	{
		mMusic.stop();
		mGameOverText.setString("Victory !");
		context.sounds->play(SoundEffect::WinJingle);
	}

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.65f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window{ *getContext().window };
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background for Game Over and Victory states
	if (mPlayer->getMissionStatus() != Player::MissionIntro)
	{
		sf::RectangleShape backgroundShape{};
		backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
		backgroundShape.setSize(window.getView().getSize());

		window.draw(backgroundShape);
	}

	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	mElapsedTime += dt;

	// Show state for 3 seconds
	if (mElapsedTime > sf::seconds(3.0f))
	{
		// Intro finished : let the player play
		if (mPlayer->getMissionStatus() == Player::MissionIntro)
		{
			requestStackPop();
			mMusic.play(Music::ArkanoidTheme);
		}

		// Victory or defeat : go back to menu
		else
		{
			requestStateClear();
			requestStackPush(States::Menu);
		}
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}