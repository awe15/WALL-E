/********************************************************
* 文件名：
* 说明：
* 
* 依赖头文件：
*
*
* 调用说明：1.创建VIDEO对象，并需要对应tcp/ip的通道monConnector
*           2.调用init初始化TCP/IP通道，并发送HTTP请求
*          
* 版本：
*********************************************************/
#ifndef  STREAM_UNPACKET_H
#define  STREAM_UNPACKET_H

#include<stdlib.h>
#include "mon_connector.h"
#include "port.h"
	

class video
{
private:

	int iWidth;   /* 宽度: 一行有多少个象素 */
	int iHeight;  /* 高度: 一列有多少个象素 */
	int iBpp;     /* 一个象素用多少位来表示 */
	int iLineBytes;  /* 一行数据有多少字节 */
	int iTotalBytes; /* 所有字节数 */ 
	unsigned char *aucPixelDatas;  /* 象素数据存储的地方 */

	monConnector *channel;     //对应一个tcp/ip网络通道
	MUTEX read_mutex;         
	Sig   read_fin_sig;

private:
	long int getFileLen(char *FreeBuf, int *FreeLen);
	long int http_recv(char **lpbuff, long int size);

public:
	video(monConnector *connector);
	~video();

	//初始化
	bool init();

	//获得数据流
	int video::get_videoStream(char *retStream);



};






#endif


