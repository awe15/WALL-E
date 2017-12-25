/*************************************************
* �ļ�����socket.h     
* ˵����ͨ�ŷ�ʽ��ͨ��socket ip4����ͨ�ţ�LINUX��
*       ������Ϊ������
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#ifndef  SOCKET_H_
#define  SOCKET_H_
#include "msg_output.h"

#define SOCKET_ERROR_PRINT

/*! ����������IP��ͨ�Ŷ˿ں�  */
#define     SERVER_IP                "192.168.55.1"
#define     SERVER_DATA_PORT         1234
#define     SERVER_CAMERA_PORT1      8070
#define     SERVER_CAMERA_PORT2      8090      

typedef enum SocketErrNo{
	No_Err = 0,
	IP_Err,
	Protocol_Err,
	Port_Err,
	Send_Err,
	Recv_Err,
}SocketErrNo;



typedef enum ProtocolType{
	Socket_TCP,
	Socket_UDP,
	Socket_IP,
}ProtocolType;



/*! socket���ƽṹ�� */
typedef struct SocketInfo
{
	unsigned char ip[16];
	int port;
	ProtocolType  proto;
	int clnt_sock;
	int serv_sock;
}SocketInfo;



SocketErrNo socket_init(SocketInfo * const sInfo, const char *ip,
                  int port, ProtocolType protocol);
				  
SocketErrNo socket_send(const SocketInfo * const sInfo, const unsigned char *dataBuff, int size);

SocketErrNo socket_recv(const SocketInfo * const sInfo, 
                    unsigned char *dataBuff, int size, int *ret_size);

SocketErrNo socket_close(const SocketInfo * const sInfo);


#endif

