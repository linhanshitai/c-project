#ifndef _H_LOGTHREAD_H
#define _H_LOGTHREAD_H
#include "thread.h"
#include "singleton.h"
//��־�߳�
class CLogThread:public CThread,public CSingleton<CLogThread>
{
public:
	CLogThread();
	~CLogThread();
	void virtual Run()override;
};
#endif