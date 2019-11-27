#include "gameroutersocket.h"
#include "gameconfig.h"
#include "socketthread.h"
#include "singleton.h"
#include "msgmanager.h"
CGameRouterSocket::CGameRouterSocket()
	:CClientSocket(CGameConfig::Instance().GetGameRecvSize(),
		CGameConfig::Instance().GetGameSendSize())
	, m_actTime(time(nullptr))
{
}
CGameRouterSocket::~CGameRouterSocket()
{
}
CSocket* CGameRouterSocket::CreateClient()
{
	if (CGameConfig::Instance().IsCrossServer())
		return new CGameRouterSocket;
	auto pSocket = &CSingleton<CGameRouterSocket>::GetInstance();
	pSocket->Reinit();
	return pSocket;
}
bool CGameRouterSocket::OnAccept()
{
	if (CGameConfig::Instance().IsCrossServer())
		CClientSocket::OnAccept();
	else
		g_pSocketThread->AddTimerSec(TimerType::enmTimerType_SocketAct, MAXSOCKETCLOSETIME, MAXSOCKETCLOSETIME, GetSessionIndex());
	return true;
}
int32_t CGameRouterSocket::EpollErr()
{
	if (CGameConfig::Instance().IsCrossServer())
		return CClientSocket::EpollErr();
	return CSocket::EpollErr();
}
void CGameRouterSocket::DispatchMsg(const char* pMsg, uint32_t size)
{
	//������Ϣ�� ���ڷ�����֮ǰ����Ϣֱ����sessionid��Ϊ key ǰ�˷��ͻ���gm���͵���Ϣֱ������Ϣ�е�key
	CMsgManager::Instance().OnMsgEvent(pMsg, size,GetKey());
 }