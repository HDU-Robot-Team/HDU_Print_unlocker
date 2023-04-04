#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H



#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "print_mode.h"

#define motor_tim htim2
#define motor_channel TIM_CHANNEL_2
#define sch_gpio GPIOB //���Ӷ��mos�ܿ����ź�
#define sch_pin GPIO_PIN_5

void lock(void);
void unlock(void);

extern uint16_t lock_compare; //��� ����ռ�ձ�
extern uint16_t unlock_compare; //��� ����ռ�ձ�



#endif


