#ifndef DATA_TABLES_HPP
#define DATA_TABLES_HPP

#include "resources/resource_identifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>

class Arkanoid;

struct ButtonData
{
	sf::IntRect textureRect;
};

struct VausData
{
	Textures::ID texture;
	sf::IntRect textureRect;
	sf::Vector2i frameSize;
	size_t numFrames;
	sf::Time duration{ sf::seconds(1.0f) };
	int spacing{ 2 };
	float speed{ 800.0f };
};

struct EnemyData
{
	Textures::ID texture;
	sf::IntRect textureRect;
	sf::Vector2i frameSize;
	size_t numFrames{ 3 };
	sf::Time duration{ sf::seconds(0.5f) };
	int spacing{ 2 };
	float speed;
};

struct PowerUpData
{
	Textures::ID texture;
	sf::IntRect textureRect;
	sf::Vector2i frameSize{ 62, 32 };
	size_t numFrames{ 6 };
	sf::Time duration{ sf::seconds(0.5f) };
	int spacing{ 2 };
	std::function<void(Arkanoid&)> action;
};

struct BlockData
{
	int hitpoints{ 1 };
	Textures::ID texture;
	sf::IntRect textureRect;
	sf::Vector2i frameSize{ 58, 32 };
	size_t numFrames{ 0 };
	sf::Time duration{ sf::Time::Zero };
	int spacing{ 2 };
	int value;
};

struct ProjectileData
{
	int damage;
	float speed;
	Textures::ID texture;
	sf::IntRect textureRect;

	Textures::ID destructionTexture;
	sf::IntRect destructionTextureRect;
	sf::Vector2i frameSize;
	size_t numFrames;
	int spacing{ 0 };
	sf::Time duration;
};

struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};

std::vector<std::vector<int>>   initializeStageData();
std::vector<ButtonData>         initializeButtonData();
std::vector<ParticleData>	    initializeParticleData();
std::vector<BlockData>          initializeBlockData();
std::vector<VausData>           initializeVausData();
std::vector<EnemyData>          initializeEnemyData();
std::vector<ProjectileData>     initializeProjectileData();
std::vector<PowerUpData>        initializePowerUpData();

#endif // DATA_TABLES_HPP