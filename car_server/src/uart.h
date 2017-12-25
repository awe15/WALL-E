/*************************************************
* 文件名：uart.h     
* 说明：串口模块
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef    UART_H_
#define    UART_H_

#define  UART_PRINT


/*!    车相关信息          */
typedef struct Uart_Info
{
	int baud; 
}Uart_Info;


//获得串口

Uart_Info *get_uartObj(void);
int BSP_uart_init(Uart_Info *uartInfo);
int readFromUart(Uart_Info *uartInfo, char *pdata, unsigned int readSize);
void write2Uart(Uart_Info *uartInfo, char *pdata, unsigned int size);




#endif



