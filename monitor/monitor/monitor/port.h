#ifndef  MONITOR_PORT_H
#define  MONITOR_PORT_H

#include "iostream"

#define  PRINT_DEBUG_INFO


//�Զ�����������ʾ��ʽ
#define  ERROR_OUTPUT(_err_msg)         std::cout << _err_msg << std::endl


#define  PRINT(_msg, _val)             printf(_msg, _val)


//�ź�������
#define   MUTEX_LOCK(_mut)
#define   MUTEX_UNLOCK(_mut)
#define   MUTEX                  int


#define  SEND_SIGNAL(_sig)
#define  Sig                     int 

#endif

