/*************************************************
* 文件名：pwm_test.c     
* 说明：电机驱动测试
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/6/10
**************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
//#include "pwm.h"  
  
#define PWM_DEV "/dev/pwmdrv"  
char buf[] = {0, 0, 0, 0}; 
int main(int argc, char *argv)  
{  
    int pwm_fd;  
    int i = 0;
	
	
    pwm_fd = open(PWM_DEV, O_RDWR);  
    if (pwm_fd < 0) {  
        printf("open pwm fd failed\n");  
        return -1;  
    }

    printf("forward test\n");
    buf[0] = buf[2] = 1; //forward  
    for (i = 0; i < 20; i++)
    {
	buf[1] = i*5;
	printf("buf[1]:%d\n", buf[1]);
	buf[3] = 100 - i*5;
	printf("buf[1]:%d\n", buf[3]);
   	write(pwm_fd, buf, sizeof(buf));
   	sleep(3);
    }

    printf("-------------------------\n");
    printf("backward test\n");
    buf[0] = buf[2] = 2; //backward  
    for (i = 0; i < 20; i++)
    {
	buf[3] = i*5;
	printf("buf[3]:%d\n", buf[1]);
	buf[1] = 100 - i*5;
	printf("buf[1]:%d\n", buf[1]);
   	write(pwm_fd, buf, sizeof(buf));
   	sleep(3);
    }   

    printf("--------------\n");
    printf("keep test\n");
    buf[0] = buf[2] = 0; //keep
    buf[1] = buf[3] = 50; 
    write(pwm_fd, buf, sizeof(buf));

    printf("--------------\n");
    printf("error dirction arg\n");
    buf[0] = buf[2] = 3; 
    buf[1] = buf[3] = 50;
    write(pwm_fd, buf, sizeof(buf));
    printf("error duty arg\n");
    buf[0] = buf[2] = 1; 
    buf[1] = buf[3] = -1;
    write(pwm_fd, buf, sizeof(buf));
    buf[1] = buf[3] = 101;
    write(pwm_fd, buf, sizeof(buf));
    
	close(pwm_fd);
	
    return 0;  
}  
