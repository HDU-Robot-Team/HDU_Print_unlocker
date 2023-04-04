#include "motor_control.h"

uint16_t lock_compare=50; //��� ����ռ�ձ�
uint16_t unlock_compare=154; //��� ����ռ�ձ�

void unlock()
{
		HAL_GPIO_WritePin(sch_gpio,sch_pin,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&motor_tim,motor_channel);
		Compare_expected=unlock_compare;
}
void lock()
{
	int i=0;
	Compare_expected=lock_compare;
	while(Compare_now!=Compare_expected)
	{
		i++;
		HAL_Delay(20);
		if(i>400)
			break;
	}
		HAL_TIM_PWM_Stop(&motor_tim,motor_channel);
		HAL_GPIO_WritePin(sch_gpio,sch_pin,GPIO_PIN_RESET);
}

