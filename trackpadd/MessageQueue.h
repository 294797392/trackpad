#pragma once

#include <string>

class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

public:
	void Enqueue(std::string message);
	std::string Dequeue();
};

