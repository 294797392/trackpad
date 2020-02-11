#include "AbstractMouseDevice.h"



AbstractMouseDevice::AbstractMouseDevice()
{
}


AbstractMouseDevice::~AbstractMouseDevice()
{
}

void AbstractMouseDevice::SetEventHandler(AbstractAMDEventHandler *handler)
{
	this->handler = handler;
}