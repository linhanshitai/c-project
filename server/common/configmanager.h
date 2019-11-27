#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_
#include <map>
#include "singleton.h"
//�������������ݼ��ع�����
class CBaseHolder
{
public:
	CBaseHolder() {}
	virtual ~CBaseHolder() {}
	virtual bool LoadCfg() = 0;
	virtual bool ReloadCfg() = 0;
	virtual bool LoadData() = 0;
	virtual bool Init() = 0;
	virtual bool Reinit() = 0;
	virtual const char* GetInfo() = 0;
};
template<class T>
class CCommonHolder :public CBaseHolder
{
	//ͨ��ģ���Զ����������ü������ݺͳ�ʼ�� ͨ��LoadCfgOrder�ĵ�һλ��Ϊ�Ƿ���Ҫ���¼��غ����³�ʼ�� �ñ�κ괫���Ƿ���Ҫ���¼��غ����³�ʼ��
private:
	template <typename U>
	class IsCfgLoader
	{
		template <class C> static uint8_t check(typename C::ConfigValueType*) { return 0; }
		template <class C> static uint16_t check(...) { return 0; }
	public:
		enum { value = (sizeof (uint8_t) == sizeof( check<U>(nullptr))) };
	};
	template<typename U>
	std::enable_if_t<std::is_same_v<decltype(&U::LoadCfg), bool (U::*)()> && IsCfgLoader<U>::value, bool> LoadCfg(U*ptr) { return U::Instance().LoadCfg(); }
	template<typename U>
	bool LoadCfg(...) { return true; }
	template<typename U>
	std::enable_if_t<std::is_same_v<decltype(&U::ReloadCfg), bool (U::*)()> && IsCfgLoader<U>::value, bool> ReloadCfg(U* ptr) { return U::Instance().ReloadCfg(); }
	template<typename U>
	bool ReloadCfg(...) { return true; }
	template<typename U>
	std::enable_if_t<std::is_same_v<decltype(&U::LoadData), bool (U::*)()>, bool> LoadData(U* ptr) { return U::Instance().LoadData(); }
	template<typename U>
	bool LoadData(...) { return true; }
	template<typename U>
	std::enable_if_t<std::is_same_v<decltype(&U::Init), bool (U::*)()>, bool> Init(U* ptr) { return U::Instance().Init(); }
	template<typename U>
	bool Init(...) { return true; }
	template<typename U>
	std::enable_if_t<std::is_same_v<decltype(&U::Reinit), bool (U::*)()>, bool> Reinit(U* ptr) { return U::Instance().Reinit(); }
	template<typename U>
	bool Reinit(...) { return true; }
public:
	CCommonHolder() {}
	virtual ~CCommonHolder() {}
	virtual bool LoadCfg()override { return LoadCfg<T>(nullptr); }
	virtual bool ReloadCfg()override { return ReloadCfg<T>(nullptr); }
	virtual bool LoadData()override { return LoadData<T>(nullptr); }
	virtual bool Init()override { return Init<T>(nullptr); }
	virtual bool Reinit()override { return Reinit<T>(nullptr); }
	virtual const char* GetInfo() override{ return typeid(T).name(); }
};
enum LoadCfgOrder :unsigned int
{
	LoadCfgOrder_Reload = 1,//���¼��غ����³�ʼ����� 
	LoadCfgOrder_Pre = 0xFF,//���ȼ��ص����ݿ����ݵ�ö�ٷ��������ǰ��
	LoadCfgOrder_Data = 0x8FFF,//���ü���˳��ö�ٷ����ǰ�� ���ݼ���˳��ö�ٷ����������
	LoadCfgOrder_Defualt = 0xFFFF,//Ĭ�ϵ�˳�� �м���˳����������ú����ݿ�ö�ٷ������ǰ��
	LoadCfgOrder_PreData = (LoadCfgOrder_Pre << 1) | LoadCfgOrder_Reload,
};
//����ͬʱ֧�ּ������� �������� ��ʼ������
//һ�������ͬʱ�������ú��������һ����Ҫ֧�����¼���һ������Ҫ֧�����¼���ֻ���Լ���Init��LoadCfg�п���
#ifdef WIN32
#define REGHOLDER(TYPE,...) static bool __##TYPE__ = CCfgManager::GetInstance().AddLoader(new CCommonHolder<TYPE>,##__VA_ARGS__);
#else
#define REGHOLDER(TYPE,...) static bool __##TYPE__  __attribute__((unused))= CCfgManager::GetInstance().AddLoader(new CCommonHolder<TYPE>,##__VA_ARGS__);
#endif // WIN32
class CCfgManager:public CSingleton<CCfgManager>
{
public:
	CCfgManager(){}
	~CCfgManager();
	bool AddLoader(CBaseHolder* pLoader, LoadCfgOrder id = LoadCfgOrder_Defualt,bool reload=false);
	bool Load();
	bool ReLoad();
private:
	std::multimap<unsigned int, CBaseHolder*> m_holder;//�м���˳������
};
#endif