#include "animated_button.hpp"
#include "utility.hpp"
#include "data_tables.hpp"
#include "audio/sound_player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

namespace
{
	const std::vector<ButtonData> Table = initializeButtonData();
}

namespace GUI
{
	// Build an animated sprite button with known data (only available in buttons.png)
	AnimatedButton::AnimatedButton(Identifier id, State::Context context)
		: Button(context)
		, mIdentifier(id)
		, mSprite(context.textures->get(Textures::Buttons))
	{
		centerOrigin(mSprite);
		changeTexture(Normal);

		// Place the text just below the sprite by default
		sf::FloatRect const bounds{ mSprite.getGlobalBounds() };
		mText.setPosition(std::floor(bounds.left + bounds.width / 2.0f), std::floor(bounds.top + bounds.height));
	}

	void AnimatedButton::select()
	{
		Component::select();

		changeTexture(Selected);
		mSounds.play(SoundEffect::ButtonSelected);
	}

	void AnimatedButton::activate()
	{
		// Button has been pressed
		Component::activate();

		// If we are toggle then we should show that the button is pressed and thus "toggled"
		if (mIsToggle)
			changeTexture(Pressed);

		if (mCallback)
			mCallback();

		// If we are not a toggle then deactivate the button since we are just momentarily activated
		if (!mIsToggle)
			deactivate();

		mSounds.play(SoundEffect::ButtonPressed);
	}

	void AnimatedButton::update(sf::Time dt)
	{
		mTimer += dt;

		if (mCurrentType == GUI::Button::Selected)
		{
			// Lightly move the sprite upper
			if (mTimer < sf::seconds(0.5f))
				mSprite.move(0.0f, -1.7f * mTimer.asSeconds());

			// Scale the text label
			if (mText.getScale().x < 1.2f)
				mText.scale(1.0f + mTimer.asSeconds() / 15.0f, 1.0f + mTimer.asSeconds() / 15.0f);
		}
		else if (mCurrentType == GUI::Button::Pressed)
		{
			// Scale the sprite
			if (mSprite.getScale().x < 1.2f)
				mSprite.scale(1.0f + mTimer.asSeconds() / 15.0f, 1.0f + mTimer.asSeconds() / 15.0f);

			// Move the text up
			if (mTimer < sf::seconds(1.0f))
				mText.move(0.0f, -1.2f * mTimer.asSeconds());

			// Add blinking effect to both sprite and text
			if (mTimer < sf::seconds(1.0f))
				mShowText = !mShowText;
		}
	}

	void AnimatedButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		if (mShowText)
		{
			target.draw(mSprite, states);
			target.draw(mText, states);
		}
	}

	void AnimatedButton::resetTransformations()
	{
		mSprite.setScale(1, 1);
		mSprite.setRotation(0);
		mSprite.setPosition(0, 0);

		mText.setScale(1, 1);
		mText.setRotation(0);
	}

	void AnimatedButton::changeTexture(Type buttonType)
	{
		mCurrentType = buttonType;
		mTimer = sf::Time::Zero;

		if (buttonType != Pressed)
			resetTransformations();

		mSprite.setTextureRect(Table[mIdentifier].textureRect);
	}
}