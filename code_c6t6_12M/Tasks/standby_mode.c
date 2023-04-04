#include "standby_mode.h"

//进入待机模式
void Standby_Entering(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU|PWR_FLAG_SB);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);				//使能WKUP引脚唤醒功能，否则WKUP引脚上升沿不能唤醒
	HAL_PWR_EnterSTANDBYMode();					//进入待机模式
}




