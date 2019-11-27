#pragma once
#include<map>
#include<list>
#include "newqueue.h"
#include "serverbase.h"
#include "singleton.h"
class CClientSocket;
class CHttpServer :public CServerBase, public CSingleton<CHttpServer>
{
public:
	CHttpServer();
	virtual ~CHttpServer();
	static void SigHandle(int sig);
	virtual void PushSocketEvent(EventType type, CSocket* pSocket)override;
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt)override;
	void Start();
	void Stop() { m_run = false; }
	void SendMsgToSocket(SessionID id, const char* msg, uint32_t size);//http���ط��ͺ���
	bool SendMsgToZone(uint32_t id, const char* msg, uint32_t size);//���͵�����������
	void RegiterHttpSocket(CClientSocket* pSocket);
	void RegiterRouterSocket(CClientSocket* pSocket);
private:
	void Run();
private:
	CNewQueue<EventData, 5 * 1024 * 1024> m_queueEvent;//�����¼�����
	std::list<EventData> m_listEvent;//m_queueEvent���˵�ʱ�������������
	std::list<SessionID> m_listSession;//һ��ѭ��û�д�������´�ѭ���ڴ����session
	std::map<SessionID, CClientSocket*> m_mapSocket;//���������б�
	std::map<uint32_t, CClientSocket*> m_zoneSocket;//�����������б�
	CTimterEventImpl m_timerImpl;//�����߳�ʹ�õĶ�ʱ���ӿ�
	bool m_run = true;
};

