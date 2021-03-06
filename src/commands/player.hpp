#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "commands/command.hpp"

#include <SFML/Window/Event.hpp>

#include <unordered_map>

class CommandQueue;

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		Fire,
		ActionCount
	};

	enum MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure,
		MissionIntro
	};

public:
	Player();

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

	void setMissionStatus(MissionStatus status);
	MissionStatus getMissionStatus() const;

private:
	void initializeActions();
	static bool	isRealtimeAction(Action action);

private:
	std::unordered_map<sf::Keyboard::Key, Action> mKeyBinding;
	std::unordered_map<Action, Command> mActionBinding;
	MissionStatus mCurrentMissionStatus;
};

#endif // PLAYER_HPP