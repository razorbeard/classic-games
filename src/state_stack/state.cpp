#include "state.hpp"
#include "state_stack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, MusicPlayer& music, SoundPlayer& sounds)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player(&player)
	, music(&music)
	, sounds(&sounds)
{
}

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{
}

State::~State()
{
}

// Allow the state to alter the stack, by adding it
void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

// Allow the state to alter the stack, by removing it
void State::requestStackPop()
{
	mStack->popState();
}

// Empty the active stack (special operation)
void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}