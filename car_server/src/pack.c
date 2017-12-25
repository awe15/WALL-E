/*************************************************
* 文件名：pack.c     
* 说明：通信协议数据打包和拆包
*       这层负责组装好的数据做最后打包协议数据
*       传向通信层
*       拆包负责解析出校验(如果有)正确的数据 
*       打包,拆包内容包括帧头尾，地址，长度，校验等
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include "pack.h"
#include "packCmd_port.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>



/*************************************************
* 函数名: checkout
* 说明：解析校验
* 输入参数：data            数据缓冲
*           size            数据长度
* 输出参数：原数据+校验码的总字节数
**************************************************/
static int checkout(unsigned char *data, int size)
{

	return 1;
}





/*************************************************
* 函数名: commData_unpack
* 说明：数据拆包并调用分析处理
* 输入参数：data            源数据缓冲
*           size            数据长度
*        //   data_analy      处理方式  
* 输出参数：是否存在有效数据
*************************************************/
int commData_unpack(const unsigned char *data, int size)
{
	const unsigned char *p = data;
	int len = 0;
	int frame_num = 0;
	int i = 0;
	while (1)
	{
		if (size < FREAM_MIN_LEN)         //检查最小长度
			return frame_num;

		if (*p++ == FREAM_HEAD)
		{	
			len = ((*p)<<8) + *(p+1);
			#ifdef PACK_PRINT
				printf("pack len is %d\n", len);
			#endif
			if (len <= (size-2)) //数据长度合法判断   FREAM_HEAD+FREAM_TAIL
			{
			#ifdef DATA_CHECK
				if ((*(p+len) == FREAM_TAIL) && checkout(*(p+2), len-2))//长度判断和校验
			#else
				if (*(p+len) == FREAM_TAIL)//长度判断
			#endif
				{
					cmdData_analysis((p+2), len-2);
					frame_num++;
					size -= (len+1);  //len+FREAM_TAIL
				}
			}
			size -= 3;      //FREAM_HEAD+len
			len = 0;
		}
		else
			size--;

		if (0 == size)      
			return frame_num;
	}

}



/*----------------------------------------------------------------------------*/
/*************************************************
* 函数名: generate_check
* 说明：内置各种校验
* 输入参数：data            数据缓冲
*           size            数据长度
* 输出参数：原数据+校验码的总字节数
**************************************************/
static int generate_check(unsigned char *data, int size)
{


	return 1;
}


/*************************************************
* 函数名: commData_pack
* 说明：数据打包函数
*       外部保证缓冲区有足够长度
* 输入参数：data            发送数据缓冲
*           size            数据长度,包括标识
* 输出参数：原数据+校验码的总字节数
**************************************************/
int commData_pack(unsigned char *data, int size)
{
	int i = 0;
	//往后移动3个字节(长度,帧头)
	for (i = size - 1; i >= 0; i--)
	{
		data[size+3] = data[size];
	}
	data[0] = 0X7F;
	data[1] = size+2 / 256;
	data[2] = size+2 % 256;
	data[size+3] = 0x65;
	return 1;

}
