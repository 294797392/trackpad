#include "TCPMouseDevice.h"
#include "TrackpadCode.h"
#include "DefaultValues.h"
#include "Logger.h"

#include <errno.h>
#include <fcntl.h>
#include <io.h>

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

	this->svcfd = socket(PF_INET, SOCK_STREAM, 0);

	// �����׽���ѡ������ַʹ�ô���
	char on = 1;
	if ((rc = setsockopt(this->svcfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(char))) < 0)
	{
		LOGE("setsockopt failed, " << WSAGetLastError());
		return TPCODE_FAIL;
	}

	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(DefaultValues::TCPServicePort);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	if ((rc = bind(this->svcfd, (struct sockaddr*)&bdaddr, sizeof(struct sockaddr))) < 0)
	{
		LOGE("bind failed, " << errno);
		return TPCODE_FAIL;
	}

	if ((rc = listen(this->svcfd, 5)) < 0)
	{
		LOGE("pushsvc listen failed, " << errno);
		return TPCODE_FAIL;
	}

	FD_ZERO(&this->fds);
	FD_SET(this->svcfd, &this->fds);

	pthread_create(&this->receiveThread, NULL, TCPMouseDevice::ReceiveThreadProc, this);
	pthread_detach(this->receiveThread);
	
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

SOCKET TCPMouseDevice::AcceptClient()
{
	struct sockaddr_in client_addr;
	int sin_size = sizeof(struct sockaddr_in);
	int clientfd = 0;
	if ((clientfd = accept(this->svcfd, (struct sockaddr*)&client_addr, &sin_size)) == -1)
	{
		LOGE("accept clinet failed, " << WSAGetLastError());
		return -1;
	}

	LOGI("client connected..");

	return clientfd;

	/* ʹ��inet_ntop������ntohs����ת���ͻ���IP��ַ�Ͷ˿ں� */
	//char ipaddr[64] = { '\0' };
	//inet_ntop(AF_INET, &client_addr.sin_addr, ipaddr, INET6_ADDRSTRLEN);
	//int port = ntohs(client_addr.sin_port);

	//printf("client connected, ipaddr=%s, port=%d\n", ipaddr, port);
}

void *TCPMouseDevice::ReceiveThreadProc(void *state)
{
	TCPMouseDevice *mouseDev = static_cast<TCPMouseDevice*>(state);

	while (true)
	{
		int ret;

		/*
		 * Ϊ����Ч�ʣ����԰�Ҫ��ص��ļ���������������
		 * �ڵ���select֮ǰ����ֵ��һ����ʱ����ֱ�Ӵ��ݸ�select�����Ϳ���
		 */
		fd_set readfds = mouseDev->fds;
		fd_set writefds = mouseDev->fds;
		fd_set exceptfds = mouseDev->fds;

		/* ���õȴ��ļ��������¼��ĳ�ʱʱ�� */
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 20 * 1000;

		ret = select(0, &readfds, &writefds, &exceptfds, &tv);
		if (ret == 0)
		{
			/* timeout */
			continue;
		}
		else if (ret < 0)
		{
			/* error */
			LOGE("socket select failed, " << WSAGetLastError());
			switch (errno)
			{
				case EBADF: // ���ڷǷ��ļ�������
					break;

				case EINTR: // �ȴ�ʱ������һ���źţ��������·������
					continue;

				case EINVAL: // �����Ƿ�����һ�������Ǹ������߳�ʱʱ��Ƿ�
					break;

				case ENOMEM: // û���ڴ���
					break;
			}
		}
		else
		{
			/* one or more fd has io event */
			u_int count = readfds.fd_count;
			for (u_int i = 0; i < count; i++)
			{
				SOCKET fd = readfds.fd_array[i];

				if (fd == mouseDev->svcfd)
				{
					SOCKET clientfd = mouseDev->AcceptClient();
					FD_SET(clientfd, &mouseDev->fds);
				}
				else
				{
					std::string message;
					if (mouseDev->ReadMessage(fd, message))
					{
						// �յ�������
						if (message.empty())
						{
							LOGI("receive empty message, disconnect");
							mouseDev->CloseClient(fd);
						}
						else
						{
							if (mouseDev->handler != nullptr)
							{
								mouseDev->handler->MouseDeviceMessageReceivedHandler(message);
							}
						}
					}
					else
					{
						// �ͻ��˶���
						LOGI("client disconnected, " << fd);
						mouseDev->CloseClient(fd);
					}
				}
			}
		}
	}

	return nullptr;
}

bool TCPMouseDevice::ReadMessage(SOCKET socket, std::string &message)
{
	size_t ret;
	char c[2] = { '\0' };
	while ((ret = recv(socket, c, 1, 0)) != 0)
	{
		if (ret == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				LOGE("read message failed, " << WSAGetLastError());
				return false;
			}
		}
		else
		{
			if (c[0] == '\n')
			{
				return true;
			}
			if (c[0] != '\0' && c[0] != '\r')
			{
				message.append(c);
			}
		}
	}

	return true;
}

bool TCPMouseDevice::WriteMessage(SOCKET socket, std::string &message)
{
	size_t ret;
	size_t l = message.length();
	const char *m = message.c_str();
	while (l != 0 && (ret = send(socket, m, l, 0)) != 0)
	{
		if (ret == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				LOGE("write message failed, " << WSAGetLastError());
				return false;
			}
		}

		l -= ret;
		m += ret;
	}

	return true;
}

void TCPMouseDevice::CloseClient(SOCKET fd)
{
	FD_CLR(fd, &this->fds);
	shutdown(fd, SD_BOTH);
	closesocket(fd);
}