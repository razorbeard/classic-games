#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "commands/category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

// Any scene node can be triggered by a function object
struct Command
{
	using Action = std::function<void(SceneNode&, sf::Time)>;

	Command();

	Action action;
	unsigned int category;
};

// Template used to avoid downcast on the scene node
template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}

#endif // COMMAND_HPP