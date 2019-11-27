#pragma once
#include <map>
#include <list>
#include "thread.h"
#include "timerbase.h"
#include"singleton.h"
#include "newqueue.h"
class CSocket;
class CClientSocket;
class SocketThreadBase :public CThread,public CTimerBase
{
public:
	SocketThreadBase();
	virtual ~SocketThreadBase();
	virtual int32_t EpollAdd(CSocket* pSocket);
	virtual int32_t EpollMod(CSocket *pSocket) { return S_OK; }
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt)override;
	void AddConnectSocket(CClientSocket* pSocket);
	void AddKickSession(SessionID id);
	SessionID GetSessionID() { return ++m_nSeesionIndex; }
	virtual SSL* GetNewSSL() { return nullptr; }
protected:
	virtual int32_t EpollDelete(const SOCKET fd) { return S_OK; }
	virtual bool ErrorSocket(CSocket *pSocket);
	virtual bool Init();
	void virtual Run() override;
	void AddConnectSocket();
	void KickSession();
	bool CanSleep()const {return  m_doAllTimer && m_waitConnect.IsEmpty() && m_kickSession.IsEmpty();}
protected:
	std::map<SessionID, CSocket*> m_ClientSocket;
private:
	SessionID m_nSeesionIndex;
	CTimterEventImpl m_impl;//�����߳�ʹ�õĶ�ʱ���ӿ�
	bool m_doAllTimer = false;

	CNewQueue<CClientSocket*> m_waitConnect;//�ȴ����ӵ�socket���߳�push �����߳�pop
	std::list<CClientSocket*> m_connectList;

	CNewQueue<SessionID> m_kickSession;//���߳��ߵ�
	std::list<SessionID> m_kickList;
};
extern SocketThreadBase* g_pSocketThread;