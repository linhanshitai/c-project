#ifndef _H_COMMONAPI_H
#define _H_COMMONAPI_H
#include <string>
#include <cstdint>
#include "define.h"
//���ú���
//����Ŀ¼
int32_t CreateDir(const char *pszDir);
//˯��ָ������
void MySleep(uint32_t nMilliseconds);//����
//˯��ָ��΢�� ������������΢�뼶�����߿���cpuҲ�ܸ߲�����ʹ�� ����1�����Ѿ��㹻��
void MicroSleep(uint32_t nMicrosecond);//΢���С��1�� ΢�����߿���cpuռ�úܸ�ÿ������1����Ƚ��ʺ�
//��ʼ��window�����
void InitWinSocket();
//��ȡʱ��
int32_t GetTimeZone();
//��ӡ����ʱ��
void PrintVersion();
//��ʼ��Ϊ�ػ�����Linux������
void InitDaemon();
int32_t CreateServerDir(const char *pszSeverName);
//������������������Ϊ���̵߳����ŵ������ڴ�ͷ��
//uint32_t Lock(ShareMemoryStatus& flag,ShareMemoryStatus type);
//int32_t Unlock(ShareMemoryStatus& flag,ShareMemoryStatus type);

int64_t	atoi64(const char* pszNum);
int32_t Random(int32_t nRange);
int32_t Random2(int32_t nRange);
uint32_t randomNumberSeed() ;
FILE *Fopen( const char * szFilename,const char * szMode);
std::string ToStr(uint8_t nValue);
std::string ToStr(uint16_t nValue);
std::string ToStr(uint32_t nValue);
std::string ToStr(uint64_t nValue);
std::string ToStr(int8_t nValue);
std::string ToStr(int16_t nValue);
std::string ToStr(int32_t nValue);
std::string ToStr(int64_t nValue);
std::string ToStr(const std::string& nValue);
std::string ToStr(std::string&& nValue);
int32_t ToStr(uint8_t nValue,char* pBuf);
int32_t ToStr(uint16_t nValue ,char* pBuf);
int32_t ToStr(uint32_t nValue,char* pBuf);
int32_t ToStr(uint64_t nValue ,char* pBuf);
int32_t ToStr(int8_t nValue ,char* pBuf);
int32_t ToStr(int16_t nValue ,char* pBuf);
int32_t ToStr(int32_t nValue ,char* pBuf);
int32_t ToStr(int64_t nValue ,char* pBuf);
template <typename T>
T ToInt(const std::string &str);
template <>
uint8_t ToInt<uint8_t>(const std::string &str);
template <>
uint16_t ToInt<uint16_t>(const std::string &str);
template <>
uint32_t ToInt<uint32_t>(const std::string &str);
template <>
uint64_t ToInt<uint64_t>(const std::string &str);
template <>
int8_t ToInt<int8_t>(const std::string &str);
template <>
int16_t ToInt<int16_t>(const std::string &str);
template <>
int32_t ToInt<int32_t>(const std::string &str);
template <>
int64_t ToInt<int64_t>(const std::string &str);
template <typename T>
T ToInt(const char* pSt);
template <>
uint8_t ToInt<uint8_t>(const char* pSt);
template <>
uint16_t ToInt<uint16_t>(const char* pSt);
template <>
uint32_t ToInt<uint32_t>(const char* pSt);
template <>
uint64_t ToInt<uint64_t>(const char* pSt);
template <>
int8_t ToInt<int8_t>(const char* pSt);
template <>
int16_t ToInt<int16_t>(const char* pSt);
template <>
int32_t ToInt<int32_t>(const char* pSt);
template <>
int64_t ToInt<int64_t>(const char* pSt);
int32_t Avg(int32_t nValue1,int32_t nValue2);
uint32_t Avg(uint32_t nValue1,uint32_t nValue2);
uint32_t Ceil(uint32_t nValue1,uint32_t nValue2);
template <typename T>
T ToInt(const std::string &&str);
template <>
uint8_t ToInt<uint8_t>(const std::string &&str);
template <>
uint16_t ToInt<uint16_t>(const std::string &&str);
template <>
uint32_t ToInt<uint32_t>(const std::string &&str);
template <>
uint64_t ToInt<uint64_t>(const std::string &&str);
template <>
int8_t ToInt<int8_t>(const std::string &&str);
template <>
int16_t ToInt<int16_t>(const std::string &&str);
template <>
int32_t ToInt<int32_t>(const std::string &&str);
template <>
int64_t ToInt<int64_t>(const std::string &&str);
int32_t ToStr(const uint8_t nValue,char* pBuf,const uint32_t nCount);
int32_t ToStr(const uint16_t nValue ,char* pBuf,const uint32_t nCount);
int32_t ToStr(const uint32_t nValue,char* pBuf,const uint32_t nCount);
int32_t ToStr(const uint64_t nValue ,char* pBuf,const uint32_t nCount);
int32_t ToStr(const int8_t nValue ,char* pBuf,const uint32_t nCount);
int32_t ToStr(const int16_t nValue ,char* pBuf,const uint32_t nCount);
int32_t ToStr(const int32_t nValue ,char* pBuf,const uint32_t nCount);
int32_t ToStr(const int64_t nValue ,char* pBuf,const uint32_t nCount);
void ToStr(uint8_t nValue,std::string &str);
void ToStr(uint16_t nValue,std::string &str);
void ToStr(uint32_t nValue,std::string &str);
void ToStr(uint64_t nValue,std::string &str);
void ToStr(int8_t nValue,std::string &str);
void ToStr(int16_t nValue,std::string &str);
void ToStr(int32_t nValue,std::string &str);
void ToStr(int64_t nValue,std::string &str);
void Trim(std::string& str);
#ifdef WIN32
int epoll_create(int size);
int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);
int epoll_ctl(int epfd, int op, int64_t fd, struct epoll_event *event);
#endif
uint32_t GetThreadId();//��ȡ�߳�id
uint32_t GetProcessId();//��ȡ����id
void KillSelf();
void IgnoreSignal();
void RegistSignal(void (*handle)(int));
bool CpySql(char* out, uint32_t max, const char* in, uint32_t inSize, uint32_t& outSize);
std::string GetDir(const char* pSrc);
std::string GetServerName(const char* pSrc);
bool CommonInit(int argc, char* argv[]);
void Base64Encode(const char* d, uint32_t len, std::string& ret);
std::string UrlEncode(const std::string& str);
#ifdef WIN32
#define SSL_VERIFY_NONE 0
#define SSL_FILETYPE_PEM 0
#define SSL_ERROR_WANT_READ 0
#define SSL_ERROR_WANT_WRITE 1
struct SSL_METHOD {};
void SSL_library_init();
void SSL_load_error_strings();
const SSL_METHOD* SSLv23_client_method();
const SSL_METHOD* SSLv23_server_method();
const SSL_METHOD* SSLv23_method();
SSL_CTX* SSL_CTX_new(const SSL_METHOD*);
void SSL_CTX_set_verify(SSL_CTX*, int, int*);
int SSL_CTX_use_certificate_file(SSL_CTX*, const char*, int);
int SSL_CTX_use_certificate_chain_file(SSL_CTX* ctx, const char* file);
int SSL_CTX_use_PrivateKey_file(SSL_CTX*, const char*, int);
int SSL_CTX_check_private_key(SSL_CTX*);
int SSL_do_handshake(SSL*);
int SSL_read(SSL* ssl, void* buf, int num);
int SSL_write(SSL* ssl, const void* buf, int num);
int SSL_get_error(const SSL* ssl, int ret);
int SSL_set_fd(SSL*, int);
void SSL_set_connect_state(SSL* ssl);
void SSL_set_accept_state(SSL* ssl);
SSL* SSL_new(SSL_CTX*);
void SSL_CTX_free(SSL_CTX*);
void ERR_free_strings();
void SSL_free(SSL*);
void SSL_shutdown(SSL*);
#endif 
#endif
