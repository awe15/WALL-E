#include "mon_connector.h"

#define   IP_VALUE(ip)              1          //ip判断标准

int monConnector::nLink = 0;

monConnector::monConnector(socket_info *mon_sInfo)
{
	if (nLink == 0)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	nLink++;
	//ip格式检测
	if (!IP_VALUE(mon_sInfo->ip) || (mon_sInfo->port < 0))
	{
		ERROR_OUTPUT("ip input is wrong!");
		return;
	}
	connect_sta = false;
	sInfo.ip = mon_sInfo->ip;
	sInfo.port = mon_sInfo->port;

}

monConnector::~monConnector()
{
	mon_close();
}


//连接监控终端
int monConnector::mon_connect()
{
	//创建套接字
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(sInfo.ip);
	sockAddr.sin_port = htons(sInfo.port);
	if (-1 == connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)))
		connect_sta = false;
	else 
		connect_sta = true;
	return connect_sta;
}

//关闭连接
void monConnector::mon_close()
{
	if (nLink > 0)
	{
		closesocket(sock);
		nLink--;
		if (nLink == 0)
		{
			WSACleanup(); //终止使用 DLL
		}
	}
}

//重新绑定IP
int monConnector::rebindIP(socket_info *mon_sInfo)
{
	//ip格式检测
	if (!IP_VALUE(mon_sInfo->ip) || (mon_sInfo->port < 0))
	{
		ERROR_OUTPUT("ip input is wrong!");
		return -1;
	}

	if (connect_sta == true)  //断开原来的连接
	{
		closesocket(sock);
	}
	connect_sta = false;
	sock = NULL;
	sInfo.ip = mon_sInfo->ip;
	sInfo.port = mon_sInfo->port;
	mon_connect();
	return connect_sta;
}


int monConnector::mon_recv(char *recv_buf, int size)
{
	if(connect_sta == true)
		return recv(sock, recv_buf, size, NULL);
	else
	{
		ERROR_OUTPUT("not connect!");
		return -1;
	}
}



int monConnector::mon_send(char *send_buf, int size)
{
	if(connect_sta == true)
		return send(sock, send_buf, size, 0);
	 else
	 {
		 ERROR_OUTPUT("not connect!");
		 return -1;
	}
}
