#include "player.hpp"
#include "command_queue.hpp"
#include "arkanoid/vaus.hpp"

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

struct VausMover
{
	VausMover(float vx, float vy)
		: velocity{ vx, vy }
	{
	}

	// Overload parenthesis operator, used in the constructor
	void operator() (Vaus& vaus, sf::Time) const
	{
		vaus.accelerate(velocity * vaus.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player()
	: mCurrentMissionStatus{ MissionRunning }
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Space] = Fire;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's vaus
	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::PlayerVaus;
	}
}

// Search if an event occurs and transmit command to the queue
void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

// Search in the Player bindings and transmit command to the queue
void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for (auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

void Player::initializeActions()
{
	mActionBinding[MoveLeft].action = derivedAction<Vaus>(VausMover(-1, 0));
	mActionBinding[MoveRight].action = derivedAction<Vaus>(VausMover(+1, 0));
	mActionBinding[Fire].action = derivedAction<Vaus>([](Vaus& a, sf::Time) { a.fire(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case Fire:
		return true;

	default:
		return false;
	}
}