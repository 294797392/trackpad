#pragma once

#include <string>

class AbstractAMDEventHandler
{
public:
	virtual void MouseDeviceConnectedHandler() = 0;
	virtual void MouseDeviceDisconnectedHandler() = 0;
	virtual void MouseDeviceMessageReceivedHandler(std::string message) = 0;
};

class AbstractMouseDevice
{
protected:
	AbstractAMDEventHandler *handler = nullptr;

public:
	AbstractMouseDevice();
	~AbstractMouseDevice();

public:
	virtual int Initialize() = 0;
	virtual int Release() = 0;
	virtual int Connect() = 0;
	virtual int Disconnect() = 0;

public:
	void SetEventHandler(AbstractAMDEventHandler *handler);
};

