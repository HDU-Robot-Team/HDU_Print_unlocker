#ifndef __KEYS_4X4_H
#define __KEYS_4X4_H


#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor_control.h"
#include "oled4p.h"
#include "flash.h"
#define R_G_1 GPIOB
#define R_P_1 GPIO_PIN_15
#define R_G_2 GPIOB
#define R_P_2 GPIO_PIN_14
#define R_G_3 GPIOB
#define R_P_3 GPIO_PIN_13
#define R_G_4 GPIOB
#define R_P_4 GPIO_PIN_12

#define C_G_1 GPIOA
#define C_P_1 GPIO_PIN_7
#define C_G_2 GPIOA
#define C_P_2 GPIO_PIN_6
#define C_G_3 GPIOA
#define C_P_3 GPIO_PIN_5
#define C_G_4 GPIOA
#define C_P_4 GPIO_PIN_4


void Set_R(uint8_t row);
uint8_t Read_C(void);
uint8_t password_compare(void);
uint8_t  password_input(void);
void keys_scan(void);
void keys_start(void);
extern volatile uint8_t row,column;
extern char key_chr;
extern uint8_t menu_enter,digital_enter;
extern char password[8];



#endif
