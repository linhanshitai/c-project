#ifndef _COMMON_SINGLETON_H_
#define _COMMON_SINGLETON_H_
#include <assert.h>
//����
template<typename T>
class CSingleton
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
			assert(m_pInstance != nullptr);
			CSingleton<T>::GetGC();
		}
		return *m_pInstance;
	}
	static T& Instance(){return GetInstance();}
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
	static T	*m_pInstance;
protected:
	CSingleton()
	{
	}
	virtual ~CSingleton()
	{
	}
private:
	// Prohibited actions
	CSingleton(const CSingleton &);
	CSingleton& operator = (const CSingleton &);
private:
	//�Զ���������
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
	static CAutoGC m_gc;
	static CAutoGC& GetGC(){return m_gc;}
};
template<typename T>
T* CSingleton<T>::m_pInstance = nullptr;

template<typename T>
typename CSingleton<T>::CAutoGC CSingleton<T>::m_gc;
#endif
