#pragma once

#include "AbstractMouseDevice.h"

class MouseDispatcher :public AbstractAMDEventHandler
{
private:
	AbstractMouseDevice *mouseDev = nullptr;

public:
	MouseDispatcher();
	~MouseDispatcher();

public:
	int Initialize();
	int Run();
	int Release();

	void MouseDeviceConnectedHandler();
	void MouseDeviceDisconnectedHandler();
	void MouseDevicePositionChangedHandler(int offsetX, int offsetY);
};

