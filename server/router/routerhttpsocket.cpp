#include "routerhttpsocket.h"
#include "serverbase.h"
#include "routerconfig.h"
CRouterHttpSocket::CRouterHttpSocket()
	:CClientSocket(CRouterConfig::Instance().GetHttpRecvSize(), CRouterConfig::Instance().GetHttpSendSize())
{
	Init(CRouterConfig::Instance().GetHttpIp().data(), CRouterConfig::Instance().GetHttpPort(), true);
	CClientSocket::SetSessionIndex(enmClientType_Http);
	g_pServerBase->AddIntervalTimer(TimerType::enmTimerType_SocketHeart, 5, 5, GetSessionIndex());
}
CRouterHttpSocket::~CRouterHttpSocket()
{

}
void CRouterHttpSocket::ConnectSucc(bool)
{
	//����ע����Ϣ
}
void CRouterHttpSocket::DispatchMsg(const char* pMsg, uint32_t size)
{
	//����http���������͹�������Ϣ
}
 void CRouterHttpSocket::SendHeart()
{
	 if (!CanSendMsg())return;
}