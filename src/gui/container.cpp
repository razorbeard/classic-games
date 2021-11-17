#include "container.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	Container::Container()
		: mChildren()
		, mSelectedChild(-1)
	{
	}

	void Container::pack(Component::Ptr component)
	{
		// Add a component to the container
		mChildren.push_back(component);

		// Check if the incoming child is selectable and select it
		if (!hasSelection() && component->isSelectable())
			select(mChildren.size() - 1);
	}

	bool Container::isSelectable() const
	{
		// Return false : Container is not a selectable component
		return false;
	}

	void Container::handleEvent(const sf::Event& event)
	{
		// If we have selected a child then give it events
		if (hasSelection() && mChildren[mSelectedChild]->isActive())
		{
			mChildren[mSelectedChild]->handleEvent(event);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Down)
			{
				selectNext();
			}
			else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
			{
				if (hasSelection())
					mChildren[mSelectedChild]->activate();
			}
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (const Component::Ptr& child : mChildren)
		{
			target.draw(*child, states);
		}
	}

	void Container::update(sf::Time dt)
	{
		// Update children
		for (const Component::Ptr& child : mChildren)
		{
			child->update(dt);
		}
	}

	bool Container::hasSelection() const
	{
		// Check if we have a valid selection
		// Valid means a value equals to 0 or more
		return mSelectedChild >= 0;
	}

	void Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
				mChildren[mSelectedChild]->deselect();

			mChildren[index]->select();
			mSelectedChild = (int)index;
		}
	}

	void Container::selectNext()
	{
		if (!hasSelection())
			return;

		// Search next component that is selectable, wrap around if necessary
		int next{ mSelectedChild };
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

		// Select that component
		select(next);
	}

	void Container::selectPrevious()
	{
		if (!hasSelection())
			return;

		// Search previous component that is selectable, wrap around if necessary
		int prev{ mSelectedChild };
		do
			prev = (prev + (int)mChildren.size() - 1) % (int)mChildren.size();
		while (!mChildren[prev]->isSelectable());

		// Select that component
		select(prev);
	}
}