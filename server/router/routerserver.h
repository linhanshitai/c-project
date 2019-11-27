#pragma once
#include"serverbase.h"
#include "singleton.h"
#include "newqueue.h"
#include <list>
#include <unordered_set>
class CClientSocket;
class CRouterServer :public CServerBase,public CSingleton<CRouterServer>
{
public:
	CRouterServer();
	virtual ~CRouterServer();
	static void SigHandle(int sig);
	virtual void PushSocketEvent(EventType type, CSocket* pSocket)override;
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt)override;
	void Start();
	void Stop() { m_run = false; }
private:
	void Run();
	CClientSocket* GetSocket(SessionID id);
private:
	CNewQueue<EventData, 5 * 1024 * 1024> m_queueEvent;//�����¼�����
	std::list<EventData> m_listEvent;//m_queueEvent���˵�ʱ�������������
	std::list<SessionID> m_listSession;//һ��ѭ��û�д�������´�ѭ���ڴ����session
	CTimterEventImpl m_timerImpl;//�����߳�ʹ�õĶ�ʱ���ӿ�
	bool m_run = true;
	std::unordered_set<uint64_t> m_crossSession;
	std::unordered_set<uint64_t> m_crossRole;
};

