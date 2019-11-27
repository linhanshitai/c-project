#pragma once
#include"serverbase.h"
#include "singleton.h"
#include "newqueue.h"
#include <list>
#include <map>
class CClientSocket;
class CAgentServer :public CServerBase,public CSingleton<CAgentServer>
{
public:
	CAgentServer();
	virtual ~CAgentServer();
	static void SigHandle(int sig);
	virtual void PushSocketEvent(EventType type, CSocket* pSocket)override;
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt)override;
	void Start();
	void Stop() { m_run = false;}
	void SendMsgToSocket(SessionID id, const char* msg, uint32_t size);
private:
	void Run();
	void SendOfflineMsgToServer(SessionID id);
private:
	CNewQueue<EventData, 5 * 1024 * 1024> m_queueEvent;//�����¼�����
	std::list<EventData> m_listEvent;//m_queueEvent���˵�ʱ�������������
	std::list<SessionID> m_listSession;//һ��ѭ��û�д�������´�ѭ���ڴ����session
	std::map<SessionID, CClientSocket*> m_mapSocket;//���������б�
	CTimterEventImpl m_timerImpl;//�����߳�ʹ�õĶ�ʱ���ӿ�
	bool m_run = true;
};

