#ifndef EMITTER_NODE_HPP
#define EMITTER_NODE_HPP

#include "scene_node.hpp"
#include "graphics/particle.hpp"

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
	explicit EmitterNode(Particle::Type type);

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void emitParticles(sf::Time dt);

private:
	sf::Time mAccumulatedTime;
	Particle::Type mType;
	ParticleNode* mParticleSystem;
};

#endif // EMITTER_NODE_HPP