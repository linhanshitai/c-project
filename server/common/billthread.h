#ifndef _BILL_THREAD_H_
#define _BILL_THREAD_H_
#include <string>
#include <list>
#include <mysql.h>
#include "thread.h"
#include "singleton.h"
#include "newqueue.h"
#include "msgbase.h"
/*
���ݿ���־�߳�
���ʵ�ֶ��߳̿��Խ�m_sql����Ϊһ�����飬ÿ���߳�ִֻ��һ��ָ�����±꣬
�����е����Ӿ��ȷ��䵽ָ�����±������С������Ҿ���һ���߳��㹻.�������־����ܶ�
����Ҳ�����ö��m_sqlʵ�ֶ��̣߳���Ȼǰ�����߼����߳� Ҳ��������������
*/
class CBillThread:public CThread,public CSingleton<CBillThread>
{
private:
	struct SqlConnInfo
	{
		MYSQL* conn=nullptr;//���ݿ�����sql�߳�ʹ��
		std::string host;
		std::string user;
		std::string password;
		std::string dbName;
		uint16_t port=0;
	};
public:
	CBillThread();
	~CBillThread();
	int32_t Init(const std::string& host,const std::string& user,const std::string& password,
		const std::string& dbName,uint16_t port,ServerID serverId=0);
	void SetCross(){m_cross=true;}
	void virtual Run()override;
	//���ط�serverId�ᱻ����
	void PostBill(const char*pSql,uint32_t nSize,ServerID serverId=0);
	void PostBill(const std::string& szSql,ServerID serverId=0)
	{PostBill(szSql.data(),(uint32_t)szSql.size());}
	void PostBill(std::string&& szSql,ServerID serverId=0)
	{PostBill(szSql.data(),(uint32_t)szSql.size());}
	void PostBill(CBillBase* pBill,ServerID serverId=0);
	void PostBill(CBillBase* pBill,const CRole* pRole,ServerID serverId=0)
	{
		if(likely(pRole != nullptr))
			PostBill(pBill,*pRole,serverId);
	}
	void PostBill(CBillBase* pBill,const CRole& role,ServerID serverId=0);
private:
	int32_t Connect(std::shared_ptr<SqlConnInfo>& pInf);
	void LocalRun();
	void CrossRun();
	ServerID GetServerID(RoleID roleId){return 0;}
private:
	CNewQueue<char,5*1024*1024> m_sql;
	std::list<std::string> m_list;//������������ϲ�Ӧ�ñ��õ����õ����ǹ��ƾ͵�������������
	std::map<ServerID,std::shared_ptr<SqlConnInfo>> m_ConnMap;
	MYSQL* m_Conn;
	bool m_cross;
};
#endif