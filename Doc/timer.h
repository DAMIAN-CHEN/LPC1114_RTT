#ifndef __NXPLPC11xx_TIME_H__
#define __NXPLPC11xx_TIME_H__

extern uint16_t cycle;
extern uint8_t duty;

extern void T16B0_init(void);
extern void T16B0_delay_ms(uint16_t ms);
extern void T16B0_delay_us(uint16_t us);
extern void T16B0_cnt_init(void);
extern void T16B0_CAP_Init(void);
extern void T16B0M0_PWM_Init(void);
extern void T16B0M0_PWM_SET(void);

#endif





