#include "gui/button.hpp"
#include "utility.hpp"
#include "resources/resource_holder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	// Build a text button
	Button::Button(State::Context context)
		: mCallback()
		, mCurrentType(Normal)
		, mTimer(sf::Time::Zero)
		, mText("", context.fonts->get(Fonts::Upheaval))
		, mSounds(*context.sounds)
		, mShowText(true)
		, mIsToggle(false)
	{
		centerOrigin(mText);
		changeTexture(Normal);
	}

	void Button::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	void Button::setText(const std::string& text)
	{
		mText.setString(text);
		centerOrigin(mText);
	}

	void Button::setText(const std::string& text, unsigned int size)
	{
		mText.setString(text);
		mText.setCharacterSize(size);
		centerOrigin(mText);
	}

	void Button::setText(const std::string& text, unsigned int size, float thickness)
	{
		mText.setString(text);
		mText.setCharacterSize(size);
		mText.setOutlineThickness(thickness);
		centerOrigin(mText);
	}

	void Button::setToggle(bool flag)
	{
		mIsToggle = flag;
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();

		changeTexture(Selected);
	}

	void Button::deselect()
	{
		Component::deselect();

		changeTexture(Normal);
	}

	void Button::activate()
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
	}

	void Button::deactivate()
	{
		// Button has been released
		Component::deactivate();

		if (mIsToggle)
		{
			// Reset texture to right one depending on if we are selected or not
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
	}

	void Button::handleEvent(const sf::Event&)
	{
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		if (mShowText)
			target.draw(mText, states);
	}

	void Button::update(sf::Time dt)
	{
		mTimer += dt;

		// Modify text color
		if (mCurrentType == Normal)
		{
			mText.setFillColor(sf::Color(175, 175, 175, 255)); // Grey
		}
		else if (mCurrentType == Selected)
		{
			// Generate blinking effect
			if (mTimer >= sf::seconds(0.2f))
			{
				mShowText = !mShowText;
				mTimer = sf::Time::Zero;
			}

			mText.setFillColor(sf::Color::Yellow);
		}
		else if (mCurrentType == Pressed)
		{
			mText.setFillColor(sf::Color::Red);
		}
	}

	void Button::changeTexture(Type buttonType)
	{
		mCurrentType = buttonType;
		mTimer = sf::Time::Zero;

		// Reset text visibility
		mShowText = true;
	}
}