#pragma once
#include "AbstractMouseDevice.h"
#include <WinSock2.h>
#include <Windows.h>

#include <pthread.h>

class TCPMouseDevice : public AbstractMouseDevice
{
private:
	SOCKET svcfd = -1;
	pthread_t receiveThread;
	fd_set fds;

public:
	TCPMouseDevice();
	~TCPMouseDevice();

public:
	int Initialize() override;
	int Release() override;
	int Connect() override;
	int Disconnect() override;

private:
	static void *ReceiveThreadProc(void *state);

private:
	bool ReadMessage(SOCKET socket, std::string &message);
	bool WriteMessage(SOCKET socket, std::string &message);
	SOCKET AcceptClient();
	void CloseClient(SOCKET s);
};

