#include "data_tables.hpp"
#include "graphics/particle.hpp"
#include "gui/animated_button.hpp"
#include "arkanoid/projectile.hpp"
#include "arkanoid/arkanoid.hpp"
#include "arkanoid/power_up.hpp"
#include "arkanoid/enemy.hpp"

#include <limits>

std::vector<std::vector<int>> initializeStageData()
{
	std::vector<std::vector<int>> data(5, std::vector<int>(14 * 5));

	// Reminder : White  = 0      Blue   = 5      No block = -1
	//            Orange = 1      Violet = 6
	//            Cyan   = 2      Yellow = 7
	//            Green  = 3      Silver = 8
	//            Red    = 4      Gold   = 9

	data[0] = { -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1,
				-1,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  0, -1,
				 0,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  0,
				-1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0, -1,
				-1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1 };

	data[1] = { -1, -1, -1, -1, -1, -1,  7,  7, -1, -1, -1, -1, -1, -1,
				 4,  4,  4, -1, -1,  0,  0,  0,  0, -1, -1,  4,  4,  4,
				 2,  3, -1, -1,  2,  2,  9,  9,  2,  2, -1, -1,  3,  2,
				 4,  4,  4, -1, -1,  0,  0,  0,  0, -1, -1,  4,  4,  4,
				-1, -1, -1, -1, -1, -1,  7,  7, -1, -1, -1, -1, -1, -1 };

	return data;
}

std::vector<ButtonData> initializeButtonData()
{
	std::vector<ButtonData> data(GUI::AnimatedButton::IdentifierCount);

	data[GUI::AnimatedButton::ArkanoidMenu].textureRect = sf::IntRect(0, 0, 160, 160);
	data[GUI::AnimatedButton::QuitMenu].textureRect = sf::IntRect(0, 160, 84, 84);

	return data;
}

std::vector<VausData> initializeVausData()
{
	std::vector<VausData> data(Vaus::ModeCount);

	data[Vaus::Normal].texture = Textures::Arkanoid;
	data[Vaus::Normal].textureRect = sf::IntRect(384, 136, 109, 128);
	data[Vaus::Normal].frameSize = sf::Vector2i(109, 24);
	data[Vaus::Normal].numFrames = 5;

	data[Vaus::Catch] = data[Vaus::Normal];

	data[Vaus::Laser].texture = Textures::Arkanoid;
	data[Vaus::Laser].textureRect = sf::IntRect(495, 136, 109, 128);
	data[Vaus::Laser].frameSize = sf::Vector2i(109, 24);
	data[Vaus::Laser].numFrames = 5;

	data[Vaus::Enlarge].texture = Textures::Arkanoid;
	data[Vaus::Enlarge].textureRect = sf::IntRect(420, 0, 173, 128);
	data[Vaus::Enlarge].frameSize = sf::Vector2i(173, 24);
	data[Vaus::Enlarge].numFrames = 5;

	return data;
}

std::vector<EnemyData> initializeEnemyData()
{
	std::vector<EnemyData> data(Enemy::TypeCount);

	data[Enemy::Ghost].texture = Textures::Arkanoid;
	data[Enemy::Ghost].textureRect = sf::IntRect(384, 266, 166, 42);
	data[Enemy::Ghost].frameSize = sf::Vector2i(54, 42);
	data[Enemy::Ghost].speed = 100.0f;

	data[Enemy::Skull].texture = Textures::Arkanoid;
	data[Enemy::Skull].textureRect = sf::IntRect(384, 310, 124, 39);
	data[Enemy::Skull].frameSize = sf::Vector2i(40, 39);
	data[Enemy::Skull].speed = 80.0f;

	return data;
}

std::vector<PowerUpData> initializePowerUpData()
{
	std::vector<PowerUpData> data(PowerUp::TypeCount);

	data[PowerUp::Laser].texture = Textures::Arkanoid;
	data[PowerUp::Laser].textureRect = sf::IntRect(0, 136, 382, 32);
	data[PowerUp::Laser].action = [](Arkanoid& a) { a.setVausMode(Vaus::Laser); };

	data[PowerUp::Enlarge].texture = Textures::Arkanoid;
	data[PowerUp::Enlarge].textureRect = sf::IntRect(0, 170, 382, 32);
	data[PowerUp::Enlarge].action = [](Arkanoid& a) { a.setVausMode(Vaus::Enlarge); };

	data[PowerUp::Catch].texture = Textures::Arkanoid;
	data[PowerUp::Catch].textureRect = sf::IntRect(0, 204, 382, 32);
	data[PowerUp::Catch].action = [](Arkanoid& a) { a.setVausMode(Vaus::Catch); };

	data[PowerUp::Slow].texture = Textures::Arkanoid;
	data[PowerUp::Slow].textureRect = sf::IntRect(0, 238, 382, 32);
	data[PowerUp::Slow].action = [](Arkanoid& a) { a.slowBall(); };

	data[PowerUp::Break].texture = Textures::Arkanoid;
	data[PowerUp::Break].textureRect = sf::IntRect(0, 272, 382, 32);
	data[PowerUp::Break].action = [](Arkanoid& a) { a.resetBlockCounter(); };

	data[PowerUp::Disruption].texture = Textures::Arkanoid;
	data[PowerUp::Disruption].textureRect = sf::IntRect(0, 306, 382, 32);
	data[PowerUp::Disruption].action = [](Arkanoid& a) { a.multiBalls(2); a.setVausMode(Vaus::Normal); };

	data[PowerUp::Player].texture = Textures::Arkanoid;
	data[PowerUp::Player].textureRect = sf::IntRect(0, 339, 382, 32);
	data[PowerUp::Player].action = [](Arkanoid& a) { a.earnExtraLife(); };

	return data;
}

std::vector<BlockData> initializeBlockData()
{
	std::vector<BlockData> data(Block::ColorCount);

	data[Block::White].texture = Textures::Arkanoid;
	data[Block::White].textureRect = sf::IntRect(0, 0, 58, 32);
	data[Block::White].value = 50;

	data[Block::Orange].texture = Textures::Arkanoid;
	data[Block::Orange].textureRect = sf::IntRect(60, 0, 58, 32);
	data[Block::Orange].value = 60;

	data[Block::Cyan].texture = Textures::Arkanoid;
	data[Block::Cyan].textureRect = sf::IntRect(120, 0, 58, 32);
	data[Block::Cyan].value = 70;

	data[Block::Green].texture = Textures::Arkanoid;
	data[Block::Green].textureRect = sf::IntRect(180, 0, 58, 32);
	data[Block::Green].value = 80;

	data[Block::Blue].texture = Textures::Arkanoid;
	data[Block::Blue].textureRect = sf::IntRect(0, 34, 58, 32);
	data[Block::Blue].value = 100;

	data[Block::Violet].texture = Textures::Arkanoid;
	data[Block::Violet].textureRect = sf::IntRect(60, 34, 58, 32);
	data[Block::Violet].value = 110;

	data[Block::Yellow].texture = Textures::Arkanoid;
	data[Block::Yellow].textureRect = sf::IntRect(120, 34, 58, 32);
	data[Block::Yellow].value = 120;

	data[Block::Red].texture = Textures::Arkanoid;
	data[Block::Red].textureRect = sf::IntRect(180, 34, 58, 32);
	data[Block::Red].value = 90;

	data[Block::Silver].hitpoints = 2;
	data[Block::Silver].texture = Textures::Arkanoid;
	data[Block::Silver].textureRect = sf::IntRect(0, 68, 418, 32);
	data[Block::Silver].duration = sf::seconds(0.5f);
	data[Block::Silver].numFrames = 7;
	data[Block::Silver].value = 50;

	data[Block::Gold].hitpoints = std::numeric_limits<int>::max();
	data[Block::Gold].texture = Textures::Arkanoid;
	data[Block::Gold].textureRect = sf::IntRect(0, 102, 418, 32);
	data[Block::Gold].duration = sf::seconds(0.5f);
	data[Block::Gold].numFrames = 7;
	data[Block::Gold].value = 0;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::Bullet].damage = 1;
	data[Projectile::Bullet].speed = 400.0f;
	data[Projectile::Bullet].texture = Textures::Arkanoid;
	data[Projectile::Bullet].textureRect = sf::IntRect(265, 23, 8, 24);
	data[Projectile::Bullet].destructionTexture = Textures::Particle;
	data[Projectile::Bullet].destructionTextureRect = sf::IntRect(0, 62, 39, 12);
	data[Projectile::Bullet].frameSize = sf::Vector2i(13, 12);
	data[Projectile::Bullet].numFrames = 3;
	data[Projectile::Bullet].duration = sf::seconds(0.2f);

	data[Projectile::Ball].damage = 1;
	data[Projectile::Ball].speed = 500.0f;
	data[Projectile::Ball].texture = Textures::Arkanoid;
	data[Projectile::Ball].textureRect = sf::IntRect(265, 0, 20, 20);
	data[Projectile::Ball].destructionTexture = Textures::Particle;
	data[Projectile::Ball].destructionTextureRect = sf::IntRect(0, 0, 202, 32);
	data[Projectile::Ball].frameSize = sf::Vector2i(32, 32);
	data[Projectile::Ball].numFrames = 6;
	data[Projectile::Ball].spacing = 2;
	data[Projectile::Ball].duration = sf::seconds(0.3f);

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(Particle::ParticleCount);

	data[Particle::Star].color = sf::Color(255, 255, 50);
	data[Particle::Star].lifetime = sf::seconds(4.f);

	return data;
}