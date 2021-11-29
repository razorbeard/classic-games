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
		ArkanoidBackground,

		// Tileset
		BackgroundTile,

		// Tetris
		Tetris,
		TetrisBackground,
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

		// Tetris
		TetroMove,
		TetroRotate,
		TetroDrop,
		LineCleared,
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		ArkanoidTheme,
		TetrisTheme,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, SoundEffect::ID>;

#endif // RESOURCE_IDENTIFIERS_HPP