#ifndef MONITOR_SOCKET_H
#define MONITOR_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include "port.h"

#include <winsock2.h>   

#pragma comment(lib, "ws2_32.lib")        


struct socket_info
{
public:	
	char* ip;
	int port;
};


class monConnector
{
private:
	 socket_info sInfo;       //ip和端口号
	 bool connect_sta;
	 SOCKET sock;
	 static int nLink;

public:
	monConnector(socket_info *mon_sInfo);
	~monConnector();

	//连接ip(阻塞)
	int mon_connect();

	//断开连接
	void mon_close();

	//查看连接状态
	bool get_connetSta()
	{
		return connect_sta;
	}

	//重新绑定IP
	int rebindIP(socket_info *mon_sInfo);

	//接收函数
	int mon_recv(char *recv_buf, int size);

	//发送函数
	int mon_send(char *send_buf, int size);


};


#endif