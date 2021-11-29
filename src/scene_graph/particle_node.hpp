#ifndef PARTICLE_NODE_HPP
#define PARTICLE_NODE_HPP

#include "scene_graph/scene_node.hpp"
#include "graphics/particle.hpp"
#include "resources/resource_identifiers.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>

class ParticleNode : public SceneNode
{
public:
	ParticleNode(Particle::Type type, const TextureHolder& textures);

	void addParticle(sf::Vector2f position);
	Particle::Type getParticleType() const;
	virtual unsigned int getCategory() const override;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
	void computeVertices() const;

private:
	std::deque<Particle> mParticles;
	const sf::Texture& mTexture;
	Particle::Type mType;

	mutable sf::VertexArray	mVertexArray;
	mutable bool mNeedsVertexUpdate;
};

#endif // PARTICLE_NODE_HPP