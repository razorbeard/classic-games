#include "arkanoid_state.hpp"
#include "commands/player.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

ArkanoidState::ArkanoidState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window, *context.fonts, *context.sounds)
	, mPlayer(*context.player)
{
	mPlayer.setMissionStatus(Player::MissionRunning);
}

void ArkanoidState::draw()
{
	mWorld.draw();
}

bool ArkanoidState::update(sf::Time dt)
{
	if (mWorld.hasClearStage())
	{
		mWorld.setNextStage(dt);
		mPlayer.setMissionStatus(Player::MissionIntro);
		requestStackPush(States::GameOver);
	}
	if (mWorld.hasBallsRemaining() && mWorld.hasLivesRemaining())
	{
		mWorld.setLostBallScene(dt);
		mPlayer.setMissionStatus(Player::MissionIntro);
		requestStackPush(States::GameOver);
	}

	mWorld.update(dt);

	if (!mWorld.hasLivesRemaining())
	{
		mPlayer.setMissionStatus(Player::MissionFailure);
		requestStackPush(States::GameOver);
	}
	else if (mWorld.hasPlayerReachedEnd())
	{
		mPlayer.setMissionStatus(Player::MissionSuccess);
		requestStackPush(States::GameOver);
	}

	CommandQueue& commands{ mWorld.getCommandQueue() };
	mPlayer.handleRealtimeInput(commands);

	return true;
}

bool ArkanoidState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands{ mWorld.getCommandQueue() };
	mPlayer.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}