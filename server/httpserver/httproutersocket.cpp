#include "httproutersocket.h"
#include "httpconfig.h"
#include "socketthread.h"
#include "httpserver.h"
CHttpRouterSocket::CHttpRouterSocket()
	:CClientSocket(CHttpConfig::Instance().GetClientRecvSize(),
		CHttpConfig::Instance().GetClientSendSize())
	, m_actTime(time(nullptr))
{
}
CHttpRouterSocket::~CHttpRouterSocket()
{
}
CSocket* CHttpRouterSocket::CreateClient()
{
	return new CHttpRouterSocket();
}
bool CHttpRouterSocket::OnAccept()
{
	g_pSocketThread->AddTimerSec(TimerType::enmTimerType_SocketAct, MAXSOCKETCLOSETIME, MAXSOCKETCLOSETIME, GetSessionIndex());
	return true;
}
void CHttpRouterSocket::DispatchMsg(const char* pMsg, uint32_t size)
{
	//��Ϣ����
	CHttpServer::Instance().RegiterRouterSocket(this);//�յ�ע����Ϣ��ʱ��
}