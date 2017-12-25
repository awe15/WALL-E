/********************************************************
* 文件名：
* 说明：
* 
*
* 版本：
*********************************************************/

#include "stream_unpacket.h"

video::video(monConnector *connector)
{
//	iWidth = iW;
	//iHeight = iH;
	//iBpp = iB;
	//iTotalBytes = totalBytes;
	channel = connector;
}

video::~video()
{

}


//初始化
//包括初始化socket连接部分，独立于socket
bool video::init()
{
	int iRecvLen = 0;
	char ucRecvBuf[500] = {0};

#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter video_init function\n", 1);
#endif

	if (channel->mon_connect())//连接成功
	{
		char ucSendBuf[] =  "GET /?action=stream HTTP/1.1\n\n";//连接视频
		if (-1 != channel->mon_send(ucSendBuf, sizeof(ucSendBuf)+1))
		{
			/* 如果我们不使用密码功能!则只需发送任意长度为小于2字节的字符串 */
			memset(ucSendBuf, 0x0, sizeof(ucSendBuf));
			strcpy(ucSendBuf, "f\n");   /* 发送我们不选择密码功能 */
			if (channel->mon_send(ucSendBuf, strlen(ucSendBuf)+1) <= 0)
			{
				return false;
			}

			/* 将从服务器端接收一次报文 */
			/* 接收客户端发来的数据并显示出来 */
			iRecvLen = channel->mon_recv(ucRecvBuf, 499);
			if (iRecvLen >= 0)
			{
				ucRecvBuf[iRecvLen] = '\0';
#ifdef  PRINT_DEBUG_INFO  
				PRINT("http header: \n%s\n", ucRecvBuf);
#endif
				return true;
			}
		}
	}
	return false;
}


//获得文件长度
long int video::getFileLen(char *FreeBuf, int *FreeLen)
{
	int iRecvLen;
	long int videolen;
	char ucRecvBuf[1024];
	char *plen, *buffp;

#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter getFileLen function\n", 1);
#endif

	while(1)
	{
		iRecvLen = channel->mon_recv(ucRecvBuf, 1024); /* 获取1024字节数据 */
		if (iRecvLen <= 0)
			return -1;
		else if (iRecvLen == 0)
			iRecvLen = 1024;
		/* sprintf(buffer, "Content-Type: image/jpeg\r\n" \
		 *               "Content-Length: %d\r\n" \
		 *               "\r\n", frame_size);
		*服务端会发送一些数据报头，这是基本的格式
		*
		*/
		/* 解析ucRecvBuf，判断接收到的数据是否是报文 */
		plen = strstr(ucRecvBuf, "Length:");  /* plen指针指向Length字符串开头的地址 */
		if(NULL != plen)
		{
			plen = strchr(plen, ':');  /* 在plen中个找到：出的地址 */
			plen++;                   /* 指向下一个地址，后面的地址存储的是视频数据的真正大小 */
			videolen = atol(plen);    /* 读取视频数据的大小 */
#ifdef  PRINT_DEBUG_INFO  
			PRINT("the Video Len %ld\n", videolen);
#endif
		}

		buffp = strstr(ucRecvBuf, "\r\n\r\n");   /* \r\n\r\n遇到就跳出while循环 */
		if(buffp != NULL)
		break;
	}   

	buffp += 4;   /* 指向真正数据的开头，这个地址在1024字节中 */
	/* 需要注意的是我们已经接受了1024字节的数据，但是这1024字节除了表头后还包含了一部分数据
	*这里我们需要计算一下，这里逻辑关系一定要想明白。
	*/
	*FreeLen = 1024 - (buffp - ucRecvBuf);  /* 1024字节中剩余的视频数据 */
	memcpy(FreeBuf, buffp, *FreeLen);     /* 把这部分的视频数据放在FreeBuf缓冲器中 */
	return videolen;     /* 返回1024字节中真正的视频数据的大小 */
}


/* 这个函数用来获取除1024字节（包含数据表头和视频数据）数据以外的视频数据 */
long int video::http_recv(char **lpbuff, long int size)
{    /* *lpbuff用来存放剩余数据的大小，size剩余视频数据的大小 */
#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter http_recv function\n", 1);
#endif
	int iRecvLen = 0, RecvLen = 0;
	char ucRecvBuf[1024];

	while(size > 0) /* 把这一次传输的剩余数据接收完， */
	{
		iRecvLen = channel->mon_recv(ucRecvBuf, (size>1024)?1024:size);
		if (iRecvLen <= 0)
			break;

		RecvLen += iRecvLen;
		size -= iRecvLen;

		if(*lpbuff == NULL)
		{
			*lpbuff = (char *)malloc(RecvLen);
			if(*lpbuff == NULL)
				return -1;
		}
		else
		{
			*lpbuff = (char *)realloc(*lpbuff, RecvLen);
			if(*lpbuff == NULL)
				return -1;
		}

		memcpy(*lpbuff+RecvLen-iRecvLen, ucRecvBuf, iRecvLen);
	}
	return RecvLen;    /* 返回这次接收到的数据 */
}


/* 这个是真正获取视频所以数据的函数 */
int video::get_videoStream(char *retStream)
{
	long int video_len = 0, iRecvLen;
	int FirstLen = 0;
	char tmpbuf[1024];
	char *FreeBuffer = NULL;

#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter get_videoStream function\n", 1);
#endif


	/* 接收1024字节中的视频数据 */
	video_len = getFileLen(tmpbuf, &FirstLen);
	/* 接收剩余视频数据 */
	iRecvLen = http_recv(&FreeBuffer, video_len - FirstLen);

	MUTEX_LOCK(read_mutex);     //原子操作

	/* 将两次接收到的视频数据组装成一帧数据 */
	memcpy(retStream, tmpbuf, FirstLen);
	memcpy((retStream+FirstLen), FreeBuffer, iRecvLen);
		
	SEND_SIGNAL(read_fin_sig);// 发出一个数据更新的信号，通知发送通道来取数据
	MUTEX_UNLOCK(read_mutex);// 原子操作结束

	return video_len;
}