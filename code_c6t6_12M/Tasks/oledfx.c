#include "oled4p.h"
#include "main.h"
#include "gpio.h"
#include "print_mode.h"
const unsigned char battery[][32]=	
{
{0xC0,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xF0},
{0x03,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F},//0% 0
{0xC0,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xF0,0xF0,0xF0},
{0x03,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x0F,0x0F},//15% 1
{0xC0,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xF0,0xF0,0xF0},
{0x03,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x0F,0x0F},//30% 2
{0xC0,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},
{0x03,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},//45% 3
{0xC0,0xF0,0x10,0x10,0x10,0x10,0x10,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},
{0x03,0x0F,0x08,0x08,0x08,0x08,0x08,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},//60% 4
{0xC0,0xF0,0x10,0x10,0x10,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},
{0x03,0x0F,0x08,0x08,0x08,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},//75% 5
{0xC0,0xF0,0x10,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},
{0x03,0x0F,0x08,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},//90% 6
{0xC0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},
{0x03,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},//100% 7
};
void OLED_ShowBattery(void)
{      			    
	uint8_t no,t,adder=0;
	uint32_t x=0;
	for(int i=0;i<10;i++)
		x+=electric_quantity[i];
	x/=10;
	if((float)x/4096 > 0.90)//100
		no=7;
	else if((float)x/4096 > 0.89)//90
		no=6;
	else if((float)x/4096 > 0.88)//75
		no=5;
	else if((float)x/4096 > 0.87)//60
		no=4;
	else if((float)x/4096 > 0.85)//45
		no=3;
	else if((float)x/4096 > 0.83)//30
		no=2;
	else if((float)x/4096 > 0.81)//15
		no=1;
	else 
		no=0;
	OLED_SetPos(111,0);//显示文字的坐标	
    for(t=0;t<16;t++)
		{
				OLED_WriteData(battery[2*no][t]);
				adder+=1;
    }	
		OLED_SetPos(111,1);	
    for(t=0;t<16;t++)
		{	
			OLED_WriteData(battery[2*no+1][t]);
			adder+=1;
    }					
}
void oled_waiting_touch(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(24,3,14);
	OLED_ShowCHinese(40,3,0);
	OLED_ShowCHinese(56,3,1);
	OLED_ShowCHinese(72,3,4);
	OLED_ShowCHinese(88,3,5);
	OLED_ShowCHinese(104,3,23);
	OLED_ShowCHinese(120,3,23);
}
void oled_identifying(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,23);
	OLED_ShowCHinese(40,3,6);
	OLED_ShowCHinese(56,3,7);
	OLED_ShowCHinese(72,3,67);
	OLED_ShowCHinese(88,3,23);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_identify_right(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,6);
	OLED_ShowCHinese(48,3,7);
	OLED_ShowCHinese(64,3,8);
	OLED_ShowCHinese(80,3,9);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_identify_fault(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(24,3,23);
	OLED_ShowCHinese(32,3,6);
	OLED_ShowCHinese(48,3,7);
	OLED_ShowCHinese(64,3,10);
	OLED_ShowCHinese(80,3,11);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_waiting_untouch(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(24,3,14);
	OLED_ShowCHinese(40,3,2);
	OLED_ShowCHinese(56,3,3);
	OLED_ShowCHinese(72,3,4);
	OLED_ShowCHinese(88,3,5);
	OLED_ShowCHinese(104,3,23);
	OLED_ShowCHinese(120,3,23);
}
void oled_standby_enter(void)
{
	OLED_Clean();
	OLED_ShowCHinese(32,3,24);
	OLED_ShowCHinese(48,3,25);
	OLED_ShowCHinese(64,3,12);
	OLED_ShowCHinese(80,3,13);
}
void oled_show_unlock(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,23);
	OLED_ShowCHinese(48,3,17);
	OLED_ShowCHinese(64,3,20);
	OLED_ShowCHinese(80,3,23);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}


void oled_addprint_successfully(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,21);
	OLED_ShowCHinese(48,3,22);
	OLED_ShowCHinese(64,3,8);
	OLED_ShowCHinese(80,3,9);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_addprint_failed(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,21);
	OLED_ShowCHinese(48,3,22);
	OLED_ShowCHinese(64,3,10);
	OLED_ShowCHinese(80,3,11);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_print_clear(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,26);
	OLED_ShowCHinese(48,3,27);
	OLED_ShowCHinese(64,3,8);
	OLED_ShowCHinese(80,3,9);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_timeout(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,23);
	OLED_ShowCHinese(48,3,39);
	OLED_ShowCHinese(64,3,40);
	OLED_ShowCHinese(80,3,23);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_print_dirty(void)
{
	OLED_ShowCHinese(0,3,30);
	OLED_ShowCHinese(16,3,31);
	OLED_ShowCHinese(32,3,32);
	OLED_ShowCHinese(48,3,5);
	OLED_ShowCHinese(64,3,33);
	OLED_ShowCHinese(80,3,6);
	OLED_ShowCHinese(96,3,7);
	OLED_ShowCHinese(112,3,34);
	
	OLED_ShowCHinese(0,5,23);
	OLED_ShowCHinese(16,5,23);
	OLED_ShowCHinese(32,5,35);
	OLED_ShowCHinese(48,5,36);
	OLED_ShowCHinese(64,5,37);
	OLED_ShowCHinese(80,5,38);
	OLED_ShowCHinese(96,5,23);
	OLED_ShowCHinese(112,5,23);
	HAL_Delay(500);
	OLED_Clean();
}
void oled_print_empty(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(24,3,5);
	OLED_ShowCHinese(40,3,33);
	OLED_ShowCHinese(56,3,41);
	OLED_ShowCHinese(72,3,42);
	OLED_ShowCHinese(88,3,43);
	OLED_ShowCHinese(104,3,23);
	OLED_ShowCHinese(120,3,23);
}
void oled_print_full(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(24,3,5);
	OLED_ShowCHinese(40,3,33);
	OLED_ShowCHinese(56,3,41);
	OLED_ShowCHinese(72,3,15);
	OLED_ShowCHinese(88,3,44);
	OLED_ShowCHinese(104,3,23);
	OLED_ShowCHinese(120,3,23);
}
void oled_count_down(char ch)
{
	OLED_ShowChar(7,0,ch);
	OLED_ShowChar(8,0,'s');
}
void oled_count_break(void)
{
	OLED_Clean();
	count_down=5;
}
void oled_show_menu(void)
{
	OLED_ShowChar(15,2,'#');
	OLED_ShowCHinese(96,6,51);
	OLED_ShowCHinese(112,6,52);
}
void oled_show_digital(void)
{
	OLED_ShowChar(0,2,'*');
	OLED_ShowCHinese(0,6,72);
	OLED_ShowCHinese(16,6,73);
}
void oled_show_lock(void)
{
	OLED_ShowCHinese(0,3,23);
	OLED_ShowCHinese(16,3,23);
	OLED_ShowCHinese(32,3,23);
	OLED_ShowCHinese(40,3,15);
	OLED_ShowCHinese(56,3,72);
	OLED_ShowCHinese(72,3,73);
	OLED_ShowCHinese(88,3,23);
	OLED_ShowCHinese(96,3,23);
	OLED_ShowCHinese(112,3,23);
}
void oled_show_delete(void)
{
	OLED_ShowChar(13,3,'D');
	OLED_ShowCHinese(112,4,53);
	OLED_ShowCHinese(112,6,54);
}
void oled_show_back(void)
{
	OLED_ShowChar(13,0,'C');
	OLED_ShowCHinese(112,0,55);
	OLED_ShowCHinese(112,2,56);
}
void oled_show_passward_mistake(void)
{
	OLED_Clean();
	OLED_ShowCHinese(32,3,49);
	OLED_ShowCHinese(48,3,50);
	OLED_ShowCHinese(64,3,59);
	OLED_ShowCHinese(80,3,60);
}
void oled_show_passward_correct(void)
{
	OLED_Clean();
	OLED_ShowCHinese(32,3,49);
	OLED_ShowCHinese(48,3,50);
	OLED_ShowCHinese(64,3,61);
	OLED_ShowCHinese(80,3,57);
}
void oled_show_passward_change(uint8_t y)
{
	if(y>=0&&y<7){
	OLED_ShowCHinese(8,y,62);
	OLED_ShowCHinese(24,y,63);
	OLED_ShowCHinese(40,y,49);
	OLED_ShowCHinese(56,y,50);
	}
}
void oled_show_passwordinput(void)
{
	OLED_Clean();
	OLED_ShowCHinese(0,2,14);
	OLED_ShowCHinese(16,2,45);
	OLED_ShowCHinese(32,2,46);
	OLED_ShowCHinese(48,2,49);
	OLED_ShowCHinese(64,2,50);
}
void oled_show_confirm(void)
{
	OLED_ShowChar(13,3,'B');
	OLED_ShowCHinese(112,4,57);
	OLED_ShowCHinese(112,6,58);
}
void oled_show_print_num(uint8_t y)
{
	if(y>=0&&y<7){
	OLED_ShowCHinese(8,y,64);
	OLED_ShowCHinese(24,y,65);
	OLED_ShowCHinese(40,y,5);
	OLED_ShowCHinese(56,y,33);
	OLED_ShowCHinese(72,y,66);
	}
}
void oled_show_print_clear(uint8_t y)
{
	if(y>=0&&y<7){
	OLED_ShowCHinese(8,y,26);
	OLED_ShowCHinese(24,y,27);
	OLED_ShowCHinese(40,y,5);
	OLED_ShowCHinese(56,y,33);
	}
}
void oled_show_addprint(uint8_t y)
{
	if(y>=0&&y<7){
	OLED_ShowCHinese(8,y,21);
	OLED_ShowCHinese(24,y,22);
	OLED_ShowCHinese(40,y,5);
	OLED_ShowCHinese(56,y,33);
	}
}
void oled_show_choose(uint8_t y)
{
	OLED_ShowChar(0,0,' ');
	OLED_ShowChar(0,1,' ');
	OLED_ShowChar(0,2,' ');
	OLED_ShowChar(0,3,' ');
	OLED_ShowChar(0,y,0x80);
}
void oled_show_angle(uint8_t y)
{
	if(y>=0&&y<7){
	OLED_ShowCHinese(8,y,68);
	OLED_ShowCHinese(24,y,69);
	OLED_ShowCHinese(40,y,17);
	OLED_ShowCHinese(56,y,20);
	OLED_ShowCHinese(72,y,70);
	OLED_ShowCHinese(88,y,71);
	}
}
void oled_show_options(uint8_t t)
{
	OLED_Clean();
	oled_show_addprint(t*2);
	oled_show_print_num((t+1)*2);
	oled_show_print_clear((t+2)*2);
	oled_show_passward_change((t+3)*2);
	oled_show_angle((t+4)*2);
}




