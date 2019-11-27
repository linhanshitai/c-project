#ifndef _COMMON_LOCALSINGLETON_H_
#define _COMMON_LOCALSINGLETON_H_
#include <cassert>
#include "define.h"
//�̱߳��ش洢����
template<typename T>
class CLocalSingleton
{
public:
	//����ʵ��
	static T& CreateInstance()
	{
		return GetInstance();
	}
	//��ȡʵ��
	static T& GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new T();
#ifndef  WIN32
			CLocalSingleton<T>::GetGC();
#endif
		}
		return *m_pInstance;
	}
	//����ʵ��
	static void DestroyInstance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
	//���µ�ʵ�����ɵ�ʵ��������
	static T* SwapInstance(T* new_one)
	{
		T* old_one = m_pInstance;
		m_pInstance = new_one;
		return old_one;
	}
protected:
	THREAD_LOCAL static T	*m_pInstance;
protected:
	CLocalSingleton()
	{
	}
	virtual ~CLocalSingleton()
	{
	}
private:
	// Prohibited actions
	CLocalSingleton(const CLocalSingleton &);
	CLocalSingleton& operator = (const CLocalSingleton &);
private:
	//�Զ���������
#ifndef  WIN32
	class CAutoGC
	{
	public:
		~CAutoGC()
		{
			if(nullptr != m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
		}
	};
	THREAD_LOCAL static CAutoGC m_gc;
	static CAutoGC& GetGC(){return m_gc;}
#endif
};
template<typename T>
THREAD_LOCAL T* CLocalSingleton<T>::m_pInstance = nullptr;
#ifndef  WIN32
template<typename T>
THREAD_LOCAL typename CLocalSingleton<T>::CAutoGC CLocalSingleton<T>::m_gc;
#endif
#endif
