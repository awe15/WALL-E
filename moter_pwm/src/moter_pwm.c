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
*******************************/
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/random.h>
#include <linux/init.h>
#include <linux/raw.h>
#include <linux/tty.h>
#include <linux/capability.h>
#include <linux/ptrace.h>
#include <linux/device.h>
#include <linux/highmem.h>
#include <linux/crash_dump.h>
#include <linux/backing-dev.h>
#include <linux/bootmem.h>
#include <linux/splice.h>
#include <linux/pfn.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/aio.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include "moter_pwm.h"

static volatile unsigned long *CLKCFG;               //时钟配置寄存器
static volatile unsigned long *AGPIO_CFG;            //模拟GPIO寄存器
static volatile unsigned long *GPIO_MODE;            //配置资源寄存器
static volatile unsigned long *PWM_BASE;             //PWM基础偏移寄存器
static volatile unsigned long *PWM_ENABLE_REG;
static volatile unsigned long *GPIO_DATA;
static volatile unsigned long *GPIO_CTRL;

static spinlock_t pwm_lock;

static void pwm_disable(int no)  
{  
    unsigned long  flags;  
      
    printk("disable pwm%d\n", no);  
  
    spin_lock_irqsave(&pwm_lock, flags);  
    *PWM_ENABLE_REG  &= ~PWM_ENABLE_VALUE(no);      
    spin_unlock_irqrestore(&pwm_lock, flags);  
	
}  

static void pwm_enable(int no)  
{  
	unsigned long  flags;  
    printk("enable pwm%d\n", no);  
  
    spin_lock_irqsave(&pwm_lock, flags);  
    *PWM_ENABLE_REG  |= PWM_ENABLE_VALUE(no);    
    spin_unlock_irqrestore(&pwm_lock, flags);  
	
}  




//配置pwm  
static void pwm_config(struct pwm_cfg *cfg)  
{  
    u32 value = 0;  
    unsigned long  flags;  
    u32 basereg;  
    volatile unsigned long *tReg;
    printk("config PWM%d\n", cfg->no); 
	
    basereg = PWM_REG[cfg->no];   
  	
    spin_lock_irqsave(&pwm_lock, flags);     //锁
      
    /* 1.设置PWM控制寄存器 */  
    tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_CON),4);

	/* old PWM模式 */  
    value |= PWM_GPIO_MODE_VALUE;  
	
    /*设置空闲值和输出值*/  
    value &= ~((1 << PWM_IVAL_BIT) | (1 << PWM_GVAL_BIT));  
    value |= ((cfg->idelval & 0x1) << PWM_IVAL_BIT);  
    value |= ((cfg->guardval & 0x1) << PWM_GVAL_BIT);  
	
    /* 设置信号源时钟*/  
    if (cfg->clksrc == PWM_CLK_100KHZ) {  
        value &= ~(1<<3);             //设置时钟为100KHZ
    } else {  
        value |= (1<<3);             //设置时钟为100MHZ
    }  
    /* 设置时钟分频 */  
    value &= ~0x7;  
    value |= (0x7 & cfg->clkdiv);  
    
     *tReg = value;   
     iounmap(tReg); //解映射

    /* 2. 设置guard duration值 */
    tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_GDUR),4);
    value = 0;
    value |= (cfg->guarddur & 0xffff);  
    *tReg = value;   
    iounmap(tReg); //解映射
      
      
    /* 3.设置脉冲产生次数，为0则一直不断产生，直到是失能 */  
    tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_WNUM),4); 
    value = 0;  
    value |= (cfg->wavenum & 0xffff);  
    *tReg = value;   
    iounmap(tReg); //解映射 
      
      
    /* 4.设置数据宽度值 */  
    tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_DWID),4); 
    value = 0; 
    value |= (cfg->datawidth & 0x1fff);  
    *tReg = value;   
    iounmap(tReg); //解映射    
      
      
    /* 5. 设置thresh值*/  
   tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_THRE),4); 
    value = 0; 
    value |= (cfg->threshold & 0x1fff);  
    *tReg = value;   
    iounmap(tReg); //解映射       
  

    spin_unlock_irqrestore(&pwm_lock, flags); 
	
} 
 
//读取PWM数
static void pwm_getsndnum(struct pwm_cfg *cfg)  
{  
    u32 value;  
    unsigned long  flags;  
    u32 basereg = PWM_REG[cfg->no]; 
    volatile unsigned long *tReg; 
      
	printk("read output PWM%d number...\n", cfg->no);    
    spin_lock_irqsave(&pwm_lock, flags);  
    tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_SNDNUM),4);
    cfg->wavenum = *tReg;  
	iounmap(tReg); //解映射 
    spin_unlock_irqrestore(&pwm_lock, flags);  
	
}  

//设置PWM波占空比
static void pwm_dutySet(struct pwm_cfg *cfg)
{
	u32 value = 0;
	u32 basereg;  
	unsigned long  flags;  
	 volatile unsigned long *tReg;
	basereg = PWM_REG[cfg->no];   
	/* 设置thresh值*/  
	//printk("set PWM%d duty\n", cfg->no);  
    spin_lock_irqsave(&pwm_lock, flags);  
    tReg = (volatile unsigned long *)ioremap((PREG_PWM_BASE + basereg + PWM_REG_THRE),4);
    *tReg &= ~(0x1fff);  
    value |= (cfg->threshold & 0x1fff);  
    *tReg = value;
    spin_unlock_irqrestore(&pwm_lock, flags);  
	
}



//初始化
static void gpio_init(void)
{
	//GPIO初始化
	GPIO_CTRL = (volatile unsigned long *)ioremap(PREG_GPIO_CTRL, PREG_GPIO_CTRL_L);
	GPIO_DATA = (volatile unsigned long *)ioremap(PREG_GPIO_DATA, PREG_GPIO_DATA_L);

	*GPIO_CTRL |= (1<<MOTER1_DIR_IO_BIT) | (1<<MOTER2_DIR_IO_BIT);
	*GPIO_DATA |= (1<<MOTER1_DIR_IO_BIT) | (1<<MOTER2_DIR_IO_BIT);
        *GPIO_MODE |= GPIO2_MODE_VALUE;   
}

static void pwm_init(void)
{
	int i = 0;
	//io映射
	CLKCFG = (volatile unsigned long *)ioremap(PREG_CLKCFG, PREG_GPIO_MODE_L);
	AGPIO_CFG = (volatile unsigned long *)ioremap(PREG_AGPIO_CFG, PREG_AGPIO_CFG_L);
	GPIO_MODE = (volatile unsigned long *)ioremap(PREG_GPIO_MODE, PREG_GPIO_MODE_L);
	PWM_ENABLE_REG = (volatile unsigned long *)ioremap(PREG_PWM_ENABLE, PREG_PWM_ENABLE_L);

	PWM_BASE = PWM_ENABLE_REG;
	
	//pwm ch0ch1
	*CLKCFG |= PWM_CLKCFG_VALUE;                       //使能时钟
	*AGPIO_CFG |= PWM_AGPIO_CFG_VALUE;                 //设置为AGPIO的物理配置
	*GPIO_MODE &= PWM_GPIO_MODE_VALUE;                 //设置PWM模式  
	  

	//失能所有pwm
	for (i = 0; i < PWM_NUM; i++) 
	{  
        	pwm_disable(i);    
    }  
}


static void gpio_exit(void)
{
	iounmap(GPIO_CTRL);
	iounmap(GPIO_DATA);
}

static  void pwm_exit(void)
{
	//解除GPIO映射
	iounmap(CLKCFG);
	iounmap(AGPIO_CFG);
	iounmap(GPIO_MODE);
	iounmap(PWM_ENABLE_REG);
}

//PWM命令操作
static int pwm_ctl(struct pwm_cfg *cfg, unsigned int cmd)
{
	switch (cmd) 
	{  
		case PWM_ENABLE:  
			pwm_enable(((struct pwm_cfg *)cfg)->no);  
			break;  
		case PWM_DISABLE:  
			pwm_disable(((struct pwm_cfg *)cfg)->no);  
			break;  
		case PWM_CONFIGURE:  
			pwm_config((struct pwm_cfg *)cfg);  
			break;  
		case PWM_GETSNDNUM:  
			pwm_getsndnum((struct pwm_cfg *)cfg);  
			break; 
		case PWM_DUTYSET:  
			pwm_dutySet((struct pwm_cfg *)cfg);  
			break;  
		default:  
			return -1;  
	}
	return 1;
}


//初始化函数必要资源定义
//用于初始化函数当中
//device number;
dev_t dev_num;
//struct dev
struct cdev pwmdrv_cdev;
struct class *pwmdrv_class = NULL;
struct device *pwmdrv_device = NULL;

pwm_cfg  pwm_ch1, pwm_ch2; 

/**************** 结构体 file_operations 成员函数 *****************/
//open
static int pwmdrv_open(struct inode *inode, struct file *file)
{
	printk("moter pwm drive open...\n");
	
	//PWM_ch1
	pwm_ch1.no        =   0;    /* pwm0 */  
   	pwm_ch1.clksrc    =   PWM_CLK_40MHZ;   
    	pwm_ch1.clkdiv    =   PWM_CLK_DIV2;  
    	pwm_ch1.idelval   =   0;    
    	pwm_ch1.guardval  =   0;  
    	pwm_ch1.guarddur  =   0;   
    	pwm_ch1.wavenum   =   0;  /* forever loop */  
    	pwm_ch1.datawidth =   1000;  
    	pwm_ch1.threshold =   0;       //duty=threshold/datawidth*100
	
	//PWM_ch2
	pwm_ch1.no        =   1;    /* pwm1 */  
    	pwm_ch2.clksrc    =   PWM_CLK_40MHZ;   
    	pwm_ch2.clkdiv    =   PWM_CLK_DIV2;  
    	pwm_ch2.idelval   =   0;    
    	pwm_ch2.guardval  =   0;  
    	pwm_ch2.guarddur  =   0;   
    	pwm_ch2.wavenum   =   0;  /* forever loop */  
    	pwm_ch2.datawidth =   1000;  
   	pwm_ch2.threshold =   0;   

	pwm_ctl(&pwm_ch1, PWM_CONFIGURE);
	pwm_ctl(&pwm_ch2, PWM_CONFIGURE);	
	pwm_ctl(&pwm_ch1, PWM_ENABLE);
	pwm_ctl(&pwm_ch2, PWM_ENABLE);
	
	//gpioctl
	*GPIO_DATA |= (1<<MOTER1_DIR_IO_BIT);
	*GPIO_DATA |= (1<<MOTER2_DIR_IO_BIT);
	return 0;
}


//close
static int pwmdrv_close(struct inode *inode , struct file *file)
{
	printk("moter pwm drive close...\n");
	
	pwm_ctl(&pwm_ch1, PWM_DISABLE);
	pwm_ctl(&pwm_ch2, PWM_DISABLE);

	//gpioctl
	*GPIO_DATA |= (1<<MOTER1_DIR_IO_BIT);
	*GPIO_DATA |= (1<<MOTER2_DIR_IO_BIT);
	return 0;
}

static ssize_t pwmdrv_write( struct file *file , const char __user *buffer,
			   size_t len , loff_t *offset );

static ssize_t pwmdrv_read(struct file *file, char __user *buffer,
			size_t len, loff_t *pos);
/***************** 结构体： file_operations ************************/
//struct
static const struct file_operations pwmdrv_fops = {
	.owner   = THIS_MODULE,
	.open	 = pwmdrv_open,
	.release = pwmdrv_close,	
	.read	 = pwmdrv_read,
	.write   = pwmdrv_write,
};

/*************  functions: init , exit*******************/
//条件值变量，用于指示资源是否正常使用
unsigned char init_flag = 0;
unsigned char add_code_flag = 0;

//init
static __init int pwmdrv_init(void)
{
	int ret_v = 0;
	printk("pwmdrv drive init...\n");
	printk("pwmdrv v1.0...\n");

	//函数alloc_chrdev_region主要参数说明：
	//参数2： 次设备号
	//参数3： 创建多少个设备
	if( ( ret_v = alloc_chrdev_region(&dev_num,0,1,"pwmdrv") ) < 0 )
	{
		goto dev_reg_error;
	}
	init_flag = 1; //标示设备创建成功；

	printk("pwmdrv drive init...\n");

	//函数alloc_chrdev_region主要参数说明：
	//参数2： 次设备号
	//参数3： 创建多少个设备
	if( ( ret_v = alloc_chrdev_region(&dev_num,0,1,"pwmdrv") ) < 0 )
	{
		goto dev_reg_error;
	}
	init_flag = 1; //标示设备创建成功；

	printk("The drive info of pwmdrv:\nmajor: %d\nminor: %d\n",
		MAJOR(dev_num),MINOR(dev_num));

	cdev_init(&pwmdrv_cdev,&pwmdrv_fops);
	if( (ret_v = cdev_add(&pwmdrv_cdev,dev_num,1)) != 0 )
	{
		goto cdev_add_error;
	}

	pwmdrv_class = class_create(THIS_MODULE,"pwmdrv");
	if( IS_ERR(pwmdrv_class) )
	{
		goto class_c_error;
	}

	pwmdrv_device = device_create(pwmdrv_class,NULL,dev_num,NULL,"pwmdrv");
	if( IS_ERR(pwmdrv_device) )
	{
		goto device_c_error;
	}
	printk("auto mknod success!\n");

	//------------   请在此添加您的初始化程序  --------------//
	pwm_init();
	gpio_init();
	//----------------------  END  ---------------------------// 

	goto init_success;
	
//------------------ 请在此添加您的错误处理内容 ----------------//
dev_reg_error:
	printk("alloc_chrdev_region failed\n");	
	return ret_v;

cdev_add_error:
	printk("cdev_add failed\n");
 	unregister_chrdev_region(dev_num, 1);
	init_flag = 0;
	return ret_v;

class_c_error:
	printk("class_create failed\n");
	cdev_del(&pwmdrv_cdev);
 	unregister_chrdev_region(dev_num, 1);
	init_flag = 0;
	return PTR_ERR(pwmdrv_class);

device_c_error:
	printk("device_create failed\n");
	cdev_del(&pwmdrv_cdev);
 	unregister_chrdev_region(dev_num, 1);
	class_destroy(pwmdrv_class);
	init_flag = 0;
	return PTR_ERR(pwmdrv_device);

	add_code_flag = 0;
	return -1;
//--------------------          END         -------------------//
  
init_success:
	printk("pwmdrv init success!\n");
	return 0;
}

//exit
static __exit void pwmdrv_exit(void)
{
	printk("pwmdrv drive exit...\n");	

	if(add_code_flag == 1)
 	{   
           //----------   请在这里释放您的程序占有的资源   ---------//
	    printk("free your resources...\n");	   
		
		//接触GPIO映射
		pwm_exit();
		gpio_exit();

	    printk("free finish\n");		               
	    //----------------------     END      -------------------//
	}					            

	if(init_flag == 1)
	{
		//释放初始化使用到的资源;
		cdev_del(&pwmdrv_cdev);
 		unregister_chrdev_region(dev_num, 1);
		device_unregister(pwmdrv_device);
		class_destroy(pwmdrv_class);
	}
}

//write 写入速度和方向
static ssize_t pwmdrv_write( struct file *file , const char __user *buffer,
			   size_t len , loff_t *offset )
{
	int ret_v = 0;
	pwm_cfg* oper_ch;
	printk("set car's dirction and change the speed\n");
	if (len != 4)
	{printk("input arg size error!\n");return 1;}
	/* left moter */
	if (buffer[0] == 1)    //forward
	{
		oper_ch = &pwm_ch1;
		*GPIO_DATA |= (1<<MOTER1_DIR_IO_BIT);
		printk("pwm_ch0 forward\n");
	}
	else if (buffer[0] == 2)  //back
	{	
		oper_ch = &pwm_ch1;
		*GPIO_DATA &= ~(1<<MOTER1_DIR_IO_BIT);
		printk("pwm_ch0 back\n");
	}
	else if (buffer[0] != 0)
	{  
		printk("left moter has bad arg!!\n");
		return -1;
	}
	if (buffer[1] <= 100 && buffer[1] >= 0)
	{
		printk("duty=%d\n", buffer[1]);
		oper_ch->threshold = ((long int)buffer[1]*(oper_ch->datawidth)/100);
		pwm_ctl(oper_ch, PWM_DUTYSET);
	}
	else
	{
		printk("left moter pwm duty has bad arg!!\n");
		return -1;
	}

	/*right moter */
	if (buffer[2] == 1)    //forward
	{
		oper_ch = &pwm_ch2;
		*GPIO_DATA |= (1<<MOTER2_DIR_IO_BIT);
		printk("pwm_ch1 forward\n");
	}
	else if (buffer[2] == 2)  //back
	{	
		oper_ch = &pwm_ch2;
		*GPIO_DATA &= ~(1<<MOTER2_DIR_IO_BIT);
		printk("pwm_ch1 back\n");
	}
	else if (buffer[2] != 0)
	{  
		printk("left moter has bad arg!!\n");
		return -1;
	}

	if (buffer[3] <= 100 && buffer[3] >= 0)
	{
		printk("duty=%d\n", buffer[3]);
		oper_ch->threshold = ((long int)buffer[3]*(oper_ch->datawidth)/100);
		pwm_ctl(oper_ch, PWM_DUTYSET);
	}
	else
	{
		printk("right moter pwm duty has bad arg!!\n");
		return -1;
	}
	return ret_v;
}

//read 读取当前车速
static ssize_t pwmdrv_read(struct file *file, char __user *buffer,
			size_t len, loff_t *pos)
{
	int ret_v = 0;	
	printk("read current car speed:\n");
	if (len > 2)
	{printk("input arg size error!\n");return 1;}
		
	pwm_ctl(&pwm_ch1, PWM_GETSNDNUM);
	pwm_ctl(&pwm_ch2, PWM_GETSNDNUM);
	buffer[0] = ((char)(pwm_ch1.threshold)/(pwm_ch1.datawidth));
	buffer[1] = ((char)(pwm_ch2.threshold)/(pwm_ch2.datawidth));
	return ret_v;
}

/**************** module operations**********************/
//module loading
module_init(pwmdrv_init);
module_exit(pwmdrv_exit);

//some infomation
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("from AWE");
MODULE_DESCRIPTION("pwmdrv drive");


/*********************  The End ***************************/
