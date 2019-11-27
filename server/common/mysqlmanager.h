#ifndef _MYSQL_MANAGER_H
#define _MYSQL_MANAGER_H
#include <list>
#include <string>
#include <mysql.h>
#include <map>
#include "functionevent.h"
#include "mysqlevent.h"
#include "newqueue.h"
#include "log.h"
#include "singleton.h"
#include "timerbase.h"
#include "thread.h"
#if 0//Ԥ������Լ���ͷ�ļ�����
struct st_mysql;
typedef st_mysql MYSQL;
struct st_mysql_res;
typedef st_mysql_res MYSQL_RES;
typedef char** MYSQL_ROW;
#endif
//���ݿ��߳�
class CMysqlManager:public CThread,public CTimerBase,public CSingleton<CMysqlManager>
{
private:
	struct SqlConnInfo
	{
		MYSQL* conn = nullptr;//���ݿ�����sql�߳�ʹ��
		std::string host;
		std::string user;
		std::string password;
		std::string dbName;
		uint16_t port = 0;
	};
public:
	CMysqlManager();
	~CMysqlManager();
	int32_t Init(const std::string& host,const std::string& user,const std::string& password,
		const std::string& dbName,uint16_t port,ServerID serverId);
	void SetCross(){m_cross=true;m_emptyEvent.NewServerID();}
	void virtual Run()override;
public:
	////���ط�id�ᱻ����
	//��Ҫ���̵߳Ļ��ӿ���Ҫ���̱߳�ǲ�����Ҫ����
	//���½ӿ�û�лص���������delete update insert
	void PostSql(char* pSql,uint32_t nSize,ServerID id=0);
	//���½ӿڵ���void CallBack(uint32_t nAffectRow)����delete update insert
	void PostSql(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,ServerID id=0);
	//���½ӿڵ���void CallBack(uint32_t nAffectRow,MysqlData data)����delete update insert
	void PostSql(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,FuncData data,ServerID id=0);
	//���½ӿڵ���void CallBack(uint32_t nAffectRow,uint64_t nInsertID) insert
	void PostInsert(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,ServerID id=0);
	//���½ӿڵ���void CallBack(uint32_t nAffectRow,uint64_t nInsertID,MysqlData data)insert
	void PostInsert(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,FuncData data,ServerID id=0);
	//���½ӿ�ֱ�Ӵ���һ��lambda���� ���÷�ʽΪhandle(CMysqlManager*)
	template< class Handler>
	void PostSql(Handler handle,char* pSql,uint32_t nSize,ServerID id=0,bool bstore=false)
	{
		if (m_sql.AddOnlyBuf(pSql, nSize) == S_OK)
			PostSql(new CMysqlEventFunc<Handler>(nullptr, nSize, handle, bstore, id));
		else
			PostSql(new CMysqlEventFunc<Handler>(pSql,nSize,handle,bstore,id));
	}
	//���½ӿ�ֱ�Ӵ���һ��lambda���� ���÷�ʽΪhandle(CMysqlManager*��data)
	template< class Handler>
	void PostSql(Handler handle,char* pSql,uint32_t nSize,FuncData data,ServerID id=0,bool bstore=false)
	{
		if (m_sql.AddOnlyBuf(pSql,nSize) == S_OK)
			PostSql(new CMysqlEventFuncData<Handler>(nullptr,nSize,handle,data,bstore,id));
		else
			PostSql(new CMysqlEventFuncData<Handler>(pSql,nSize,handle,data,bstore,id));
	}
	//���½ӿ�����select ����һ��std::shared_ptr<CMysqlRecord>����
	template< class T>
	void PostSql(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,ServerID id=0)
	{
		if (m_sql.AddOnlyBuf(pSql,nSize) == S_OK)
			PostSql(new CMysqlEventSelect<T>(nullptr,nSize,pCallBack,id));
		else
			PostSql(new CMysqlEventSelect<T>(pSql,nSize,pCallBack,id));
	}
	//���½ӿ�����select ����һ��std::shared_ptr<CMysqlRecord>����
	template< class T>
	void PostSql(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,FuncData data,ServerID id=0)
	{
		if (m_sql.AddOnlyBuf(pSql,nSize) == S_OK)
			PostSql(new CMysqlEventSelectData<T>(nullptr,nSize,pCallBack,data,id));
		else
			PostSql(new CMysqlEventSelectData<T>(pSql,nSize,pCallBack,data,id));
	}
	//���½ӿ�����select ����һ��std::shared_ptr<CMysqlRecord>���� ��CreateRecord����
	void PostSql(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,uint32_t nCase,ServerID id=0);
	//���½ӿ�����select ����һ��std::shared_ptr<CMysqlRecord>���� ��CreateRecord����
	void PostSql(CMysqlCallBack* pCallBack,char* pSql,uint32_t nSize,FuncData data,uint32_t nCase,ServerID id=0);
public:
	uint32_t GetColNum()const {return m_numFields;}
	uint32_t GetAffectRow();
	uint64_t GetInsertID();
	std::shared_ptr<CMysqlRecord> CreateRecord(uint32_t nCase);
	//sql�߳���һ�����У�������̶߳��������ȴ��������ָ��
	void PushCallBack(CFuncEventBase* pEvent);
	bool PopCallBack(CFuncEventBase*& pEvent) { return m_funEvent.Pop(pEvent) == S_OK; }
	bool CallBackEmpty() { return m_funEvent.IsEmpty(); }
	//sql�߳��������߳��Լ�������ָ��
	 void PostSql(CMysqlEventBase* pEvent);
	 virtual void OnTimerEvent(const TimerDataInfo& data,const CDateTime& dt)override;
	 void CheckSqlEvent();
	 char* GetBuildSql() {return buildSql;}
private:
	//���߳�push sql�߳�pop ��������
	CNewQueue<CMysqlEventBase*> m_sqlEvent;
	//m_sqlEvent������ʱ�洢����ֻ�����̲߳���
	std::list<CMysqlEventBase*> m_listSql;
	//sql�߳�ִ��sql����push�Ļص��¼� sql�߳�push���߳�pop
	CNewQueue<CFuncEventBase*> m_funEvent;
	//m_funEvent���˵�ʱ����ʱ�洢����ֻ��sql�̲߳���
	std::list<CFuncEventBase*> m_listFun;
	//���¼���ִֻ��sql��䲻��Ҫ�ص���sql
	CMysqlEventEmpty m_emptyEvent;
	//��sql��䣬���������ڴ���� ���߳�push sql�߳�pop
	CNewQueue<char,5*1024*1024> m_sql;
	//����sql�̻߳���m_sql��pop������sql��� sql�߳�ʹ��
	char* m_pPopSql;
	uint32_t m_MaxPopSize;
	//�������߳ɵ���mysql_real_escape_string������Ŀ�껺�棬�̲߳���ȫ��Ƶ�ֻ�����߳�ʹ��
	char* m_pEscapeSql;
	uint32_t m_MaxEscapeSize;
	//���߳����ڹ���sql���ʹ��
	char buildSql[SQLMAXSIZE];
private:
	MYSQL* m_mysqlConn;//���ݿ�����sql�߳�ʹ��
	MYSQL* m_EscapeStr;//û����ʵ�������ݿ�ֻ���ڵ���mysql_real_escape_string�������Զ��̵߳���
	MYSQL_RES* m_res;//mysql_store_result
	MYSQL_ROW m_row;//mysql_fetch_row
	unsigned long* m_length;//mysql_fetch_lengths
	uint32_t m_numFields;//mysql_num_fields
	uint32_t m_numRow;
	bool m_forceKill;//sql�޷�������sql�̻߳�ǿ��ɱ������
	bool m_cross;
	std::map<ServerID,SqlConnInfo> m_ConnMap;
public:
	void FreeRes();
	bool LoadData(const char* sql,size_t len);
	bool LoadData(const char* sql) { return LoadData(sql, strlen(sql)); }
	bool LoadData(std::string sql) { return LoadData(sql.data(),sql.size()); }
	void ExcSql();
	void ExcSqlCross();
	bool Fetch();
	uint32_t EscapeString(const char* pFrom,uint32_t nSize,char * pTo,uint32_t nMaxSize);
	//���̰߳�ȫ
	void EscapeString(const char*pFrom,uint32_t nSize,std::string& to);
	const char* EscapeString(const char*pFrom,uint32_t nSize);
	std::string EscapeString(const char*pFrom,uint32_t nSize,bool);//���һ��������λ�õ�
	void EscapeString(std::string& from, std::string& to);
	bool EscapeString(std::string& from, char* to, uint32_t maxSize, uint32_t& outSize);
	uint32_t GetFieldSize(uint32_t index) const{return uint32_t(m_length[index]);}
	uint32_t GetRowSize()const{return m_numRow;}
	void Get(uint32_t index,uint8_t&val);
	void Get(uint32_t index,int8_t&val);
	void Get(uint32_t index,uint16_t&val);
	void Get(uint32_t index,int16_t&val);
	void Get(uint32_t index,uint32_t&val);
	void Get(uint32_t index,int32_t&val);
	void Get(uint32_t index,uint64_t&val);
	void Get(uint32_t index,int64_t&val);
	void Get(uint32_t index,std::string&val);
	uint32_t Get(uint32_t index,char*val,uint32_t maxSize);
	template <typename T>
	T Get(uint32_t index);
private:
	//ѭ��1�����������ݿ�
	int32_t Connect(SqlConnInfo& stInfo);
	//�����е�sql����ӡ����־�����ݿ����ӳ��������˲Ż�
	void DumpSql();
	void DumpSqlCross();
};
#define ESCAPESTR CMysqlManager::GetInstance().EscapeString
template <>
uint8_t CMysqlManager::Get<uint8_t>(uint32_t index);
template <>
uint16_t CMysqlManager::Get<uint16_t>(uint32_t index);
template <>
uint32_t CMysqlManager::Get<uint32_t>(uint32_t index);
template <>
uint64_t CMysqlManager::Get<uint64_t>(uint32_t index);
template <>
int8_t CMysqlManager::Get<int8_t>(uint32_t index);
template <>
int16_t CMysqlManager::Get<int16_t>(uint32_t index);
template <>
int32_t CMysqlManager::Get<int32_t>(uint32_t index);
template <>
int64_t CMysqlManager::Get<int64_t>(uint32_t index);
template <>
std::string CMysqlManager::Get<std::string>(uint32_t index);
template <>
const char* CMysqlManager::Get<const char*>(uint32_t index);
#endif