/*************************************************
* 文件名：car_server.c     
* 说明： 瓦力车的主程序
*     作为一个运行应用
*     开发板作为服务器端设置ip和端口号
*     主要实现：
*     1.协议解析
*     2.摄像头的开关上传控制
*     3.小车运行控制
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <memory.h>
#include "car_cntl.h"
#include "socket.h"
#include "pack.h"

#define RECV_BUFF_SIZE       50
pthread_t socket_th_id;  
pthread_t moter_th_id;  
pthread_t timer_th_id; 

static  Car * car;

/*************************************************
* 函数名: socketListen_thread
* 说明：scoket监听线程。
* 输入参数：
* 输出参数：
**************************************************/
void * socketListen_thread(void * arg)   
{  
	system("echo start socket listen thread! >> /car_server/car_server.log");
	printf("start socket listen thread!\n");  
	while (1)   
	{  
		//如果处于未连接状态，进行socket监听连接
		if (get_carLinkStatus(car) == Car_Link_Disconnet)
		{
			system("echo remote link! >> /car_server/car_server.log");
			car_waitRemoteLink(car);
		}

		car_recvAnalysisCmd(car);//接受数据

		usleep(2000);    
	}  
	return (void *)0;  
}  


/*************************************************
* 函数名: moterControl_thread
* 说明：电机控制线程。
* 输入参数：
* 输出参数：
**************************************************/
void * moterControl_thread(void * arg)   
{
	system("echo start moter control thread! >> /car_server/car_server.log");
	printf("start moter control thread!\n");  
	while (1)
	{
		//电机控制20ms一次
		usleep(20000);   
	}
}


/*************************************************
* 函数名: timer_thread
* 说明：电机控制线程。
* 输入参数：
* 输出参数：
**************************************************/
void * timer_thread(void * arg)  
{
	system("echo start timer thread! >> /car_server/car_server.log");
	printf("start timer thread!\n");  
	while (1)
	{	
		car_linkTimeoutDece(car);
		sleep(1);   
	}
} 


/*************************************************
* 函数名: main
* 说明：主函数
* 输入参数：
* 输出参数：
**************************************************/
int main(int argc , char** argv)
{
	int err = 0;
	sleep(10);
	printf("car server start running!\n");
	system("echo server start running! >> /car_server/car_server.log");
	car = get_carObj();
	car_init(car);
	err = pthread_create(&socket_th_id, NULL, socketListen_thread, NULL);  
	err = pthread_create(&moter_th_id, NULL, moterControl_thread, NULL);
	err = pthread_create(&timer_th_id, NULL, timer_thread, NULL);
	while (1)
	{
		sleep(1);
	}
	

    return 0;
}



