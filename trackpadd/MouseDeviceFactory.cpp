#include "MouseDeviceFactory.h"
#include "TCPMouseDevice.h"

AbstractMouseDevice *MouseDeviceFactory::Create(MouseDevices type)
{
	switch (type)
	{
		case TCP:
			return new TCPMouseDevice();

		default: 
			return nullptr;
	}
}