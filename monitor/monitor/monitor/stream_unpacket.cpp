/********************************************************
* �ļ�����
* ˵����
* 
*
* �汾��
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


//��ʼ��
//������ʼ��socket���Ӳ��֣�������socket
bool video::init()
{
	int iRecvLen = 0;
	char ucRecvBuf[500] = {0};

#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter video_init function\n", 1);
#endif

	if (channel->mon_connect())//���ӳɹ�
	{
		char ucSendBuf[] =  "GET /?action=stream HTTP/1.1\n\n";//������Ƶ
		if (-1 != channel->mon_send(ucSendBuf, sizeof(ucSendBuf)+1))
		{
			/* ������ǲ�ʹ�����빦��!��ֻ�跢�����ⳤ��ΪС��2�ֽڵ��ַ��� */
			memset(ucSendBuf, 0x0, sizeof(ucSendBuf));
			strcpy(ucSendBuf, "f\n");   /* �������ǲ�ѡ�����빦�� */
			if (channel->mon_send(ucSendBuf, strlen(ucSendBuf)+1) <= 0)
			{
				return false;
			}

			/* ���ӷ������˽���һ�α��� */
			/* ���տͻ��˷��������ݲ���ʾ���� */
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


//����ļ�����
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
		iRecvLen = channel->mon_recv(ucRecvBuf, 1024); /* ��ȡ1024�ֽ����� */
		if (iRecvLen <= 0)
			return -1;
		else if (iRecvLen == 0)
			iRecvLen = 1024;
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
#ifdef  PRINT_DEBUG_INFO  
			PRINT("the Video Len %ld\n", videolen);
#endif
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
long int video::http_recv(char **lpbuff, long int size)
{    /* *lpbuff�������ʣ�����ݵĴ�С��sizeʣ����Ƶ���ݵĴ�С */
#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter http_recv function\n", 1);
#endif
	int iRecvLen = 0, RecvLen = 0;
	char ucRecvBuf[1024];

	while(size > 0) /* ����һ�δ����ʣ�����ݽ����꣬ */
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
	return RecvLen;    /* ������ν��յ������� */
}


/* �����������ȡ��Ƶ�������ݵĺ��� */
int video::get_videoStream(char *retStream)
{
	long int video_len = 0, iRecvLen;
	int FirstLen = 0;
	char tmpbuf[1024];
	char *FreeBuffer = NULL;

#ifdef  PRINT_DEBUG_INFO  
	PRINT("enter get_videoStream function\n", 1);
#endif


	/* ����1024�ֽ��е���Ƶ���� */
	video_len = getFileLen(tmpbuf, &FirstLen);
	/* ����ʣ����Ƶ���� */
	iRecvLen = http_recv(&FreeBuffer, video_len - FirstLen);

	MUTEX_LOCK(read_mutex);     //ԭ�Ӳ���

	/* �����ν��յ�����Ƶ������װ��һ֡���� */
	memcpy(retStream, tmpbuf, FirstLen);
	memcpy((retStream+FirstLen), FreeBuffer, iRecvLen);
		
	SEND_SIGNAL(read_fin_sig);// ����һ�����ݸ��µ��źţ�֪ͨ����ͨ����ȡ����
	MUTEX_UNLOCK(read_mutex);// ԭ�Ӳ�������

	return video_len;
}