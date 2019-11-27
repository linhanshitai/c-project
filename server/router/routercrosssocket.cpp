#include "routercrosssocket.h"
#include "serverbase.h"
#include "routerconfig.h"
CRouterCrossSocket::CRouterCrossSocket()
	:CClientSocket(CRouterConfig::Instance().GetCrossRecvSize(), CRouterConfig::Instance().GetCrossSendSize())
{
	Init(CRouterConfig::Instance().GetCrossIp().data(), CRouterConfig::Instance().GetCrossPort(), true);
	CClientSocket::SetSessionIndex(enmClientType_Cross);
	g_pServerBase->AddIntervalTimer(TimerType::enmTimerType_SocketHeart, 5, 5, GetSessionIndex());//���������ʱ��
}
CRouterCrossSocket::~CRouterCrossSocket()
{

}
void CRouterCrossSocket::ConnectSucc(bool)
{
	//����ע����Ϣ
}
void CRouterCrossSocket::DispatchMsg(const char* pMsg, uint32_t size)
{
	//���������͹�������Ϣ
}
void CRouterCrossSocket::SendHeart()
{
	if (!CanSendMsg())return;
}