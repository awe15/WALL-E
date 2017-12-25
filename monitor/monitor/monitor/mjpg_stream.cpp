#include "mjpg_stream.h"

static PT_VideoRecv g_ptVideoRecvHead = NULL;

static int connect_to_server(int *SocketClient, const char *ip)
{
	int iRet;
	struct sockaddr_in tSocketServerAddr;

	*SocketClient = socket(AF_INET, SOCK_STREAM, 0);

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
		//tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
		if (0 == inet_aton(ip, &tSocketServerAddr.sin_addr))
		{
			DBG_PRINTF("invalid server_ip\n");
			return -1;
		 }
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iRet = connect(*SocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		DBG_PRINTF("connect error!\n");
		return -1;
	}
	return 0;
}

static int disconnect_to_server(int *SocketClient)
{
	close(*SocketClient);
	return 0;
}


static int init(int *SocketClient)  /* 做一些初始化工作 */
{
	char ucSendBuf[100];
	int iSendLen;

	int iRecvLen;
	unsigned char ucRecvBuf[1000];

	/* 发请求类型字符串 */
	memset(ucSendBuf, 0x0, 100);
	strcpy(ucSendBuf, "GET /?action=stream\n");   /* 发送我们是选择 action=stream*/
	iSendLen = send(*SocketClient, ucSendBuf, strlen(ucSendBuf), 0);
	if (iSendLen <= 0)
	{
		close(*SocketClient);
		return -1;
	}

	/* 如果我们不使用密码功能!则只需发送任意长度为小于2字节的字符串 */
	memset(ucSendBuf, 0x0, 100);
	strcpy(ucSendBuf, "f\n");   /* 发送我们不选择密码功能 */
	iSendLen = send(*SocketClient, ucSendBuf, strlen(ucSendBuf), 0);
	if (iSendLen <= 0)
	{
		close(*SocketClient);
		return -1;
	}

	/* 将从服务器端接收一次报文 */
	/* 接收客户端发来的数据并显示出来 */
	iRecvLen = recv(*SocketClient, ucRecvBuf, 999, 0);
	if (iRecvLen <= 0)
	{
		close(*SocketClient);
		return -1;
	}
	else
	{
		ucRecvBuf[iRecvLen] = '\0';
		printf("http header: %s\n", ucRecvBuf);
	}

	return 0;
}



static int getformat(void)
{
	/* 直接返回视频的格式 */
	return V4L2_PIX_FMT_MJPEG;
}


static long int getFileLen(int *SocketClient, char *FreeBuf, int *FreeLen)
{
	int iRecvLen;
	long int videolen;
	char ucRecvBuf[1024];
	char *plen, *buffp;

	while(1)
	{
		iRecvLen = recv(*SocketClient, ucRecvBuf, 1024, 0); /* 获取1024字节数据 */
		if (iRecvLen <= 0)
		{
			close(*SocketClient);
			return -1;
		}
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
			printf("the Video Len %ld\n", videolen);
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
static long int http_recv(int *SocketClient, char **lpbuff, long int size)
{    /* *lpbuff用来存放剩余数据的大小，size剩余视频数据的大小 */
	int iRecvLen = 0, RecvLen = 0;
	char ucRecvBuf[BUFFER_SIZE];

	while(size > 0) /* 把这一次传输的剩余数据接收完， */
	{
		iRecvLen = recv(*SocketClient, ucRecvBuf, (size > BUFFER_SIZE)? BUFFER_SIZE: size, 0);
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
static int get_video(int *SocketClient, PT_VideoBuf ptVideoBuf)
{
	long int video_len, iRecvLen;
	int FirstLen = 0;
	char tmpbuf[1024];
	char *FreeBuffer = NULL;

	while(1)   /* 最终数据存放在ptVideoBuf->tPixelDatas.aucPixelDatas这里面了 */
	{ 
		/* 接收1024字节中的视频数据 */
		video_len = getFileLen(SocketClient, tmpbuf, &FirstLen);
		 /* 接收剩余视频数据 */
		iRecvLen = http_recv(SocketClient, &FreeBuffer, video_len - FirstLen);

		pthread_mutex_lock(&ptVideoBuf->db);

		/* 将两次接收到的视频数据组装成一帧数据 */
		memcpy(ptVideoBuf->tPixelDatas.aucPixelDatas, tmpbuf, FirstLen);
		memcpy(ptVideoBuf->tPixelDatas.aucPixelDatas+FirstLen, FreeBuffer, iRecvLen);
		ptVideoBuf->tPixelDatas.iTotalBytes = video_len;

		pthread_cond_broadcast(&ptVideoBuf->db_update);// 发出一个数据更新的信号，通知发送通道来取数据
		pthread_mutex_unlock( &ptVideoBuf->db );// 原子操作结束
	}
	return 0;
}


/* 构造 */
static T_VideoRecv g_tVideoRecv = {
    .name        = "http",      /* 结构体成员名 */
    .Connect_To_Server  = connect_to_server,     /* 连接到服务器端 */
    .DisConnect_To_Server     = disconnect_to_server,    /* 删除连接 */
    .Init = init,                      /* 做初始化工作 */
    .GetFormat= getformat,                 /* 得到摄像头数据格式 */
    .Get_Video= get_video,                 /* 获取视频数据 */
};

