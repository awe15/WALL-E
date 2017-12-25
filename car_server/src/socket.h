/*************************************************
* 文件名：socket.h     
* 说明：通信方式，通过socket ip4进行通信（LINUX）
*       本机作为服务器
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef  SOCKET_H_
#define  SOCKET_H_
#include "msg_output.h"

#define SOCKET_ERROR_PRINT

/*! 本机服务器IP及通信端口号  */
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



/*! socket控制结构体 */
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

