#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "resources/resource_holder.hpp"
#include "resources/resource_identifiers.hpp"
#include "commands/player.hpp"
#include "state_stack/state_stack.hpp"
#include "audio/music_player.hpp"
#include "audio/sound_player.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

// Controls input, logic updates and rendering
class Application
{
public:
	Application();
	void run();

private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void updateStatistics(sf::Time dt);
	void registerStates();

private:
	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	Player mPlayer;
	MusicPlayer mMusic;
	SoundPlayer mSounds;
	StateStack mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;
};

#endif // APPLICATION_HPP