#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
// One category = one or multiple game objects that react to one command
namespace Category
{
	// We use bitwise operators to make combinations possible
	enum Type
	{
		None = 0,
		DefaultLayer = 1 << 0,
		PlayerVaus = 1 << 1,
		Enemy = 1 << 2,
		Block = 1 << 3,
		PowerUp = 1 << 4,
		Projectile = 1 << 5,
		ParticleSystem = 1 << 6,
		SoundEffect = 1 << 7,
	};
}

#endif // CATEGORY_HPP