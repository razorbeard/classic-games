#include "animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Animation::Animation()
	: mSprite()
	, mFrameSize()
	, mNumFrames(0)
	, mCurrentFrame(0)
	, mSpacing(0)
	, mInitialTextRect()
	, mDuration(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero)
	, mRepeat(false)
{
}

// Set an animation using a whole texture
Animation::Animation(const sf::Texture& texture)
	: mSprite(texture)
	, mFrameSize()
	, mNumFrames(0)
	, mCurrentFrame(0)
	, mSpacing(0)
	, mInitialTextRect(mSprite.getTextureRect())
	, mDuration(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero)
	, mRepeat(false)
{
}

// Set an animation on a sub-rectangle of a texture
Animation::Animation(const sf::Texture& texture, const sf::IntRect& rectangle)
	: mSprite(texture, rectangle)
	, mFrameSize()
	, mNumFrames(0)
	, mCurrentFrame(0)
	, mSpacing(0)
	, mInitialTextRect(rectangle)
	, mDuration(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero)
	, mRepeat(false)
{
}

// Take a whole texture to set up the sprite animation
void Animation::setTexture(const sf::Texture& texture)
{
	mSprite.setTexture(texture);
}

// Take a sub-rectangle of a texture to set up the sprite animation
void Animation::setTexture(const sf::Texture& texture, const sf::IntRect& rectangle)
{
	mSprite.setTexture(texture);
	mSprite.setTextureRect(rectangle);

	mInitialTextRect = rectangle;
}

void Animation::setTextureRect(const sf::IntRect& rectangle)
{
	mSprite.setTextureRect(rectangle);

	mInitialTextRect = rectangle;
}

const sf::Texture* Animation::getTexture() const
{
	return mSprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	mFrameSize = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return mFrameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return mNumFrames;
}

void Animation::setDuration(sf::Time duration)
{
	mDuration = duration;
}

sf::Time Animation::getDuration() const
{
	return mDuration;
}

void Animation::setSpacing(int spacing)
{
	// Set a pixel spacing between the frames
	mSpacing = spacing;
}

void Animation::setRepeating(bool flag)
{
	mRepeat = flag;
}

bool Animation::isRepeating() const
{
	return mRepeat;
}

void Animation::restart()
{
	mCurrentFrame = 0;

	// This statement is needed: as time passed, every call to update()
	// makes mElapsedTime really huge over the variable timePerFrame.
	// Thus, the while loop can skip every frames, resulting as an instant
	// textureRect update
	mElapsedTime = sf::Time::Zero;
}

void Animation::setCurrentFrame(std::size_t currentFrame)
{
	mCurrentFrame = currentFrame;
}

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(0.0f, 0.0f, (float)getFrameSize().x, (float)getFrameSize().y);
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::update(sf::Time dt)
{
	sf::Time timePerFrame{ mDuration / static_cast<float>(mNumFrames) };
	mElapsedTime += dt;

	sf::IntRect textureRect{ mSprite.getTextureRect() };

	// Set the first frame
	if (mCurrentFrame == 0)
	{
		textureRect = sf::IntRect(mInitialTextRect.left, mInitialTextRect.top, mFrameSize.x, mFrameSize.y);
	}

	// While we have a frame to process
	while (mElapsedTime >= timePerFrame && (mCurrentFrame <= mNumFrames || mRepeat))
	{
		// Check if there's a frame available on the right. If so, move the texture rect
		// to the right, adding an optional spacing in pixels
		if (textureRect.left + mFrameSize.x + mSpacing < mInitialTextRect.left + mInitialTextRect.width)
		{
			textureRect.left += mFrameSize.x + mSpacing;
		}

		// If we reach the end of the rectangle (ie the right-most frame)
		else if (textureRect.left + mFrameSize.x + mSpacing >= mInitialTextRect.width + mInitialTextRect.left
				 && textureRect.top + mFrameSize.y + mSpacing < mInitialTextRect.height + mInitialTextRect.top)
		{
			// Jump to the next line of frames
			textureRect.left = mInitialTextRect.left;
			textureRect.top += mFrameSize.y + mSpacing;
		}

		// And progress to next frame
		mElapsedTime -= timePerFrame;
		if (mRepeat)
		{
			mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;

			if (mCurrentFrame == 0)
				textureRect = sf::IntRect(mInitialTextRect.left, mInitialTextRect.top, mFrameSize.x, mFrameSize.y);
		}
		else
		{
			mCurrentFrame++;
		}
	}

	mSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}