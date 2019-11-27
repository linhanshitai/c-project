#include "pch.h"
#include "configmanager.h"
#include "log.h"
 CCfgManager::~CCfgManager()
 {
	 for (auto& iter : m_holder)
		 delete iter.second;
 }
 bool CCfgManager::AddLoader(CBaseHolder* pLoader, LoadCfgOrder id , bool reload)
 { 
	 unsigned int tmp = id;
	 return m_holder.emplace((tmp << 1) | (reload ? 1 : 0), pLoader),true;
 }
bool CCfgManager::Load()
{
	for (auto& iter : m_holder)//�����ü���֮ǰ���ص�����
		if (iter.first < LoadCfgOrder_PreData && !iter.second->LoadData())
			return WRITE_ERROR_LOG("pre load data has error info=%s", iter.second->GetInfo()), false;
	for (auto& iter : m_holder)//��������
		if (!iter.second->LoadCfg())
			return WRITE_ERROR_LOG("load cfg has error info=%s", iter.second->GetInfo()), false;
	for (auto& iter : m_holder)//��������
		if (iter.first > LoadCfgOrder_PreData && !iter.second->LoadData())
			return WRITE_ERROR_LOG("load data has error info=%s", iter.second->GetInfo()), false;
	for (auto& iter : m_holder)//��ʼ������
		if (!iter.second->Init())
			return WRITE_ERROR_LOG("init data has error info=%s", iter.second->GetInfo()), false;
	return true;
}
bool CCfgManager::ReLoad()
{
	for (auto& iter : m_holder)//���¼�������
		if ((iter.first & LoadCfgOrder_Reload) > 0 && !iter.second->ReloadCfg())
			return WRITE_ERROR_LOG("reload cfg has error info=%s", iter.second->GetInfo()), false;
	for (auto& iter : m_holder)//���³�ʼ������
		if ((iter.first & LoadCfgOrder_Reload) > 0 && !iter.second->Reinit())
			return WRITE_ERROR_LOG("reinit data has error info=%s", iter.second->GetInfo()), false;
	return true;
}