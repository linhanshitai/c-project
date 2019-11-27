#ifndef _MYSQL_CALLBACK_H_
#define _MYSQL_CALLBACK_H_
#include "define.h"
#include "mysqlrecord.h"
//���ݿ�ص�����
class CMysqlCallBack
{
public:
	CMysqlCallBack(){}
	virtual ~CMysqlCallBack(){}
public:
	//insert update delete �õĻص�
	virtual void CallBack(bool succ,uint32_t nAffectRow){}
	virtual void CallBack(bool succ, uint32_t nAffectRow,FuncData data){}
	//��������id��insert�Ļص�
	virtual void CallBack(bool succ, uint32_t nAffectRow,uint64_t nInsertID){}
	virtual void CallBack(bool succ, uint32_t nAffectRow,uint64_t nInsertID,FuncData data){}
	//select �õĻص� 
	virtual void CallBack(bool succ, std::shared_ptr<CMysqlRecord> pRecord){}
	virtual void CallBack(bool succ, std::shared_ptr<CMysqlRecord> pRecord,FuncData data){}
private:

};
#endif