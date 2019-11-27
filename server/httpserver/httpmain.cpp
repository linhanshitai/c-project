// httpmain.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include "common_api.h"
#include "httpserver.h"
#include "httpconfig.h"
#include "log.h"
#include "logthread.h"
int main(int argc, char* argv[])
{
	if (!CommonInit(argc, argv))
		return 0;
	RegistSignal(&CHttpServer::SigHandle);
	CHttpConfig::Instance().SetPath(GetDir(argv[0]));
	g_log->SetFileName(GetServerName(argv[0]).data());
	std::string logpath = CHttpConfig::Instance().GetPath() + "log";
	CreateDir(logpath.data());
	CLogThread::Instance().Start();
	if (CHttpConfig::Instance().LoadCfg())
		CHttpServer::Instance().Start();
	CLogThread::Instance().Stop();
	CLogThread::Instance().WaitExit();
	return 0;
}
// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�

