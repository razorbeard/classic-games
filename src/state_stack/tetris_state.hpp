#ifndef TETRIS_STATE_HPP
#define TETRIS_STATE_HPP

#include "state.hpp"
#include "tetris/tetris.hpp"

#include <SFML/System/Time.hpp>

class TetrisState : public State
{
public:
	TetrisState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	Tetris mWorld;
	Player& mPlayer;
};

#endif // TETRIS_STATE_HPP