#ifndef __FLASH_H
#define __FLASH_H
 
#include "stm32f1xx_hal.h"
 #include "print_mode.h"
#include "motor_control.h"
#include "standby_mode.h"
#include "oled4p.h"
#include "keys_4x4.h"
#include "flash.h"

#define SN 			            0x08007800
#define PN 			            0x08007800
#define SOFTWARE_VERSION 		0x08007800

#define HARDWARE_VERSION		0x08007800
 
void WriteFlash(uint32_t Address,uint8_t *data,uint8_t page);
uint32_t ReadFlash(uint32_t Address);
extern uint8_t Save[20];

 
#endif 
