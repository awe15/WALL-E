/*************************************************
* 文件名：uart.c     
* 说明：串口模块
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  

#define   UART_DEV         "/dev/ttyS2"  

static Uart_Info uartObj;
static int uart_fd;

/*************************************************
* 函数名: uart_init
* 说明：串口初始化
* 输入参数：moInfo
* 输出参数：
**************************************************/
static void uart_init(Uart_Info *uartInfo)
{
	uartInfo->baud = 9600;	
}

/*************************************************
* 函数名: BSP_uart_init
* 说明：串口初始化
* 输入参数：moInfo
* 输出参数：
**************************************************/
int BSP_uart_init(Uart_Info *uartInfo)
{
	static char first_init_flg = 0;
	if (!first_init_flg)
	{
	#ifdef  UART_PRINT
		printf("Initialize the uart device\n");
	#endif
		//打开PWM驱动
	    	uart_fd = open(UART_DEV, O_RDWR);  
	    	if (uart_fd < 0) 
		{  
			#ifdef  UART_PRINT
	       		printf("open the uart driver failed!\n"); 
			#endif		
	       		return -1;  
	    	}  
		else
		{
			#ifdef  UART_PRINT
			printf("open the uart driver success!\n"); 
			#endif	
		}
		uart_init(uartInfo);
	}
	else
	{
		#ifdef  UART_PRINT
		printf("uart device had been initialized！\n"); 
		#endif	
	}
	return 0;
}


/*************************************************
* 函数名: get_uartObj
* 说明:获得串口
* 输入参数：
* 输出参数：
**************************************************/
Uart_Info *get_uartObj(void)
{
	return &uartObj;
}


/*************************************************
* 函数名: write2Uart
* 说明:写串口
* 输入参数：
* 输出参数：
**************************************************/
void write2Uart(Uart_Info *uartInfo, char *pdata, unsigned int size)
{
	int readRet = write(uart_fd, pdata, size);
	#ifdef  UART_PRINT
	if (readRet == -1)
	{
		printf("uart write error!\n");
	}
	else
	{
		int i = 0;
		printf("uart write data:\n");
		for (i = 0 ; i < size; i++)
			printf("%02x ", pdata[i]);
	}
	#endif
}

/*************************************************
* 函数名: write2Uart
* 说明:读串口
* 输入参数：
* 输出参数：实际读到的值
**************************************************/
int readFromUart(Uart_Info *uartInfo, char *pdata, unsigned int readSize)
{
	int l_read = 0;
	l_read = read(uart_fd, pdata, readSize);
	#ifdef  UART_PRINT
	if (l_read == -1)
	{
		printf("uart read error!\n");
	}
	else
	{
		int i = 0;
		printf("uart read data:\n");
		for (i = 0 ; i < l_read; i++)
			printf("%02x ", pdata[i]);
	}
	#endif
	return l_read;
}




