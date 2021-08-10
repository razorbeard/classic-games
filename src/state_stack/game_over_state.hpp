#ifndef GAME_OVER_STATE_HPP
#define GAME_OVER_STATE_HPP

#include "state.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:
	GameOverState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	sf::Text mGameOverText;
	sf::Time mElapsedTime;
	Player* mPlayer;
	MusicPlayer& mMusic;
};

#endif // GAME_OVER_STATE_HPP