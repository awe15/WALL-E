/*************************************************
* 文件名：car_cntl.h    
* 说明：该模块集合了车的相关信息及其控制
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef    CAR_STATUS_CNTL_H_
#define    CAR_STATUS_CNTL_H_ 
#include  "msg_output.h"
#include   "socket.h"
#include   "moter_cntl.h"
#include   "camera.h"
#include   "uart.h"

#define  CAR_INfO_DEBUG_PRINT
#define  LINK_TIMEOUT_COUNT     10


#define  CAMERA1_PORT       8070
#define  CAMERA2_PORT       8090

#define RECV_BUFF_SIZE       50

/*!    开关状态          */
typedef enum Car_SW_Sta{
	Car_Off,
	Car_On
}Car_SW_Sta;


/*!     连接状态      */
typedef enum Car_Link_Sta{
	Car_Link_Connet,
	Car_Link_Disconnet
}Car_Link_Sta;


/*!    车相关信息          */
typedef struct Car
{
	Car_SW_Sta  sw;                     //总开关状态
	volatile Car_Link_Sta link_sta;     //连接状态
	Moter_Info * moterObj;              //电机
	Camera_Info *cameraObj;             //摄像头
	Uart_Info *uartObj; 
	SocketInfo sock;                    //socket             
	unsigned int timer_count;           
}Car;


Car* get_carObj();                 //获得car控制对象
void car_init(Car* carInfo);       //初始化
void car_waitRemoteLink(Car* carInfo);       //阻塞进行远程链接
Car_Link_Sta get_carLinkStatus(Car* carInfo);      //获得连接状态
void car_recvAnalysisCmd(Car* moInfo);             //接受数据
void  car_linkTimeoutDece(Car* carInfo);

//回调
int car_link_cmdCntl(const unsigned char *data, int size);
int car_sw_cmdCntl(const unsigned char *data, int size);

#endif

