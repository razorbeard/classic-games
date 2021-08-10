#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class Animation : public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	explicit Animation(const sf::Texture& texture);
	explicit Animation(const sf::Texture& texture, const sf::IntRect& rectangle);

	void setTexture(const sf::Texture& texture);
	void setTexture(const sf::Texture& texture, const sf::IntRect& rectangle);
	void setTextureRect(const sf::IntRect& rectangle);
	const sf::Texture* getTexture() const;

	void setFrameSize(sf::Vector2i mFrameSize);
	sf::Vector2i getFrameSize() const;

	void setNumFrames(std::size_t numFrames);
	std::size_t getNumFrames() const;

	void setDuration(sf::Time duration);
	sf::Time getDuration() const;

	void setSpacing(int spacing);

	void setRepeating(bool flag);
	bool isRepeating() const;

	void restart();
	void setCurrentFrame(std::size_t currentFrame);
	bool isFinished() const;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	void update(sf::Time dt);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;
	std::size_t mNumFrames;
	std::size_t mCurrentFrame;
	sf::Time mDuration;
	sf::Time mElapsedTime;
	int mSpacing;
	bool mRepeat;

	sf::IntRect mInitialTextRect;
};

#endif // ANIMATION_HPP