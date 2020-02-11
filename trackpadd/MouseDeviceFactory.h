#pragma once

#include "AbstractMouseDevice.h"
#include "MouseDevices.h"

class MouseDeviceFactory
{
public:
	static AbstractMouseDevice *Create(MouseDevices type);
};

