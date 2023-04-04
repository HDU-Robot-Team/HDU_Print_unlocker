#ifndef _OLED4P_
#define _OLED4P_

#include "stm32f1xx_hal.h"

#define Uchar unsigned char
#define Uint  unsigned int
#define Ulong unsigned long
#define uchar unsigned char
#define uint  unsigned int


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Pin Definition
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#define OLED_PORT GPIOB;
//#define OLED_SCL_PIN GPIO_Pin_13
//#define OLED_SDA_PIN GPIO_Pin_14
#define setSCL HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET)
#define resetSCL HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET)
#define setSDA HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET)
#define resetSDA HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET)

void OLED_outlist(void);
void OLED_inqueue(uint8_t x,uint8_t y,char *s);
void OLED_inlist(uint8_t channel,uint8_t x,uint8_t y,char *s);
void OLED_Init(void);
void OLED_WriteData(Uchar Data);
void OLED_WriteCommand(Uchar Data);
void OLED_SetPos(Uchar x,Uchar page);
void OLED_Clean(void);
void OLED_ShowChar(Uchar x,Uchar y,char chr);
void OLED_ShowString(Uchar x,Uchar y,char *s);
void OLED_ShowNum(Uchar x,Uchar y,int n,Uchar len);
void OLED_ShowFloat(uint8_t x,uint8_t y,float num,uint8_t n);
void OLED_Show6x8String(int x,int y,char* s);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);

void oled_waiting_touch(void);
void oled_identifying(void);
void oled_identify_right(void);
void oled_identify_fault(void);
void oled_waiting_untouch(void);
void oled_standby_enter(void);
void oled_show_unlock(void);
void oled_addprint_successfully(void);
void oled_addprint_failed(void);
void oled_print_clear(void);
void oled_timeout(void);
void oled_print_dirty(void);
void oled_print_empty(void);
void oled_print_full(void);
void oled_count_break(void);
void oled_count_down(char ch);
void OLED_ShowBattery(void);
void oled_show_menu(void);
void oled_show_digital(void);
void oled_show_delete(void);
void oled_show_back(void);
void oled_show_passward_mistake(void);
void oled_show_passward_correct(void);
void oled_show_passward_change(uint8_t y);
void oled_show_confirm(void);
void oled_show_print_num(uint8_t y);
void oled_show_choose(uint8_t y);
void oled_show_options(uint8_t t);
void oled_show_passwordinput(void);
void oled_show_lock(void);



#endif

