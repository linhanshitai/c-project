#ifndef _SEND_SIGNAL_H_
#define _SEND_SIGNAL_H_
#include "define.h"
#define  USEMAP
#ifndef USEMAP
#include <set>
#else
#include <map>
#endif
//�����ź�
class CRecvSignal;
class CSendSignal
{
public:
	friend  class CRecvSignal;
	CSendSignal( uint32_t nSignalNum=0);
	virtual ~CSendSignal();
	//�ź�ע�ắ��pSignal�����źŶ���nSignalType�ź������Լ��Ƿ����ź���
	bool AddSignal(CRecvSignal *pSignal, uint32_t nSignalType);
	//�ź�������
	uint32_t GetSignalTypeNum()const;
	//ָ�������źŽ���������
	uint32_t GetRecvSignalNum( uint32_t nSignalType)const;
private:
	void RemoveSignal(uint32_t nSignalType,CRecvSignal* pSignal);
	void RemoveSignal();
public://�źŴ���������ȫ������Ϊconst����
	virtual void SendOnlineSignal(uint32_t nValue) const;
	virtual void SendOfflineSignal(uint32_t nValue)const;
	virtual void SendOnlineSignalEX(uint32_t nValue) const;
	virtual void SendOfflineSignalEX(uint32_t nValue)const;
	template<class F,class...Args>//�źŴ�����ʱ����ɾ���ź�
	void OnSignal(uint16_t type,F&& f,Args...args)
	{
		auto sigIter = m_pRecvSignal.find(type);
		if (sigIter == m_pRecvSignal.end())return;
		++m_nOnSignalNum;
		for (auto iter = sigIter->second.begin(); iter != sigIter->second.end();++iter)
		{
			if(iter->second)
				(iter->first->*f)(std::forward<Args>(args)...);
		}
		--m_nOnSignalNum;
		RemoveSignal();
	}
private:
	//���signaltype.h�����е��źŶ���һ���ṹ���ж�������Ƿֿ�������ô������map �����Ƕ�̬����
	//��Ϊ�����ᵼ�¶�̬����ܴ����õ����źź����˷ѿռ�unordered_set����
	typedef std::map<CRecvSignal*,bool>::const_iterator	SignalConstIter;
#ifndef USEMAP
	typedef std::set<CRecvSignal*>			SignalVector;
	SignalVector *m_pRecvSignal;
	const uint32_t m_nSignalNum;
	SignalConstIter m_iter;
	uint16_t m_type;
#else
	typedef std::map<uint32_t,std::map<CRecvSignal*,bool> >			SignalVector;
	SignalVector m_pRecvSignal;
#endif
	uint16_t m_nOnSignalNum = 0;
	std::multimap<uint32_t, CRecvSignal*>m_delSignal;
};
#endif