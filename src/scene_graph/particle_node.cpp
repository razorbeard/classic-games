#include "particle_node.hpp"
#include "data_tables.hpp"
#include "resources/resource_holder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>

namespace
{
	const std::vector<ParticleData> Table = initializeParticleData();
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures)
	: SceneNode()
	, mParticles()
	, mTexture(textures.get(Textures::Particle))
	, mType(type)
	, mVertexArray(sf::Quads)
	, mNeedsVertexUpdate(true)
{
}

// Insert a new particle into the system, according to his type from the data table
void ParticleNode::addParticle(sf::Vector2f position)
{
	Particle particle{};
	particle.position = position;
	particle.color = Table[mType].color;
	particle.lifetime = Table[mType].lifetime;

	mParticles.push_back(particle);
}

Particle::Type ParticleNode::getParticleType() const
{
	return mType;
}

unsigned int ParticleNode::getCategory() const
{
	return Category::ParticleSystem;
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue&)
{
	// Remove expired particles at beginning
	while (!mParticles.empty() && mParticles.front().lifetime <= sf::Time::Zero)
	{
		mParticles.pop_front();
	}

	// Decrease lifetime of existing particles
	for (Particle& particle : mParticles)
	{
		particle.lifetime -= dt;
	}

	// Flag to express that the render geometry must be recomputed
	mNeedsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mNeedsVertexUpdate)
	{
		computeVertices();
		mNeedsVertexUpdate = false;
	}

	// Apply particle texture
	states.texture = &mTexture;

	// Draw vertices
	target.draw(mVertexArray, states);
}

void ParticleNode::addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
	sf::Vertex vertex{};
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
	vertex.color = color;

	mVertexArray.append(vertex);
}

void ParticleNode::computeVertices() const
{
	// Need to dertermine the vertex positions
	sf::IntRect const rect{ Table[mType].textureRect };
	sf::Vector2f const size( rect.width, rect.height );
	sf::Vector2f const half{ size / 2.0f };

	// Refill vertex array
	mVertexArray.clear();
	for (const Particle& particle : mParticles)
	{
		sf::Vector2f const pos{ particle.position };
		sf::Color color{ particle.color };

		// The ratio value is used to set the alpha value of the particle, to give a fade effect
		float const ratio{ particle.lifetime.asSeconds() / Table[mType].lifetime.asSeconds() };
		color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.0f));

		// Vertice in every corner of the rectangle
		addVertex(pos.x - half.x, pos.y - half.y, rect.left, rect.top, color);
		addVertex(pos.x + half.x, pos.y - half.y, rect.left + rect.width, rect.top, color);
		addVertex(pos.x + half.x, pos.y + half.y, rect.left + rect.width, rect.top + rect.height, color);
		addVertex(pos.x - half.x, pos.y + half.y, rect.left, rect.top + rect.height, color);
	}
}