#pragma once

#include "AbstractMouseDevice.h"

class MouseDispatcher :public AbstractAMDEventHandler
{
private:
	AbstractMouseDevice *mouseDev = nullptr;
	int originalX = 0;
	int originalY = 0;

public:
	MouseDispatcher();
	~MouseDispatcher();

public:
	int Initialize();
	int Run();
	int Release();

	void MouseDeviceConnectedHandler();
	void MouseDeviceDisconnectedHandler();
	void MouseDeviceMessageReceivedHandler(std::string message);
};

