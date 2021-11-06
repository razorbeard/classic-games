#ifndef RESOURCE_IDENTIFIERS_HPP
#define RESOURCE_IDENTIFIERS_HPP

namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		// Menu and common
		WindowIcon,
		TitleText,
		SelectText,
		RingBackground,
		Buttons,
		Particle,

		// Arkanoid
		Arkanoid,
		PipeBackground,

		// Tileset
		BackgroundTile,

		// Tetris
		Tetris,
	};
}

namespace Fonts
{
	enum ID
	{
		Sansation,
		Upheaval,
		EightBitArcade,
	};
}

namespace SoundEffect
{
	enum ID
	{
		// GUI
		StartPressed,
		ButtonPressed,
		ButtonSelected,
		PauseIn,
		PauseOut,

		// Arkanoid
		VausLaserFire,
		BlockHit,
		VausBallHit,
		Explosion,
		CollectPowerUp,
		IntroJingle,
		WinJingle,
		GameOverJingle,
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		ArkanoidTheme,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>        TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>              FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif // RESOURCE_IDENTIFIERS_HPP