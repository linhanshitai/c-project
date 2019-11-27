#pragma once
#include <map>
#include "singleton.h"
#include "socketthread.h"
#ifndef WIN32
#include <sys/epoll.h>
#endif
//Linux��ʹ�������߳� epollģʽ
class CSocket;
class CClientSocket;
class CEpollThread :public SocketThreadBase
{
public:
	CEpollThread();
	virtual ~CEpollThread();
	void virtual Run() override;
	virtual int32_t EpollMod(CSocket *pSocket)override;
	virtual int32_t EpollAdd(CSocket *pSocket)override;
	virtual bool Init()override;
private:
	int32_t DoEvent(const int32_t nCount);
	virtual bool ErrorSocket(CSocket *pSocket)override;
	virtual int32_t EpollDelete(const SOCKET fd)override;
private:
	int32_t m_FdEpoll;
	epoll_event m_pEvent[MaxEpollSocket];
};

