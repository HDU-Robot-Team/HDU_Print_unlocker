#include "standby_mode.h"

//�������ģʽ
void Standby_Entering(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU|PWR_FLAG_SB);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);				//ʹ��WKUP���Ż��ѹ��ܣ�����WKUP���������ز��ܻ���
	HAL_PWR_EnterSTANDBYMode();					//�������ģʽ
}




