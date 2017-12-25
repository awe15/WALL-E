/***************************** 
*
*   电机控制驱动程序
*
* 文件名：moter_pwm.c
* 说明：电机方向底层pwm控制
*      输出PWM_CH0，PWM_CH1
*      GPIO#2，#11控制输出
*      左路电机  pwm_CH0   GPIO#2
*      右路电机  pwm_CH1   GPIO#11
* 版本：V1
* 作者：AWE
* 时间：2017/6/10   
*------------------------------------------
* 控制方式:写入4个字节数组
* 
*  字节 |     1                   2                     3                  4
*  范围 |    0-2                0-100                  0-2               0-100
*  含义 |  左电机方向        左电机占空比           右电机方向        右电机占空比 
* 
* 注：方向控制 0表示不变， 1表示正转， 2表示反转  其他值无效
*
*******************************/
#ifndef _MOTER_PWM_H_  
#define _MOTER_PWM_H_  
/***************头文件*******************/
#include <linux/kernel.h>  
#include <linux/version.h>  
#include <linux/fs.h>  
#include <linux/init.h>  
#include <linux/delay.h>  
#include <linux/irq.h>  
#include <asm/uaccess.h>  
#include <asm/irq.h>  
#include <asm/io.h>  
#include <linux/module.h>  
#include <linux/device.h>           
#include <linux/cdev.h>  
#include <linux/interrupt.h>    
#include <linux/sched.h>  
#include <linux/wait.h>  
#include <linux/poll.h>     
#include <linux/fcntl.h>  
#include <linux/spinlock.h> 

 
/************************IO寄存器定义***********************************/
#define    PREG_CLKCFG                        0x10000030      
#define    PREG_CLKCFG_L                        4
#define    PREG_AGPIO_CFG                     0x1000003C
#define    PREG_AGPIO_CFG_L                     4
#define    PREG_GPIO_MODE                     0x10000060
#define    PREG_GPIO_MODE_L                     4
#define    PREG_PWM_BASE                      0x10005000
#define    PREG_PWM_ENABLE                    0x10005000
#define    PREG_PWM_ENABLE_L                    4
#define    PREG_GPIO_CTRL                     0x10000600
#define    PREG_GPIO_CTRL_L                     4
#define    PREG_GPIO_DATA                     0x10000620
#define    PREG_GPIO_DATA_L                     4


#define    PWM_MODE_BIT                         15   
#define    PWM_GVAL_BIT                         8   
#define    PWM_IVAL_BIT                         7    
#define    MOTER1_DIR_IO_BIT                    2          //GPIO#2        
#define    MOTER2_DIR_IO_BIT                    11         //GPIO#11


#define    PWM_CLKCFG_VALUE                   (1 << 31)
#define    PWM_AGPIO_CFG_VALUE                (0x0F<<17)
#define    PWM_GPIO_MODE_VALUE                ~(3 << 28 | 3 << 30)  //PWM1,PWM0
#define    PWM_ENABLE_VALUE(no)               (1 << no) 
#define    GPIO2_MODE_VALUE                   (0x01<<6)       //gpio#2 i2c

 #define PWM_NUM  4                 //PWM通道数 

 u32 PWM_REG[PWM_NUM] = {  
    0x10,    /* pwm0 base */  
    0x50,    /* pwm1 base */  
    0x90,    /* pwm2 base */  
    0xD0     /* pwm3 base */  
};  


enum {  
    PWM_REG_CON,  
    PWM_REG_GDUR = 0x0C,  
    PWM_REG_WNUM = 0x28,  
    PWM_REG_DWID = 0x2C,  
    PWM_REG_THRE = 0x30,  
    PWM_REG_SNDNUM = 0x34,  
}PWM_REG_OFF;

/*****************************************************************/

 
typedef enum PWM_Cmd             //PWM控制命令
{ 
	PWM_ENABLE,            
	PWM_DISABLE, 
	PWM_CONFIGURE,
	PWM_GETSNDNUM,
	PWM_DUTYSET = 5            //设置PWM波
}PWM_Cmd;


/* 时钟源 */  
typedef enum {  
    PWM_CLK_100KHZ,    
    PWM_CLK_40MHZ  
}PWM_CLK_SRC;  
  
  
/* 时钟分频 */  
typedef enum {  
    PWM_CLI_DIV0 = 0,  
    PWM_CLK_DIV2,  
    PWM_CLK_DIV4,  
    PWM_CLK_DIV8,  
    PWM_CLK_DIV16,  
    PWM_CLK_DIV32,  
    PWM_CLK_DIV64,  
    PWM_CLK_DIV128,  
}PWM_CLK_DIV;  
  
  
//pwm结构 
typedef struct pwm_cfg {  
    int no;  
    PWM_CLK_SRC    clksrc;  
    PWM_CLK_DIV    clkdiv;  
    unsigned char  idelval;  
    unsigned char  guardval;  
    unsigned short guarddur;  
    unsigned short wavenum;  
    unsigned short datawidth;  
    unsigned short threshold;  
}pwm_cfg;  

#endif   
