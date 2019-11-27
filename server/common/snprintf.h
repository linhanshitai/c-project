#ifndef _SNPRINTF_H_
#define _SNPRINTF_H_
#include <stdarg.h>
#include <string>
/*
//�Լ�ʵ�ֵ�snprintf����
����־λ������ȡ���.���ȡ������ȡ�����
֧�����ͣ�
uint8_t hhu 
int8_t hhd 
hhx/hhX 8λ 
uint16_t hd
int16_t hu
uint32_t u
int32_t d
uint64_t llu
int64_t lld
double f
float f
char c
string s
blob b �����һ��size����"%b","test",4
hx/hX 16λ
x/X 32λ
llx/llX 64λ
size_t z=int32_t or int64_t
time_t t=long
ָ��   p
��־λ:(��ʵ��)
-:������ұ����ո��ڴ�ӡ�ַ�Ӧ��û������
+:ǿ�����������ֱ�Ӹ�������������������
�ո�:����ո��ȡ����ͬ+
#:ǰ׺0��0x,0X��������Լ����ƣ�С������ƹ��Ƽ�������
��ȣ��ַ�����û��ʵ������%m.ns�ﲻ��Ԥ��Ч��ֻ����%.ns����n���ܴ����ַ����Ⱥ�%b��ͬ�������������
���ȣ�double��float������snprintfʵ��
���ȣ�h,l,L,n,f,j,t,zû��ʵ�ʵ�����ֱ�������Ϳ���
8����δʵ�ָо��ô�����
*/
class CSnprintf
{
public:
	typedef unsigned int  PrintType;
	enum
	{
		enmPrintFlag_8			= 0x1,
		enmPrintFlag_16			= 0x2,
		enmPrintFlag_32			= 0x4,
		enmPrintFlag_64			= 0x8,
		enmPrintFlag_F			= 0x10,
		enmPrintFlag_Decimal	= 0x20,//10����
		enmPrintFlag_Hex_L		= 0x40,//ʮ������Сд
		enmPrintFlag_Hex_B		= 0x80,//ʮ�����ƴ�д
		enmPrintFlag_Zero		= 0x100,//��0����
		enmPrintFlag_Signed		= 0x200,
	};
public:
	static unsigned int GetNumberLength(unsigned char nValue,PrintType type);
	static unsigned int GetNumberLength(unsigned short nValue,PrintType type);
	static unsigned int GetNumberLength(unsigned int nValue,PrintType type);
	static unsigned int GetNumberLength(unsigned long long nValue,PrintType type);

	static int Snprintf(char *pBuf,unsigned int nMaxSize,const char *pFormat,...);
	static int Snprintf(char *pBuf,unsigned int nMaxSize,const char *pFormat,va_list ap);

	static int FormatValue(char *pBuf,unsigned int nMaxSize,char nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,unsigned char nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,short nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,unsigned short nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,int nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,unsigned int nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,long long  nValue,unsigned int nFlag,unsigned int nWidth);
	static int FormatValue(char *pBuf,unsigned int nMaxSize,unsigned long long nValue,unsigned int nFlag,unsigned int nWidth);

	static int SnprintfEX(char *pBuf,unsigned int nMaxSize,const char *pFormat,...);
	static int SnprintfEX(char *pBuf,unsigned int nMaxSize,const char *pFormat,va_list ap);

	static int FormatValueEX(char *pBuf,unsigned int nMaxSize,long long nValue,unsigned int nFlag,unsigned int nWidth);

	static int FormatValue(char *pBuf,unsigned int nMaxSize, double nValue,unsigned int nFlag,unsigned int nWidth,unsigned int nPrecision);
	static void FcvtEX(long double value,char *pBuf,unsigned int nPrecision);

	static std::string ToStr(long long  nValue);
	static std::string ToStr(unsigned long long nValue);
	static char* ToStr(unsigned long long nValue,char *pBuf);
private:
	static const char hexUp[17];
	static const char HexLow[17];
	static const char digits[201];
};
#endif