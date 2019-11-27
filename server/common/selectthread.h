#pragma once
#include "singleton.h"
#include "socketthread.h"
//window��ʹ�õ������߳� selectģʽ
class CSelectThread :public SocketThreadBase
{
public:
	CSelectThread(void);
	~CSelectThread(void);
	void virtual Run() override;
	virtual int32_t EpollAdd(CSocket *pSocket)override;
private:
	bool DoEvent();
	virtual bool ErrorSocket(CSocket *pSocket)override;
	virtual int32_t EpollDelete(const SOCKET fd)override;
private:
	fd_set m_FdSet;
	fd_set m_Read;
	fd_set m_Write;
	fd_set m_Error;
};

