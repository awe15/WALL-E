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
	 socket_info sInfo;       //ip�Ͷ˿ں�
	 bool connect_sta;
	 SOCKET sock;
	 static int nLink;

public:
	monConnector(socket_info *mon_sInfo);
	~monConnector();

	//����ip(����)
	int mon_connect();

	//�Ͽ�����
	void mon_close();

	//�鿴����״̬
	bool get_connetSta()
	{
		return connect_sta;
	}

	//���°�IP
	int rebindIP(socket_info *mon_sInfo);

	//���պ���
	int mon_recv(char *recv_buf, int size);

	//���ͺ���
	int mon_send(char *send_buf, int size);


};


#endif