#include "emitter_node.hpp"
#include "particle_node.hpp"
#include "commands/command_queue.hpp"

EmitterNode::EmitterNode(Particle::Type type)
	: SceneNode()
	, mAccumulatedTime(sf::Time::Zero)
	, mType(type)
	, mParticleSystem(nullptr)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (mParticleSystem)
	{
		emitParticles(dt);
	}
	else
	{
		// Find particle node with the same type as emitter node
		auto finder = [this](ParticleNode& container, sf::Time)
		{
			if (container.getParticleType() == mType)
				mParticleSystem = &container;
		};

		Command command{};
		command.category = Category::ParticleSystem;
		command.action = derivedAction<ParticleNode>(finder);

		commands.push(command);
	}
}

void EmitterNode::emitParticles(sf::Time dt)
{
	const float emissionRate{ 30.0f };
	const sf::Time interval{ sf::seconds(1.0f) / emissionRate };

	mAccumulatedTime += dt;

	// Emit particles as long as the interval still fits into the current frame
	while (mAccumulatedTime > interval)
	{
		mAccumulatedTime -= interval; // Remaining time
		mParticleSystem->addParticle(getWorldPosition());
	}
}