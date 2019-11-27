#pragma once
#include<map>
#include<unordered_map>
#include<list>
#include "serverbase.h"
#include "newqueue.h"
#include "singleton.h"
class CClientSocket;
class CGameServer :public CServerBase,public CSingleton<CGameServer>
{
public:
	CGameServer();
	virtual ~CGameServer();
	static void SigHandle(int sig);
	virtual void PushSocketEvent(EventType type, CSocket* pSocket)override;
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt)override;
	void Start();
	void Stop() { m_run = false; }
	void RegiterRouterSocket(CClientSocket* pSocket);
	bool Init();
	virtual void SendMsg(const char* msg, uint32_t size)override;//�������ͽӿ�
	virtual void SendMsgByZone(const char* msg, uint32_t size, uint32_t serverId)override;//������ͽӿ�
	virtual void SendMsgByRole(const char* msg, uint32_t size, uint64_t roleId)override;//ͨ�÷��ͽӿ�
	uint32_t GetZoneId(uint64_t roleId);
	uint64_t GetRoleId(uint64_t sessionId);
	virtual void RegisterZone(uint64_t roleId, uint32_t serverId) { m_roleZone[roleId] = serverId; }
	virtual void RegisterRole(uint64_t roleId, uint64_t sessionId) { m_roleSession[sessionId] = roleId; }
private:
	void Run();
private:
	CNewQueue<EventData, 5 * 1024 * 1024> m_queueEvent;//�����¼�����
	std::list<EventData> m_listEvent;//m_queueEvent���˵�ʱ�������������
	std::list<SessionID> m_listSession;//һ��ѭ��û�д�������´�ѭ���ڴ����session
	std::map<SessionID, CClientSocket*> m_mapSocket;//���������б�
	CTimterEventImpl m_timerImpl;//�����߳�ʹ�õĶ�ʱ���ӿ�
	std::unordered_map<uint64_t, uint32_t> m_roleZone;
	std::unordered_map<uint64_t, uint64_t> m_roleSession;
	bool m_run = true;
};

