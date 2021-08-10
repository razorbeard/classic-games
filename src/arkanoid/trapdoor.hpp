#ifndef TRAPDOOR_HPP
#define TRAPDOOR_HPP

#include "scene_graph/scene_node.hpp"
#include "graphics/animation.hpp"
#include "resources/resource_identifiers.hpp"

class Trapdoor : public SceneNode
{
public:
	Trapdoor(const TextureHolder& textures);

	void activate();

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Animation mSpriteAnimation;

	bool mIsActivated;
};

#endif // TRAPDOOR_HPP