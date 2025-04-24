 /*
 * Change Logs:
 * Date           DAMIAN_CHEN       Notes
 * 2024-04-21                  the first version
 */
#include "LPC11XX.H"
#include "rtthread.h"
#include "stdio.h"
#include "math.h"
#include "uart.h"

static rt_thread_t example_task;

const unsigned long led_mask[] = {1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3, 
                                  1UL << 4, 1UL << 5, 1UL << 6, 1UL << 7,1UL << 8,1UL << 9,1UL << 10 };

static float x;//正弦函数参数，采用静态变量
	
																	
/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_RGB_init(void) {

 LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);     /* enable clock for GPIO      */
   
	//LED灯
  /* configure GPIO as output */
   LPC_GPIO1->DIR  |=  (led_mask[9]); //每一位默认为输入（即置零） 输出为该位置1
   LPC_GPIO1->DATA &= ~( led_mask[9]); 
	
	 //RGB三色灯
	 LPC_GPIO2->DIR  |=  ( led_mask[8]| led_mask[9]| led_mask[10]);
 	 LPC_GPIO2->DATA &= ~(led_mask[8]| led_mask[9]| led_mask[10]);  
   LPC_GPIO2->DATA |= (led_mask[8]| led_mask[9]| led_mask[10]); 
}


/*----------------------------------------------------------------------------
  参考教材的PWM输出配置，用系统时钟中断+正弦函数实现呼吸灯
 *----------------------------------------------------------------------------*/
void TMR16B1_PWM_Init(void)
	{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 8);  
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 16);  
	LPC_IOCON->PIO1_9 |= 0x01;  
	LPC_TMR16B1->TCR=0x02;      
	LPC_TMR16B1->PR = 0;        
	LPC_TMR16B1->PWMC= 0x01;    
	LPC_TMR16B1->MCR = 0x02 <<9;  
	LPC_TMR16B1->MR3 = SystemCoreClock/100;  
	LPC_TMR16B1->MR0 = LPC_TMR16B1->MR3/8;
	LPC_TMR16B1->TCR = 0x01;  
}


/*----------------------------------------------------------------------------
  电平翻转函数
 *----------------------------------------------------------------------------*/
void RGB_Toggle(void){
	
	/*由于LED展示呼吸灯，此处用RGB三色灯用作替换*/
	LPC_GPIO2->DATA^=(1UL<<8);
	LPC_GPIO2->DATA^=(1UL<<9);
	LPC_GPIO2->DATA^=(1UL<<10);
}


/* 创建线程入口，计数输出，定时休眠*/
static void  example_task_entry(void *parameter){
  
    while (1){		
        RGB_Toggle();
        rt_thread_mdelay(1000);
        }
}


int main()
{
	
    LED_RGB_init();
	  TMR16B1_PWM_Init();
	
	
	  example_task = rt_thread_create("example_task",example_task_entry, RT_NULL, 512,15, 10);
    /* 如 果 获 得 线 程 控 制 块， 启 动 这 个 线 程 */
    if (example_task!= RT_NULL)
     rt_thread_startup(example_task);


	while(1){
		
		 /*使用正弦函数实现呼吸灯*/
			LPC_TMR16B1->MR0 = LPC_TMR16B1->MR3*0.5f*(1+sin(x));
			x+=0.04;
			if(x>1000)x=0;//防止长时间运行x的值溢出
			rt_thread_mdelay(10);
	    }
	
}

