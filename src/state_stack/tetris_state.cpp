#include "tetris_state.hpp"
#include "commands/player.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TetrisState::TetrisState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window, *context.fonts, *context.sounds)
	, mPlayer(*context.player)
{
	mPlayer.setMissionStatus(Player::MissionRunning);
}

void TetrisState::draw()
{
	mWorld.draw();
}

bool TetrisState::update(sf::Time dt)
{
	mWorld.update(dt);

	CommandQueue& commands{ mWorld.getCommandQueue() };
	//mPlayer.handleRealtimeInput(commands);

	return true;
}

bool TetrisState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands{ mWorld.getCommandQueue() };
	//mPlayer.handleEvent(event, commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		mWorld.rotate();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
		mWorld.moveCurrentPiece(Tetris::Direction::Right);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		mWorld.moveCurrentPiece(Tetris::Direction::Left);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		mWorld.moveCurrentPiece(Tetris::Direction::Down);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}