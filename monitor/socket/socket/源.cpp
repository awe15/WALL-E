#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
int main(){
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("192.168.80.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = {0};
	recv(sock, szBuffer, MAXBYTE, NULL);
	//输出接收到的数据
	printf("Message form server: %s\n", szBuffer);
	char sendbuff3[] = {0x7f, 0x01, 0x0, 0xb, 0x00, 0x02, 0x12,0x0c,0x8a,0x16,0x2e, 0x00, 0x00, 0x65};
	//  {0x7f, 0x01, 0x00, 0x08, 0x01, 0x10,0x1f,0,0,0,0x65};
	send(sock, sendbuff3, sizeof(sendbuff3), 0);
	char sendbuff[] = {0x7f, 0x01, 0x0, 0x9, 0x00, 0x02, 0x10,0x1f,0x00,0x00,0x00,0x65};
	                //  {0x7f, 0x01, 0x00, 0x08, 0x01, 0x10,0x1f,0,0,0,0x65};
	send(sock, sendbuff, sizeof(sendbuff), 0);
	char sendbuff1[] = {0x7f, 0x01, 0x0, 0x9, 0x00, 0x02, 0x10,0x0f,0x00,0x00,0x00,0x65};
	send(sock, sendbuff1, sizeof(sendbuff1), 0);
	//关闭套接字
	closesocket(sock);
	//终止使用 DLL
	WSACleanup();
	system("pause");
	return 0;
}