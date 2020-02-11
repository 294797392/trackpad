#pragma once
#include "AbstractMouseDevice.h"
#include <WinSock2.h>
#include <Windows.h>

#include <pthread.h>

class TCPMouseDevice : public AbstractMouseDevice
{
private:
	SOCKET socketHandle = -1;
	pthread_t acceptThread;

public:
	TCPMouseDevice();
	~TCPMouseDevice();

public:
	int Initialize() override;
	int Release() override;
	int Connect() override;
	int Disconnect() override;


private:
	static void *AcceptClientThreadProc(void *state);
};

