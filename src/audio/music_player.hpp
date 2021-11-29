#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include "resources/resource_holder.hpp"
#include "resources/resource_identifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <unordered_map>
#include <string>

class MusicPlayer : private sf::NonCopyable
{
public:
	MusicPlayer();

	void play(Music::ID theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);
	void setPitch(float pitch);
	float getPitch() const;

private:
	sf::Music mMusic;
	std::unordered_map<Music::ID, std::string> mFilenames;
	float mVolume;
};

#endif // MUSIC_PLAYER_HPP