#include "sound_player.hpp"

#include <SFML/Audio/Listener.hpp>

#include <cmath>

namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	float const ListenerZ{ 300.0f };
	float const Attenuation{ 8.0f };
	float const MinDistance2D{ 200.0f };
	float const MinDistance3D{ std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ) };
}

SoundPlayer::SoundPlayer()
	: mSoundBuffers()
	, mSounds()
{
	mSoundBuffers.load(SoundEffect::ButtonPressed, "resources/sounds/button_pressed.wav");
	mSoundBuffers.load(SoundEffect::ButtonSelected, "resources/sounds/button_selected.wav");
	mSoundBuffers.load(SoundEffect::GameOverJingle, "resources/sounds/game_over_jingle.wav");
	mSoundBuffers.load(SoundEffect::IntroJingle, "resources/sounds/intro_jingle.wav");
	mSoundBuffers.load(SoundEffect::PauseIn, "resources/sounds/pause_in.wav");
	mSoundBuffers.load(SoundEffect::PauseOut, "resources/sounds/pause_out.wav");
	mSoundBuffers.load(SoundEffect::StartPressed, "resources/sounds/start_pressed.wav");
	mSoundBuffers.load(SoundEffect::WinJingle, "resources/sounds/win_jingle.wav");

	mSoundBuffers.load(SoundEffect::BlockHit, "resources/sounds/block_hit.wav");
	mSoundBuffers.load(SoundEffect::CollectPowerUp, "resources/sounds/collect_power_up.wav");
	mSoundBuffers.load(SoundEffect::Explosion, "resources/sounds/explosion.wav");
	mSoundBuffers.load(SoundEffect::VausBallHit, "resources/sounds/vaus_ball_hit.wav");
	mSoundBuffers.load(SoundEffect::VausLaserFire, "resources/sounds/vaus_laser_fire.wav");

	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.0f, 0.0f, -1.0f);
}

// Handle non-spatialized sounds
void SoundPlayer::play(SoundEffect::ID effect)
{
	play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
	mSounds.push_back(sf::Sound());
	sf::Sound& sound{ mSounds.back() };

	sound.setBuffer(mSoundBuffers.get(effect));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);

	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](const sf::Sound& s)
					  {
						  return s.getStatus() == sf::Sound::Stopped;
					  });
}

// Adapt the 3D audio coordinates to the 2D graphics coordinates
void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f const position{ sf::Listener::getPosition() };
	return sf::Vector2f(position.x, -position.y);
}