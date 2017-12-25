/*************************************************
* 文件名：pwm_test.h     
* 说明：电机驱动测试
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/6/10
**************************************************/
#ifndef _PWM_H_  
#define _PWM_H_  
/***************头文件*******************/


 

 #define PWM_NUM  4                 //PWM通道数 
 
 
  
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
