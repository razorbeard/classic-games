#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "component.hpp"

#include <vector>
#include <memory>

namespace GUI
{
	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
		Container();

		void pack(Component::Ptr component);

		virtual bool isSelectable() const override;
		virtual void handleEvent(const sf::Event& event) override;
		virtual void update(sf::Time dt) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool hasSelection() const;
		void select(std::size_t index);
		void selectNext();
		void selectPrevious();

	private:
		std::vector<Component::Ptr> mChildren;
		int mSelectedChild;
	};
}

#endif // CONTAINER_HPP