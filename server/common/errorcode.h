#ifndef _H_ERRORCODE_H
#define _H_ERRORCODE_H
#ifdef WIN32
#include <winerror.h>
#endif
//�ɹ�
#ifndef S_OK
#define S_OK 0 
#endif
//��ָ��
#define E_NULLPOINTER -1
//δ֪����
#define E_UNKNOW -2
//����������
#define E_LESSMEMORY -3
#ifndef WSAEWOULDBLOCK
#define  WSAEWOULDBLOCK E_UNKNOW
#endif
#endif