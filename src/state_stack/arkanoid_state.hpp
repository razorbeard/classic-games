#ifndef ARKANOID_STATE_HPP
#define ARKANOID_STATE_HPP

#include "state_stack/state.hpp"
#include "arkanoid/arkanoid.hpp"

#include <SFML/System/Time.hpp>

class ArkanoidState : public State
{
public:
	ArkanoidState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	Arkanoid mWorld;
	Player& mPlayer;
};

#endif // ARKANOID_STATE_HPP