#include "music_player.hpp"

MusicPlayer::MusicPlayer()
	: mMusic()
	, mFilenames()
	, mVolume(100.0f)
{
	mFilenames[Music::ArkanoidTheme] = "resources/music/arkanoid_theme.ogg";
	mFilenames[Music::TetrisTheme] = "resources/music/tetris_theme.ogg";
	mFilenames[Music::MenuTheme] = "resources/music/menu_theme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string const filename{ mFilenames[theme] };

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}

void MusicPlayer::setPitch(float pitch)
{
	mMusic.setPitch(pitch);
}

float MusicPlayer::getPitch() const
{
	return mMusic.getPitch();
}