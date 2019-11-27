#ifndef _H_TRAITMSGHANDLE_H_
#define _H_TRAITMSGHANDLE_H_
#include "msgbase.h"
#include "define.h"
#include "singleton.h"
#include "msgmanager.h"
#include "role.h"
//��Ϣ�ӿ���ȡ
class CPkgBase;
#if 0
//��ͨ����ע�� ����
#define REGFUN(FUN) CMsgManager::Instance().AddMsgBody(\
CMsgCall<decltype(FUN)>::MType::Code(),new CMsgCall<decltype(FUN)>(FUN));
//��ͨ���ע��
#define REGFUNCROSS(FUN) CMsgManager::Instance().AddMsgBody(\
CMsgCall<decltype(FUN)>::MType::Code(),new CMsgCall<decltype(FUN)>(FUN,true));
//������û�д�Э������
#define REGFUNTYPE(FUN,TYPE) CMsgManager::Instance().AddMsgBody(\
TYPE::Code(),new CMsgCall<decltype(FUN)>(FUN));
//û��Э�����Ϳ��ע��
#define REGFUNTYPECROSS(FUN,TYPE) CMsgManager::Instance().AddMsgBody(\
TYPE::Code(),new CMsgCall<decltype(FUN)>(FUN,true));
//ע���ɫ���ĺ��� ����
#define REGPKGFUN(PKG,FUN) CMsgManager::Instance().AddMsgBody(\
CMsgCall<decltype(FUN),decltype(PKG)>::MType::Code(),new CMsgCall<decltype(FUN),decltype(PKG)>(PKG,FUN));
//���ע���ɫ������
#define REGPKGFUNCROSS(PKG,FUN) CMsgManager::Instance().AddMsgBody(\
CMsgCall<decltype(FUN),decltype(PKG)>::MType::Code(),new CMsgCall<decltype(FUN),decltype(PKG)>(PKG,FUN,true));
//������û�д�Э������
#define REGPKGFUNTYPE(PKG,FUN,TYPE)CMsgManager::Instance().AddMsgBody(\
TYPE::Code(),new CMsgCall<decltype(FUN),decltype(PKG)>(PKG,FUN));
//���ע��û��Э������
#define REGPKGFUNTYPECROSS(PKG,FUN,TYPE) CMsgManager::Instance().AddMsgBody(\
TYPE::Code(), new CMsgCall<decltype(FUN),decltype(PKG)>(PKG,FUN,true));
#endif
#if 1
//���������겻�����ڲ���Э�����͵� Ҳ���ǲ�������Ҫ��Э�����
//ע���ɫ���ĺ��� ����
#define REGFUN(ARG1,...) CMsgManager::Instance().AddMsgBody(\
CMsgCall<decltype(TraitsFunc(ARG1,##__VA_ARGS__)),decltype(TraitsPkg(ARG1,##__VA_ARGS__))>::MType::Code(),\
new CMsgCall<decltype(TraitsFunc(ARG1,##__VA_ARGS__)),decltype(TraitsPkg(ARG1,##__VA_ARGS__))>(ARG1,##__VA_ARGS__));
//���ע���ɫ������
#define REGFUNCROSS(ARG1,...) CMsgManager::Instance().AddMsgBody(\
CMsgCall<decltype(TraitsFunc(ARG1,##__VA_ARGS__)),decltype(TraitsPkg(ARG1,##__VA_ARGS__))>::MType::Code(),\
new CMsgCall<decltype(TraitsFunc(ARG1,##__VA_ARGS__)),decltype(TraitsPkg(ARG1,##__VA_ARGS__))>(ARG1,##__VA_ARGS__,true));
#endif
//���ڽ����е�pkg������Ϊ��� �������ʱ��֧�ֺ�����û��Э�����͵ķ�ʽ ���ʱ��Э��ȫ��ע��Ϊʹ�����õķ�ʽ������ֵ���õ��޸�
// ��decltype��ȡ�����ķ���ֵ
template<class F>
F TraitsFunc(F v) { return v; }
template<class T,class F>
F TraitsFunc(T,F v) { return v; }

template<class F>
void TraitsPkg(F v) {  }
template<class T, class F>
T TraitsPkg(T v, F) { return v; }

template<class T,class M>
struct ToType
{
	static T ToVal(T val, CMsgBase* _) { return val; }
};
template<class T>
struct ToType<CRole&, T>
{
	static CRole& ToVal(CRole* val, CMsgBase*_) { return *val;}
};
template<class T>
struct ToType<CRole*, T>
{
	static CRole* ToVal(CRole* val, CMsgBase*_) { return val; }
};
template<class T,class M>
struct ToType<T&, M>
{
	typedef typename std::conditional<std::is_reference<M>::value, CRole*, M>::type Type;
	static T& ToVal(Type _,CMsgBase* p) { return *(T*)p; }
};
template<class T, class M>
struct ToType<T&&, M>
{
	typedef typename std::conditional<std::is_reference<M>::value, CRole*, M>::type Type;
	static T&& ToVal(Type _, CMsgBase* p) { return (T&&)*(T*)p; }
};
template<class T, class M>
struct ToType<T*,M>
{
	typedef typename std::conditional<std::is_reference<M>::value, CRole*, M>::type Type;
	static T* ToVal(Type _,CMsgBase* p) { return (T*)p; }
};
template<class F1, class F2 = void>
class CMsgCall;
template<class H,class M,class T>
class CMsgCall<void (H::*)(M,T),H& (CRole::*)()>: public CMsgCallBase
{
public:
	typedef void (H::*CFType)(M, T);
	typedef  H& (CRole::*HFType)();
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type BaseM;
	typedef typename std::remove_reference<typename std::remove_pointer<T>::type>::type  BaseT;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, BaseT>::type  MType;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseT, BaseM>::type  KType;
	static_assert(std::is_same<KType, CRole>::value, " param must has one type of CRole");
	CMsgCall(HFType&& hfun, CFType&& fun,bool cross=false) 
		:CMsgCallBase(new MType, true,cross), f(fun), h(hfun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		H& ref = (ptr->*h)();
		(ref.*f)(ToType<M,T>::ToVal(ptr,pMsg), ToType<T,M>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
	HFType h;
};
template<class H, class M, class T>
class CMsgCall<void (H::*)(M, T), PkgIndex> : public CMsgCallBase
{
public:
	typedef void (H::* CFType)(M, T);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type BaseM;
	typedef typename std::remove_reference<typename std::remove_pointer<T>::type>::type  BaseT;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, BaseT>::type  MType;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseT, BaseM>::type  KType;
	static_assert(std::is_same<KType, CRole>::value, " param must has one type of CRole");
	CMsgCall(PkgIndex hfun, CFType&& fun, bool cross = false)
		:CMsgCallBase(new MType, true, cross), f(fun), h(hfun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		H* ref = ptr->GetPkg<H>(h);
		(ref->*f)(ToType<M, T>::ToVal(ptr, pMsg), ToType<T, M>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
	PkgIndex h;
};
template<class H, class M>
class CMsgCall<void (H::*)(M), H& (CRole::*)()> : public CMsgCallBase
{
public:
	typedef void (H::*CFType)(M);
	typedef  H& (CRole::*HFType)();
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  BaseM;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, CMsgBase>::type  MType;
	CMsgCall(HFType&& hfun, CFType&& fun, bool cross = false) 
		:CMsgCallBase(new MType, true,cross), f(fun), h(hfun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		H& ref = (ptr->*h)();
		(ref.*f)(ToType<M, CRole*>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
	HFType h;
};
template<class H, class M>
class CMsgCall<void (H::*)(M), PkgIndex> : public CMsgCallBase
{
public:
	typedef void (H::* CFType)(M);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  BaseM;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, CMsgBase>::type  MType;
	CMsgCall(PkgIndex hfun, CFType&& fun, bool cross = false)
		:CMsgCallBase(new MType, true, cross), f(fun), h(hfun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		H* ref = ptr->GetPkg<H>(h);
		(ref->*f)(ToType<M, CRole*>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
	PkgIndex h;
};
template<class H>
class CMsgCall<void (H::*)(), H& (CRole::*)()> : public CMsgCallBase
{
public:
	typedef void (H::*CFType)();
	typedef  H& (CRole::*HFType)();
	CMsgCall(HFType&& hfun, CFType&& fun, bool cross = false) 
		:CMsgCallBase(new CMsgBase, true,cross), f(fun), h(hfun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		H& ref = (ptr->*h)();
		(ref.*f)();
	}
private:
	CFType f;
	HFType h;
};
template<class H>
class CMsgCall<void (H::*)(), PkgIndex> : public CMsgCallBase
{
public:
	typedef void (H::* CFType)();
	CMsgCall(PkgIndex hfun, CFType&& fun, bool cross = false)
		:CMsgCallBase(new CMsgBase, true, cross), f(fun), h(hfun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		H* ref = ptr->GetPkg<H>(h);
		(ref->*f)();
	}
private:
	CFType f;
	PkgIndex h;
};
template<class H, class M, class T,class F2>
class CMsgCall<void (H::*)(M, T), F2> : public CMsgCallBase
{
public:
	typedef void (H::*CFType)(M, T);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  BaseM;
	typedef typename std::remove_reference<typename std::remove_pointer<T>::type>::type  BaseT;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, BaseT>::type  MType;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseT, BaseM>::type  _KType;
	typedef typename std::conditional<
		std::is_same<_KType, CRole>::value, CRole*, uint64_t>::type  KType;
	typedef typename std::conditional<
		std::is_base_of<CSingleton<H>, H>::value, H, CSingleton<H>>::type  HType;
	CMsgCall(CFType&& fun, bool cross = false)
		:CMsgCallBase(new MType, std::is_same<_KType, CRole>::value,cross), f(fun){}
	virtual ~CMsgCall() {}
	template<class P>
	std::enable_if_t<std::is_base_of<CPkgBase, P>::value, void> CallFun(KType ptr)
	{
		H* ref = ptr->template GetPkg<H>();
		(ref->*f)(ToType<M, T>::ToVal(ptr, pMsg), ToType<T, M>::ToVal(ptr, pMsg));
	}
	template<class P>
	std::enable_if_t<!std::is_base_of<CPkgBase, P>::value, void> CallFun(KType ptr)
	{
		(HType::Instance().*f)(ToType<M, T>::ToVal(ptr, pMsg), ToType<T, M>::ToVal(ptr, pMsg));
	}
	virtual void DoMsg(KType ptr) override
	{
		CallFun<H>(ptr);
	}
private:
	CFType f;
};
template<class H, class M, class F2>
class CMsgCall<void (H::*)(M), F2> : public CMsgCallBase
{
public:
	typedef void (H::*CFType)(M);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  BaseM;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, CMsgBase>::type  MType;
	typedef typename std::conditional<
		std::is_same<BaseM, CRole>::value || std::is_base_of<CPkgBase, H>::value, CRole*, uint64_t>::type  KType;
	typedef typename std::conditional<
		std::is_base_of<CSingleton<H>,H>::value, H, CSingleton<H>>::type  HType;
	CMsgCall(CFType&& fun, bool cross = false)
		:CMsgCallBase(new MType, std::is_same<KType, CRole*>::value,cross), f(fun) {}
	virtual ~CMsgCall() {}
	template<class T>
	std::enable_if_t<std::is_base_of<CPkgBase, T>::value, void> CallFun(KType ptr)
	{
		H* ref = ptr->template GetPkg<H>();
		(ref->*f)(ToType<M, CRole*>::ToVal(ptr, pMsg));
	}
	template<class T>
	std::enable_if_t<!std::is_base_of<CPkgBase, T>::value, void> CallFun(KType ptr)
	{
		(HType::Instance().*f)(ToType<M, uint64_t>::ToVal(ptr, pMsg));
	}
	virtual void DoMsg(KType ptr) override
	{
		CallFun<H>(ptr);
	}
private:
	CFType f;
};
template<class H,class F2>
class CMsgCall<void (H::*)(), F2> : public CMsgCallBase
{
public:
	typedef void (H::*CFType)();
	typedef typename std::conditional<std::is_base_of<CSingleton<H>, H>::value, H, CSingleton<H>>::type  HType;
	typedef typename std::conditional<std::is_base_of<CPkgBase, H>::value, CRole, uint64_t>::type  KeyType;
	CMsgCall(CFType&& fun,bool cross = false) :CMsgCallBase(new CMsgBase, std::is_base_of<CPkgBase, H>::value,cross), f(fun) {}
	virtual ~CMsgCall() {}
	template<class T>
	std::enable_if_t<std::is_base_of<CPkgBase, T>::value, void> CallFun(KeyType ptr)
	{
		H* ref = ptr->template GetPkg<H>();
		(ref->*f)();
	}
	template<class T>
	std::enable_if_t<!std::is_base_of<CPkgBase, T>::value, void> CallFun(KeyType ptr)
	{
		(HType::Instance().*f)();
	}
	virtual void DoMsg(KeyType ptr) override
	{
		CallFun<H>(ptr);
	}
private:
	CFType f;
};
template<class M, class F2>
class CMsgCall<void (CRole::*)(M), F2> : public CMsgCallBase
{
public:
	typedef void (CRole::*CFType)(M);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  MType;
	CMsgCall(CFType&& fun, bool cross = false) 
		:CMsgCallBase(new MType, true,cross), f(fun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		(ptr->*f)(ToType<M, CRole*>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
};
template<class F2>
class CMsgCall<void (CRole::*)(), F2> : public CMsgCallBase
{
public:
	typedef void (CRole::*CFType)();
	CMsgCall(CFType&& fun,bool cross = false) 
		:CMsgCallBase(new CMsgBase, true,cross), f(fun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(CRole* ptr) override
	{
		(ptr->*f)();
	}
private:
	CFType f;
};
template<class M, class T, class F2>
class CMsgCall<void (*)(M, T), F2> : public CMsgCallBase
{
public:
	typedef void (*CFType)(M, T);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  BaseM;
	typedef typename std::remove_reference<typename std::remove_pointer<T>::type>::type  BaseT;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, BaseT>::type  MType;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseT, BaseM>::type  _KType;
	typedef typename std::conditional<
		std::is_same<_KType, CRole>::value, CRole*, uint64_t>::type  KType;
	CMsgCall(CFType&& fun, bool cross = false) 
		:CMsgCallBase(new MType, std::is_same<_KType, CRole>::value,cross), f(fun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(KType ptr) override
	{
		f(ToType<M, T>::ToVal(ptr, pMsg), ToType<T, M>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
};
template<class M, class F2>
class CMsgCall<void (*)(M), F2> : public CMsgCallBase
{
public:
	typedef void (*CFType)(M);
	typedef typename std::remove_reference<typename std::remove_pointer<M>::type>::type  BaseM;
	typedef typename std::conditional<
		std::is_base_of<CMsgBase, BaseM>::value, BaseM, CMsgBase>::type  MType;
	typedef typename std::conditional<
		std::is_same<BaseM, CRole>::value, CRole*, uint64_t>::type  KType;
	CMsgCall(CFType&& fun, bool cross = false) 
		:CMsgCallBase(new MType, std::is_same<BaseM, CRole>::value,cross), f(fun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(KType ptr) override
	{
		f(ToType<M, uint64_t>::ToVal(ptr, pMsg));
	}
private:
	CFType f;
};
template<class F2>
class CMsgCall<void(*)(), F2> : public CMsgCallBase
{
public:
	typedef void(*CFType)();
	CMsgCall(CFType&& fun,bool cross = false) 
		:CMsgCallBase(new CMsgBase,false,cross), f(fun) {}
	virtual ~CMsgCall() {}
	virtual void DoMsg(uint64_t ptr) override
	{
		f();
	}
private:
	CFType f;
};
#endif