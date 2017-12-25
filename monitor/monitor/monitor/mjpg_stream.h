#ifndef MJPG_STREAM_H
#define MJPG_STREAM_H

/* 图片的象素数据 */
typedef struct PixelDatas {
	int iWidth;   /* 宽度: 一行有多少个象素 */
	int iHeight;  /* 高度: 一列有多少个象素 */
	int iBpp;     /* 一个象素用多少位来表示 */
	int iLineBytes;  /* 一行数据有多少字节 */
	int iTotalBytes; /* 所有字节数 */ 
	unsigned char *aucPixelDatas;  /* 象素数据存储的地方 */
}T_PixelDatas, *PT_PixelDatas;

typedef struct VideoBuf {
	T_PixelDatas tPixelDatas;
	int iPixelFormat;
	/* signal fresh frames */
	pthread_mutex_t db;
	pthread_cond_t  db_update;
}T_VideoBuf, *PT_VideoBuf;


typedef struct VideoRecv {
	char *name;  //结构体名字

	int (*Connect_To_Server)(int *SocketClient, const char *ip);//链接到mjpg-streamer服务器上（socket文件句柄的指针，IP地址字符串）
	int (*DisConnect_To_Server)(int *SocketClient);//断开链接（socket文件句柄的指针）
	int (*Init)(int *SocketClient)；//初始化函数
		int (*GetFormat)(void);//获得视频格式
	int (*Get_Video)(int *SocketClient, PT_VideoBuf ptVideoBuf);//获得视频函数（socket文件句柄的指针，PT_VideoBuf 结构体）
	struct VideoRecv *ptNext;  //用于多个结构体的联系
}T_VideoRecv, *PT_VideoRecv;


int VideoRecvInit(void);
void ShowVideoRecv(void);
int RegisterVideoRecv(PT_VideoRecv ptVideoRecv);
int Video_Recv_Init(void);
PT_VideoRecv GetVideoRecv(char *pcName);

#endif 