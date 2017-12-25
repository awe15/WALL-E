/*************************************************
* 文件名：car_cntl.c     
* 说明：该模块集合了车的相关信息及其控制
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include "car_cntl.h"

static  Car    carInfo_ctnl;
static  unsigned char recv_buff[RECV_BUFF_SIZE] = {0};  //enough

/*******--------------------------************/
void set_carLinkStatus(Car* carInfo, Car_Link_Sta sta);

/*************************************************
* 函数名: car_linkTimeoutFunc
* 说明：连接超时回调
* 输入参数：
* 输出参数：
*************************************************/
void car_stop(Car* carInfo)
{
	set_moterSwitch(carInfo->moterObj, Moter_SW_Off);
}

/*************************************************
* 函数名: car_linkTimeoutDece
* 说明：连接超时递减
* 输入参数：
* 输出参数：
*************************************************/
void  car_linkTimeoutDece(Car* carInfo)
{
	if (get_carLinkStatus(carInfo) == Car_Link_Disconnet)
		return;
	if (carInfo->timer_count > 0)
		carInfo->timer_count--;
	else
	{
		car_stop(carInfo);
		set_carLinkStatus(carInfo, Car_Link_Disconnet);
	}
}

/*************************************************
* 函数名: carInfo_init
* 说明：信息状态初始化
* 输入参数：carInfo
* 输出参数：
**************************************************/
static void car_resetLinkTime(Car* carInfo)
{
	carInfo->link_sta = Car_Link_Connet;
	carInfo->timer_count = LINK_TIMEOUT_COUNT;
}



/*************************************************
* 函数名: carHardware_init
* 说明：硬件初始化
* 输入参数：
* 输出参数：
**************************************************/
static void carHardware_init(Car* carInfo)
{ 
	//socket init
	socket_init(&carInfo->sock, SERVER_IP, SERVER_DATA_PORT, Socket_TCP);  
	set_carLinkStatus(carInfo, Car_Link_Connet);

	//moter init
	Moter_Info *moterObj = get_moterObj(); 
	BSP_moter_init(moterObj); 
	carInfo->moterObj = moterObj;

	//camera
	Camera_Info *cameraObj = get_cameraObj();
	BSP_camera_init(cameraObj, CAMERA1_PORT, CAMERA2_PORT);
	carInfo->cameraObj = cameraObj;

	//串口
	Uart_Info* uart = get_uartObj();
	BSP_uart_init(uart); 
	carInfo->uartObj = uart;

	//创建定时器
	//create_timer(&carInfo->link_timer, car_linkTimeoutFunc);
}



/*************************************************
* 函数名: carInfo_init
* 说明：信息状态初始化
* 输入参数：carInfo
* 输出参数：
**************************************************/
void car_init(Car* carInfo)
{
#ifdef  CAR_INfO_DEBUG_PRINT
        printf("car init \n");
#endif
	carInfo->sw = Car_Off;
	carInfo->link_sta = Car_Link_Disconnet;
	carInfo->timer_count = LINK_TIMEOUT_COUNT;	
	
	carHardware_init(carInfo);
}

/*************************************************
* 函数名: car_waitRemoteLink
* 说明：阻塞进行远程链接
* 输入参数：carInfo
* 输出参数：
**************************************************/
void car_waitRemoteLink(Car* carInfo)
{
	socket_listen(&(carInfo->sock));
	set_carLinkStatus(carInfo, Car_Link_Connet);
	carInfo->timer_count = LINK_TIMEOUT_COUNT;
	//setTimer(&carInfo->link_timer, 5000);
}


/*************************************************
* 函数名: get_carCtnlObj
* 说明：获得car控制对象
* 输入参数：
* 输出参数：
**************************************************/
Car* get_carObj()
{
	return &carInfo_ctnl;
}


/*************************************************
* 函数名:  get_linkStatus
* 说明：获得car信息开关状态
* 输入参数：
* 输出参数：carInfo_ctnl
**************************************************/
Car_Link_Sta get_carLinkStatus(Car* carInfo)
{
	return carInfo->link_sta; 
}

/*************************************************
* 函数名:  set_linkStatus
* 说明：设置car信息开关状态
* 输入参数：
* 输出参数：carInfo_ctnl
**************************************************/
void set_carLinkStatus(Car* carInfo, Car_Link_Sta sta)
{
	carInfo->link_sta = sta;
	#ifdef  CAR_INfO_DEBUG_PRINT
	if (carInfo->link_sta == Car_Link_Connet)
		printf("car is linking! \n");
	else
		printf("car is outof linking\n");
	#endif

}


/*************************************************
* 函数名: get_carSwStatus
* 说明：获得car信息开关状态
* 输入参数：
* 输出参数：carInfo_ctnl
**************************************************/
Car_SW_Sta get_carSwStatus(Car* carInfo)
{
	return carInfo->sw;
}


/*************************************************
* 函数名: get_carSwSta
* 说明：获得car信息开关状态
* 输入参数：
* 输出参数：carInfo_ctnl
**************************************************/
static void set_carSwStatus(Car* carInfo, Car_SW_Sta sta)
{
	carInfo->sw = sta;
}



/*************************************************
* 函数名: car_ansSta
* 说明：协议命令返回应答
* 输入参数：Moter_Info
*           dataBuff         发送数据缓冲区
* 输出参数：
*************************************************/
void car_ansStatus(Car* carInfo, unsigned char cmd)
{
	unsigned char sendbuf[10];
	sendbuf[0] = cmd;
	sendbuf[1] = 0x01;
	commData_pack(sendbuf, 2);
}

/*************************************************
* 函数名: car_sw_cmdCntl
* 说明：开关控制协议解析
* 输入参数：data        接收数据
*           size        数据大小 
* 输出参数：命令号
**************************************************/
int car_sw_cmdCntl(const unsigned char *data, int size)
{
	if ((data[0] != 0x01) && (data[0] != 0x02))
		return 0;
	if (data[0] == 0x01)    //打开
	{
	#ifdef  CAR_INfO_DEBUG_PRINT
        	printf("car switch open \n");
	#endif
		carInfo_ctnl.sw = Car_On;
		//开电机，开摄像头
		set_moterSwitch(carInfo_ctnl.moterObj, Moter_SW_On);	
		set_cameraSwitch(carInfo_ctnl.cameraObj, Camera_SW_On);
	}
	else
	{	
	#ifdef  CAR_INfO_DEBUG_PRINT
        	printf("car switch close \n");
	#endif
		carInfo_ctnl.sw = Car_Off;
		//关闭电机，开摄像头
		set_moterSwitch(carInfo_ctnl.moterObj, Moter_SW_Off);
		set_cameraSwitch(carInfo_ctnl.cameraObj, Camera_SW_Off);

	}
	car_ansStatus(&carInfo_ctnl, data[0]);
	return 1;
}

/*************************************************
* 函数名: car_link_cmdCntl
* 说明：连接确认协议解析
* 输入参数：data        接收数据
*           size        数据大小 
* 输出参数：命令号
**************************************************/
int car_link_cmdCntl(const unsigned char *data, int size)
{
	if (data[0] != 0x03)
		return 0;
	#ifdef  CAR_INfO_DEBUG_PRINT
        printf("car remote link \n");
	#endif
	car_resetLinkTime(&carInfo_ctnl);
	car_ansStatus(&carInfo_ctnl, data[0]);
	return 1;
}


/*************************************************
* 函数名: car_recvAnalysisCmd
* 说明：接受并解析协议
* 输入参数：
* 输出参数：命令号
**************************************************/
void car_recvAnalysisCmd(Car* carInfo)
{
	int recv_len = 0;
	
	if (socket_recv(&carInfo->sock, recv_buff, RECV_BUFF_SIZE, &recv_len) != No_Err)
	{
#ifdef  CAR_INfO_DEBUG_PRINT
		printf("socket unlink!\n");
#endif
	}
	if (recv_len > 0)
	{
#ifdef  CAR_INfO_DEBUG_PRINT			
		char i = 0;
		printf("thread:\nrecv len %d, data:", recv_len);
		for ( ; i < recv_len; i++)
			printf("%02x ", recv_buff[i]);
		printf("\n");
#endif
		car_resetLinkTime(&carInfo_ctnl);
		write2Uart(carInfo->uartObj, recv_buff, recv_len);
		commData_unpack(recv_buff, recv_len);		
		recv_len = 0;
	}
}


