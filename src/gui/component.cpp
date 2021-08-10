#include "component.hpp"

namespace GUI
{
	Component::Component()
		: mIsSelected(false)
		, mIsActive(false)
	{
	}

	Component::~Component()
	{
	}

	void Component::update(sf::Time dt)
	{
		// Do nothing by default
	}

	bool Component::isSelected() const
	{
		return mIsSelected;
	}

	void Component::select()
	{
		mIsSelected = true;
	}

	void Component::deselect()
	{
		mIsSelected = false;
	}

	bool Component::isActive() const
	{
		return mIsActive;
	}

	void Component::activate()
	{
		mIsActive = true;
	}

	void Component::deactivate()
	{
		mIsActive = false;
	}
}