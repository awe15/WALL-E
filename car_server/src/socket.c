/*************************************************
* 文件名：socket.c     
* 说明：通信方式，通过socket进行通信（LINUX）
*       本机作为服务器
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include "socket.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>

/*************************************************
* 函数名: socket_err_deal
* 说明：错误处理
* 输入参数：error   错误号
* 输出参数：
**************************************************/
static void socket_err_deal(SocketErrNo error)
{
	#ifdef SOCKET_ERROR_PRINT
	switch (error)
	{
		case IP_Err: 
			printf("ip set error!\n"); break;
		case Protocol_Err: 
			printf("protocol set error!\n"); break;
		case Port_Err: 
			printf("port set error!\n"); break;
		case Send_Err: 
			printf("send data error!\n"); break;
		case Recv_Err: 
			printf("receve data error!\n"); break;	
		default: break;	
	}
	
	#endif
}

/*************************************************
* 函数名：socket_send
* 说明：发送socket数据
* 输入参数：sInfo
*          dataBuff         发送缓冲
*          size             发送长度
* 输出参数：错误号
**************************************************/
SocketErrNo socket_send(const SocketInfo * const sInfo, 
                   const unsigned char *dataBuff, int size)
{
	
	if (write(sInfo->clnt_sock, dataBuff, size) == -1)
	{
		socket_err_deal(Send_Err);
		return Send_Err;
	}

	return No_Err;

}

/*************************************************
* 函数名：socket_recv
* 说明：接收socket数据
* 输入参数：sInfo
*          dataBuff     接收数据缓冲
*          size         接受长度
* 输出参数：错误号
**************************************************/
SocketErrNo socket_recv(const SocketInfo * const sInfo, 
                    unsigned char *dataBuff, int size, int *ret_size)
{	
	if ((*ret_size = read(sInfo->clnt_sock, dataBuff, size)) == -1)
	{
		socket_err_deal(Recv_Err);
		return Recv_Err;
	}
	return No_Err;
}


/*************************************************
* 函数名：socket_close
* 说明：关闭socket
* 输入参数：sInfo      
* 输出参数：错误号
**************************************************/
SocketErrNo socket_close(const SocketInfo * const sInfo)
{
    //关闭套接字
    close(sInfo->clnt_sock);
    close(sInfo->serv_sock);
    return No_Err;
}


/*************************************************
* 函数名：socket_listen
* 说明：关闭socket
* 输入参数：sInfo      
* 输出参数：错误号
**************************************************/
void socket_listen(SocketInfo * const sInfo)
{
	socklen_t clnt_addr_size;
	struct sockaddr_in clnt_addr;
#ifdef SOCKET_ERROR_PRINT
	static unsigned int link_t = 0; 
#endif	
	//进入监听状态，等待用户发起请求
	listen(sInfo->serv_sock, 20);

	//接收客户端请求
	clnt_addr_size = sizeof(clnt_addr);
	sInfo->clnt_sock = accept(sInfo->serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	
#ifdef SOCKET_ERROR_PRINT
	link_t++;
	printf("link successful, link time: %d\n", link_t);
#endif
	
}

/*************************************************
* 函数名：socket_init
* 说明：初始化socket端口
*       该函数中有阻塞部分，需要开线程实现
* 输入参数：sInfo        被设socket
*           ip          设置IP
*           port        设置端口
*           protocol    设置协议种类
* 输出参数：错误号
**************************************************/
SocketErrNo socket_init(SocketInfo * const sInfo, const char *ip,
                  int port, ProtocolType protocol)
{
	struct sockaddr_in serv_addr;
	int protoType = Socket_TCP;
	SocketErrNo error_no;
	
	
	//判断参数是否正常
	if (port <= 0)      
	{
		error_no = Port_Err; 
		goto errDeal;
	}
	switch (protocol)
	{
		case Socket_TCP:
			protoType = IPPROTO_TCP; break;
		case Socket_UDP:
			protoType = IPPROTO_UDP; break;
		case Socket_IP:
			protoType = IPPROTO_IP; break;
		default:
			error_no = Protocol_Err;  goto errDeal;
	}
	
	//初始化
	sInfo->proto = protocol;
	sInfo->port = port;
	memset(sInfo->ip, 0, 16);
	memcpy(sInfo->ip, ip, strlen(ip));
	
	
	//创建套接字
   	sInfo->serv_sock = socket(AF_INET, SOCK_STREAM, protoType);
	
	
	//将套接字和IP、端口绑定
	memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
	serv_addr.sin_family = AF_INET;  //使用IPv4地址
	serv_addr.sin_addr.s_addr = inet_addr(sInfo->ip);  //具体的IP地址
	serv_addr.sin_port = htons(sInfo->port);  //端口
	bind(sInfo->serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
	socket_listen(sInfo);
	return No_Err;
	
errDeal:	
	socket_err_deal(error_no);
	return error_no;

}














