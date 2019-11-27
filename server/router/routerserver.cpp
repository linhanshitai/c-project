#include "routerserver.h"
#include "log.h"
#include "common_api.h"
#include "epollthread.h"
#include "selectthread.h"
#include "routerconfig.h"
#include "timermanager.h"
#include "routerhttpsocket.h"
#include "routeragentsocket.h"
#include "routercrosssocket.h"
#include "routergamesocket.h"
CServerBase* g_pServerBase = &CRouterServer::Instance();
CRouterServer::CRouterServer() :m_timerImpl(this)
{

}
CRouterServer::~CRouterServer()
{

}
void CRouterServer::SigHandle(int sig)
{
	CRouterServer::Instance().Stop();
}
void CRouterServer::PushSocketEvent(EventType type, CSocket* pSocket)
{
	//��������������߳��е���
	while (!m_listEvent.empty() && S_OK == m_queueEvent.Push(m_listEvent.front()))
		m_listEvent.pop_front();
	EventData event;
	event.type = type;
	event.data = (void*)pSocket;
	if (m_queueEvent.IsFull())
		m_listEvent.emplace_back(event);
	else
		m_queueEvent.Push(event);
	if (!m_listEvent.empty() && 0 == m_timerImpl.GetTimerIdSize())
		m_timerImpl.AddIntervalTimer(TimerType::enmTimerType_AddEvent, 1000);
}
CClientSocket* CRouterServer::GetSocket(SessionID id)
{
	switch (id)
	{
	case enmClientType_Http:return &CRouterHttpSocket::Instance();
	case enmClientType_Server:return &CRouterGameSocket::Instance();
	case enmClientType_Proxy:return &CRouterAgentSocket::Instance();
	case enmClientType_Cross:return &CRouterCrossSocket::Instance();
	default:return nullptr;
	}
	return nullptr;
}
void CRouterServer::OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt)
{
	switch (data.type)
	{
	case TimerType::enmTimerType_AddEvent://����������̵߳��õ�
		while (!m_listEvent.empty() && S_OK == m_queueEvent.Push(m_listEvent.front()))
			m_listEvent.pop_front();
		if (!m_listEvent.empty())
			m_timerImpl.AddIntervalTimer(TimerType::enmTimerType_AddEvent, 1000);
		break;
	case TimerType::enmTimerType_SendMsg://��������̵߳��õ�
		GetSocket(data.data.u64)->SendListMsg();break;
	case TimerType::enmTimerType_SocketHeart://��������̵߳��õ�
		GetSocket(data.data.u64)->SendHeart();break;
	default: break;
	}
}
void CRouterServer::Start()
{
	g_pSocketThread = &CSingleton<CSocketThread>::Instance();
	g_pSocketThread->AddConnectSocket(&CRouterAgentSocket::Instance());
	g_pSocketThread->AddConnectSocket(&CRouterCrossSocket::Instance());
	g_pSocketThread->AddConnectSocket(&CRouterGameSocket::Instance());
	g_pSocketThread->AddConnectSocket(&CRouterHttpSocket::Instance());
	g_pSocketThread->Start();
	WRITE_INFO_LOG("start router server");
	printf("start router server\n");
	Run();
	g_pSocketThread->Stop();
	g_pSocketThread->WaitExit();
	CRouterAgentSocket::SwapInstance(nullptr);//�������߳̿������� ��������core�ļ����⴦�����������п������˳���ʱ����ʾ�ڴ�й¶
	CRouterCrossSocket::SwapInstance(nullptr);//�������߳̿������� ��������core�ļ����⴦�����������п������˳���ʱ����ʾ�ڴ�й¶
	CRouterGameSocket::SwapInstance(nullptr);//�������߳̿������� ��������core�ļ����⴦�����������п������˳���ʱ����ʾ�ڴ�й¶
	CRouterHttpSocket::SwapInstance(nullptr);//�������߳̿������� ��������core�ļ����⴦�����������п������˳���ʱ����ʾ�ڴ�й¶
	WRITE_INFO_LOG("stop router server");
	printf("stop router server\n");
}
void CRouterServer::Run()
{
	EventData event;
	bool doAllTimer = false;
	CClientSocket* pSocket = nullptr;
	uint32_t count = 0;
	while (m_run)
	{
		doAllTimer = CTimerManager::GetInstance().DoTimerEvent();
		for (count = 0; count < 50 && m_queueEvent.Pop(event) == S_OK; ++count)
		{
			pSocket = (CClientSocket*)event.data;
			switch (event.type)
			{
			case enmEventType_Connect:
				pSocket->ConnectSucc(true); break;
			case enmEventType_Remove:
				pSocket->ConnectClose();
				g_pSocketThread->AddConnectSocket(pSocket);
				break;
			case enmEventType_Data:
				if (pSocket->SpiltMsg())
					m_listSession.emplace_back(pSocket->GetSessionIndex());
				break;
			default:break;
			}
		}
		for (count = 0; count < 50 && !m_listSession.empty(); ++count)
		{
			if (GetSocket(m_listSession.front())->SpiltMsg())
				m_listSession.emplace_back(m_listSession.front());
			m_listSession.pop_front();
		}
		if (doAllTimer && m_queueEvent.IsEmpty() && m_listSession.empty())
			MySleep(1);
	}
}