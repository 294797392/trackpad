#include "MouseDispatcher.h"
#include "MouseDeviceFactory.h"

MouseDispatcher::MouseDispatcher()
{
	this->mouseDev = MouseDeviceFactory::Create(MouseDevices::TCP);
	this->mouseDev->SetEventHandler(this);
}

MouseDispatcher::~MouseDispatcher()
{
}

int MouseDispatcher::Initialize()
{
	return this->mouseDev->Initialize();
}

int MouseDispatcher::Run()
{
	return this->mouseDev->Connect();
}

int MouseDispatcher::Release()
{
	return 0;
}

void MouseDispatcher::MouseDeviceConnectedHandler()
{
}

void MouseDispatcher::MouseDeviceDisconnectedHandler()
{
}

void MouseDispatcher::MouseDevicePositionChangedHandler(int offsetX, int offsetY)
{
}