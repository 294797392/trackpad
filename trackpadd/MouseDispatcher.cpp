#include "MouseDispatcher.h"
#include "MouseDeviceFactory.h"
#include "Logger.h"
#include "MessageID.h"

#include <string>
#include <iostream>
#include <Windows.h>

#include <jsoncpp/json.h>

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

void MouseDispatcher::MouseDeviceMessageReceivedHandler(std::string message)
{
	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(message, value))
	{
		LOGE("invalid message format, " << message.c_str());
		return;
	}

	int type = value["type"].asInt();
	int x = value["x"].asInt();
	int y = value["y"].asInt();

	switch (type)
	{
		case MessageID::MouseMovingBegin:
		{
			POINT pos;
			if (!GetCursorPos(&pos))
			{
				LOGE("GetCursorPos failed, " << GetLastError());
				return;
			}

			this->originalX = pos.x;
			this->originalY = pos.y;
		}
		break;

		case MessageID::MouseMoving:
		{
			int newX = this->originalX + x;
			int newY = this->originalY + y;

			if (!SetCursorPos(newX, newY))
			{
				LOGE("SetCursorPos failed, " << GetLastError());
				return;
			}
		}
		break;

		case MessageID::MouseMovingEnd:
		{}
		break;

		default:
			break;
	}
}