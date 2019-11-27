#ifndef _H_TIMER_BASE_H_
#define _H_TIMER_BASE_H_
#include <set>
#include "define.h"
//��ʱ������
class CTimerManager;
class CDateTime;
class CTimerBase
{
public:
	friend CTimerManager;
	CTimerBase();
	virtual ~CTimerBase();
	/*TimerID = 0��Ч type �Զ������� nData ����д�Զ��������*/
	/*nTime ��һ�ζ�ʱ������ʱ���� ΢��/���� nCycleTime = ѭ����ʱ��ʱ�� 0��ʾֻ����һ�� ΢��/����*/
	TimerID AddIntervalTimer(TimerType type,uint64_t nTime,uint64_t nCycleTime=0,TimerData data =uint64_t(0));
	/*TimerID = 0��Ч type �Զ������� nData ����д�Զ��������*/
	/*nTime ��һ�ζ�ʱ������ʱ��� ΢��/���� nCycleTime = ѭ����ʱ��ʱ�� 0��ʾֻ����һ�� ΢��/����*/
	TimerID AddTimestampTimer(TimerType type,uint64_t nTime,uint64_t nCycleTime=0,TimerData data =uint64_t(0));
	/*TimerID = 0��Ч type �Զ������� nData ����д�Զ��������*/
	/*nTime ��һ�ζ�ʱ������ʱ��� ��λ�� nCycleTime = ѭ����ʱ��ʱ�� 0��ʾֻ����һ�� ��λ��*/
	TimerID AddTimerSec(TimerType type,uint32_t nTime,uint32_t nCycleTime=0,TimerData data =uint64_t(0));
	uint32_t DeleteAllTimer();
	bool DeleteTimer(TimerID timerID);
	bool HasTimer(TimerID timerID)const;
	uint32_t GetTimerIdSize()const { return (uint32_t)m_TimerID.size(); }
public:
	bool HasTimer(TimerType type)const;//����ӿ����ܲ��������Ҫ���Խ�m_TimerID�޸�Ϊmap
	//��Ҫ�̳еĶ�ʱ��������
	virtual void OnTimerEvent(const TimerDataInfo& data,const CDateTime& dt)=0;
private:
	bool CancelTimer(TimerID timerID);
private:
	//��set�������ж�ʱ��id
	std::set<TimerID> m_TimerID;
};
template<typename T>
class COnlyTimerEvent:public CTimerBase
{
public:
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt) override
	{
		T::Instance().OnTimerEvent(data, dt);
	}
};
class CTimterEventImpl:public CTimerBase
{
public:
	CTimterEventImpl(CTimerBase* impl) :m_impl(impl) {}
	virtual void OnTimerEvent(const TimerDataInfo& data, const CDateTime& dt) override
	{
		m_impl->OnTimerEvent(data, dt);
	}
private:
	CTimerBase* m_impl;
};
#endif