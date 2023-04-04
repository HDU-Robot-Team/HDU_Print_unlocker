#ifndef __PRINT_MODE_H
#define __PRINT_MODE_H


#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor_control.h"
#include "oled4p.h"
#include "keys_4x4.h"
#include "string.h"
#define __HAL_DMA_SET_COUNTER(__HANDLE__, __COUNTER__) ((__HANDLE__)->Instance->CNDTR = (uint16_t)(__COUNTER__))



#define MAX_LEN (30)
#define ID_OUT 0X05U
#define print_uart huart2
#define sign_gpio GPIOA  //touch信号
#define sign_pin GPIO_PIN_0

#define power_gpio GPIOB  //电路断电信号
#define power_pin  GPIO_PIN_10

#define print_gpio GPIOA //连接指纹开关信号
#define print_pin GPIO_PIN_11




struct print_flag
{
	uint16_t head;
	uint32_t address;
	uint8_t parameter[2],pkg_id,cmd;
	uint16_t pkg_size,print_id,score,sum;
};
extern uint8_t CMD_ClearALL[17];
extern uint8_t CMD_AutoIdentify[17];
extern uint8_t CMD_AutoEnroll[17];
extern uint8_t CMD_ValidTempleteNum[17];
extern uint8_t CMD_Cancel[17];
extern uint8_t CMD_SetChipAddr[17];
extern uint16_t count_down;
extern uint32_t electric_quantity[10];
extern uint8_t  Compare_expected,Compare_now;
extern uint8_t Print_ID_[4]; //指纹id，自行设置以防串口直接开锁
extern volatile struct print_flag reply_buf;
extern  uint8_t buf_rec[MAX_LEN];
extern uint8_t cmd_got;
extern uint8_t Unlocked;
void print_mode_init(void);
void print_fun(void);
void power_on(void);
void power_off(void);
void lock(void);
void unlock(void);
void SET_ID(uint16_t id_1,uint16_t id_2,uint16_t id_3,uint16_t id_4);
extern uint16_t print_max_id;
void uart_receive_handler(UART_HandleTypeDef *huart);
void uart_rx_idle_callback(UART_HandleTypeDef* huart);
int uart_receive_dma_no_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t Size);
HAL_StatusTypeDef Send_CMD(UART_HandleTypeDef* huart, uint8_t* CMD,uint16_t print_id);
#endif


