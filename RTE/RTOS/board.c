/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           DAMIAN_CHEN       Notes
 * 2024-04-21                  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include "system_LPC11xx.h"
#include "uart.h"
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (2*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/* cortex-m �ܹ�ʹ�� SysTick_Handler() */
void SysTick_Handler()
{
    rt_os_tick_callback();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
//#error "TODO 1: OS Tick Configuration."
    /* 
     * TODO 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */          
		NVIC_SetPriority(SysTick_IRQn,15);	 
		SystemClock_Config();
		SystemCoreClockUpdate (); 
	   
	   SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	   
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE

static int uart_init(void)
{
 //#error "TODO 2: Enable the hardware uart and config baudrate."
	 UART_init(115200);
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
  //#error "TODO 3: Output the string 'str' through the uart."
	 	
	/* �����ٽ�� */
	rt_enter_critical();
	/* ֱ���ַ������� */
	while (*str!='\0')
	{
		/* ���� */
		if(*str=='\n')
		{
			UART_send_byte( '\r');
		}
		UART_send_byte( *str++);
	}
 		
	/* �˳��ٽ�� */
	rt_exit_critical();
}

#endif

