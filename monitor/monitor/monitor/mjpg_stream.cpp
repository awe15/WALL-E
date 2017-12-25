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


static int init(int *SocketClient)  /* ��һЩ��ʼ������ */
{
	char ucSendBuf[100];
	int iSendLen;

	int iRecvLen;
	unsigned char ucRecvBuf[1000];

	/* �����������ַ��� */
	memset(ucSendBuf, 0x0, 100);
	strcpy(ucSendBuf, "GET /?action=stream\n");   /* ����������ѡ�� action=stream*/
	iSendLen = send(*SocketClient, ucSendBuf, strlen(ucSendBuf), 0);
	if (iSendLen <= 0)
	{
		close(*SocketClient);
		return -1;
	}

	/* ������ǲ�ʹ�����빦��!��ֻ�跢�����ⳤ��ΪС��2�ֽڵ��ַ��� */
	memset(ucSendBuf, 0x0, 100);
	strcpy(ucSendBuf, "f\n");   /* �������ǲ�ѡ�����빦�� */
	iSendLen = send(*SocketClient, ucSendBuf, strlen(ucSendBuf), 0);
	if (iSendLen <= 0)
	{
		close(*SocketClient);
		return -1;
	}

	/* ���ӷ������˽���һ�α��� */
	/* ���տͻ��˷��������ݲ���ʾ���� */
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
	/* ֱ�ӷ�����Ƶ�ĸ�ʽ */
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
		iRecvLen = recv(*SocketClient, ucRecvBuf, 1024, 0); /* ��ȡ1024�ֽ����� */
		if (iRecvLen <= 0)
		{
			close(*SocketClient);
			return -1;
		}
			/* sprintf(buffer, "Content-Type: image/jpeg\r\n" \
			 *               "Content-Length: %d\r\n" \
			 *               "\r\n", frame_size);
		*����˻ᷢ��һЩ���ݱ�ͷ�����ǻ����ĸ�ʽ
		*
		*/
		/* ����ucRecvBuf���жϽ��յ��������Ƿ��Ǳ��� */
		plen = strstr(ucRecvBuf, "Length:");  /* plenָ��ָ��Length�ַ�����ͷ�ĵ�ַ */
		if(NULL != plen)
		{
			plen = strchr(plen, ':');  /* ��plen�и��ҵ������ĵ�ַ */
			plen++;                   /* ָ����һ����ַ������ĵ�ַ�洢������Ƶ���ݵ�������С */
			videolen = atol(plen);    /* ��ȡ��Ƶ���ݵĴ�С */
			printf("the Video Len %ld\n", videolen);
		}

		buffp = strstr(ucRecvBuf, "\r\n\r\n");   /* \r\n\r\n����������whileѭ�� */
		if(buffp != NULL)
		break;
	}   

	buffp += 4;   /* ָ���������ݵĿ�ͷ�������ַ��1024�ֽ��� */
	/* ��Ҫע����������Ѿ�������1024�ֽڵ����ݣ�������1024�ֽڳ��˱�ͷ�󻹰�����һ��������
	*����������Ҫ����һ�£������߼���ϵһ��Ҫ�����ס�
	*/
	*FreeLen = 1024 - (buffp - ucRecvBuf);  /* 1024�ֽ���ʣ�����Ƶ���� */
	memcpy(FreeBuf, buffp, *FreeLen);     /* ���ⲿ�ֵ���Ƶ���ݷ���FreeBuf�������� */
	return videolen;     /* ����1024�ֽ�����������Ƶ���ݵĴ�С */
}


/* �������������ȡ��1024�ֽڣ��������ݱ�ͷ����Ƶ���ݣ������������Ƶ���� */
static long int http_recv(int *SocketClient, char **lpbuff, long int size)
{    /* *lpbuff�������ʣ�����ݵĴ�С��sizeʣ����Ƶ���ݵĴ�С */
	int iRecvLen = 0, RecvLen = 0;
	char ucRecvBuf[BUFFER_SIZE];

	while(size > 0) /* ����һ�δ����ʣ�����ݽ����꣬ */
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
	return RecvLen;    /* ������ν��յ������� */
}


/* �����������ȡ��Ƶ�������ݵĺ��� */
static int get_video(int *SocketClient, PT_VideoBuf ptVideoBuf)
{
	long int video_len, iRecvLen;
	int FirstLen = 0;
	char tmpbuf[1024];
	char *FreeBuffer = NULL;

	while(1)   /* �������ݴ����ptVideoBuf->tPixelDatas.aucPixelDatas�������� */
	{ 
		/* ����1024�ֽ��е���Ƶ���� */
		video_len = getFileLen(SocketClient, tmpbuf, &FirstLen);
		 /* ����ʣ����Ƶ���� */
		iRecvLen = http_recv(SocketClient, &FreeBuffer, video_len - FirstLen);

		pthread_mutex_lock(&ptVideoBuf->db);

		/* �����ν��յ�����Ƶ������װ��һ֡���� */
		memcpy(ptVideoBuf->tPixelDatas.aucPixelDatas, tmpbuf, FirstLen);
		memcpy(ptVideoBuf->tPixelDatas.aucPixelDatas+FirstLen, FreeBuffer, iRecvLen);
		ptVideoBuf->tPixelDatas.iTotalBytes = video_len;

		pthread_cond_broadcast(&ptVideoBuf->db_update);// ����һ�����ݸ��µ��źţ�֪ͨ����ͨ����ȡ����
		pthread_mutex_unlock( &ptVideoBuf->db );// ԭ�Ӳ�������
	}
	return 0;
}


/* ���� */
static T_VideoRecv g_tVideoRecv = {
    .name        = "http",      /* �ṹ���Ա�� */
    .Connect_To_Server  = connect_to_server,     /* ���ӵ��������� */
    .DisConnect_To_Server     = disconnect_to_server,    /* ɾ������ */
    .Init = init,                      /* ����ʼ������ */
    .GetFormat= getformat,                 /* �õ�����ͷ���ݸ�ʽ */
    .Get_Video= get_video,                 /* ��ȡ��Ƶ���� */
};

