/*************************************************
* 文件名：msg_output.h     
* 说明：简单列表的数据结构 
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef    MESSAGE_OUTPUT_H_
#define    MESSAGE_OUTPUT_H_

#define TERMINAL_OUPUT 

#ifdef TERMINAL_OUPUT 
#define  PRINT    printf
#endif 

#ifdef LOG_OUPUT 
//#define PRINT(str1, str2)   system("echo server start running! >> /car_server/car_server.log");
#endif 

#ifdef ALL_OUTPUT      //同时终端和后台输出

#endif 

#endif
