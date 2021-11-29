#ifndef STATE_HPP
#define STATE_HPP

#include "state_stack/state_identifiers.hpp"
#include "resources/resource_identifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;
class MusicPlayer;
class SoundPlayer;

class State
{
public:
	using Ptr = std::unique_ptr<State>;

	// Holder of shared objects between all states of the game
	// Used to save memory by avoiding to load the same resources for every state
	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player,
				MusicPlayer& music, SoundPlayer& sounds);

		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Player* player;
		MusicPlayer* music;
		SoundPlayer* sounds;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();

	Context	getContext() const;

private:
	StateStack* mStack;
	Context mContext;
};

#endif // STATE_HPP