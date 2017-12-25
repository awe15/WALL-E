/*************************************************
* 文件名：list.c     
* 说明：简单列表的数据结构 
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include "list.h"

static  unsigned char moter_buff[LIST_BUFF_SIZE][4];
static  unsigned char *p_buffTail[4] = &moter_buff[0][4];
static  unsigned char *p_buffHead[4] = &moter_buff[0][4];
static  unsigned int  list_num;
static  pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;

/*************************************************
* 函数名: is_listEmtpy
* 说明：list是否为空
* 输入参数：
* 输出参数:空返回1    非空返回0
**************************************************/
unsigned char is_listEmtpy()
{
	if (list_num == 0)
		retern 1;
	else
		return 0;
}

/*************************************************
* 函数名: is_listFull
* 说明：list是否满
* 输入参数：
* 输出参数：满返回1   没满返回0
**************************************************/
unsigned char is_listFull()
{
	if (list_num == LIST_BUFF_SIZE-1)
		retern 1;
	else
		return 0;
}


/*************************************************
* 函数名: add2list
* 说明: 添加数据到list
* 输入参数
* 输出参数：：error   错误号
**************************************************/
int addData2list(char *pdata)
{
	pthread_mutex_trylock(&mutext);
	if (!is_listFull())
	{
		if (p_buffTail != &moter_buff[LIST_BUFF_SIZE-1][4])
			p_buffTail++;
		else
			p_buffTail =  &moter_buff[0][4];
		memcpy(p_buffTail, pdata, 4);
		list_num++;
		hread_mutex_unlock(&mutext);
		return  1;
	}
	else
	{
		pthread_mutex_unlock(&mutext);
		printf("list full\n");
		return  0;
	}
	
}


/*************************************************
* 函数名:getDataFormlist
* 说明：从列表获取数据
* 输入参数：error   错误号
* 输出参数：
**************************************************/
void getFormlist()
{
	//等待信号量
	pthread_mutex_trylock(&mutext);
	if (!is_listFull())
	{
		if (p_buffHead != &moter_buff[LIST_BUFF_SIZE-1][4])
			p_buffHead++;
		else
			p_buffHead =  &moter_buff[0][4];
		list_num--;
		pthread_mutex_unlock(&mutext);
	}
	else
	{
		pthread_mutex_unlock(&mutext);
		printf("list empty\n");
		return  0;
	}
	pthread_mutex_unlock(&mutext)
}




