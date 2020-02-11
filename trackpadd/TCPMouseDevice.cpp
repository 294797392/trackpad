#include "TCPMouseDevice.h"
#include "TrackpadCode.h"
#include "DefaultValues.h"
#include "Logger.h"

#include <errno.h>

TCPMouseDevice::TCPMouseDevice()
{
}

TCPMouseDevice::~TCPMouseDevice()
{
}

int TCPMouseDevice::Initialize()
{
	int rc = 0;
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);

	this->socketHandle = socket(PF_INET, SOCK_STREAM, 0);

	// 设置套接字选项避免地址使用错误
	char on = 1;
	if ((rc = setsockopt(this->socketHandle, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(char))) < 0)
	{
		LOGE("setsockopt failed, " << WSAGetLastError());
		return TPCODE_FAIL;
	}

	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(DefaultValues::TCPServicePort);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	if ((rc = bind(this->socketHandle, (struct sockaddr*)&bdaddr, sizeof(struct sockaddr))) < 0)
	{
		LOGE("bind failed, " << errno);
		return TPCODE_FAIL;
	}

	if ((rc = listen(this->socketHandle, 5)) < 0)
	{
		LOGE("pushsvc listen failed, " << errno);
		return TPCODE_FAIL;
	}

	pthread_create(&this->acceptThread, NULL, this->AcceptClientThreadProc, this);
	pthread_detach(this->acceptThread);

	LOGI("TCPMouseDevice listen at " << DefaultValues::TCPServicePort << " port");

	return TPCODE_OK;
}

int TCPMouseDevice::Release()
{
	return TPCODE_OK;
}

int TCPMouseDevice::Connect()
{
	return TPCODE_OK;
}

int TCPMouseDevice::Disconnect()
{
	return TPCODE_OK;
}

void *TCPMouseDevice::AcceptClientThreadProc(void *state)
{
	TCPMouseDevice *mouseDev = static_cast<TCPMouseDevice*>(state);

	while (true)
	{
		struct sockaddr_in client_addr;
		int sin_size = sizeof(struct sockaddr_in);
		int clientfd = 0;
		if ((clientfd = accept(mouseDev->socketHandle, (struct sockaddr*)&client_addr, &sin_size)) == -1)
		{
			LOGE("accept clinet failed, " << errno);
			continue;
		}

		LOGI("client connected..");

		/* 使用inet_ntop函数和ntohs函数转换客户端IP地址和端口号 */
		//char ipaddr[64] = { '\0' };
		//inet_ntop(AF_INET, &client_addr.sin_addr, ipaddr, INET6_ADDRSTRLEN);
		//int port = ntohs(client_addr.sin_port);

		//printf("client connected, ipaddr=%s, port=%d\n", ipaddr, port);
	}

	return nullptr;
}