#ifndef COMMAND_QUEUE_HPP
#define COMMAND_QUEUE_HPP

#include "commands/command.hpp"

#include <queue>

// Wrapper that transport commands to the world and the scene graph
// Using FIFO method : elements that are pushed first are also removed first
class CommandQueue
{
public:
	void push(const Command& command);
	Command pop();
	bool isEmpty() const;

private:
	std::queue<Command> mQueue;
};

#endif // COMMAND_QUEUE_HPP