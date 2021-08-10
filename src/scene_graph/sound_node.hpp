#ifndef SOUND_NODE_HPP
#define SOUND_NODE_HPP

#include "scene_node.hpp"
#include "resources/resource_identifiers.hpp"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
	explicit SoundNode(SoundPlayer& player);
	void playSound(SoundEffect::ID sound, sf::Vector2f position);

	virtual unsigned int getCategory() const override;

private:
	SoundPlayer& mSounds;
};

#endif // SOUND_NODE_HPP