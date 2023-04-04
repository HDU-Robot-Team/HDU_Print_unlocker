#include "keys_4x4.h"



volatile uint8_t row=0,column=0;
uint8_t menu_enter=0,digital_enter=0;
char password[8]={"FFFFFFFF"};
char circle[9]={0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,'\0'};
char word[8],word2[8];
char button[5][5]={
	'x','x','x','x','x',
	'x','1','2','3','A',
	'x','4','5','6','B',
	'x','7','8','9','C',
	'x','*','0','#','D'
};
char key_chr='x';
void Set_R(uint8_t row)
{
	switch(row)
	{
		case 1:
			HAL_GPIO_WritePin(R_G_1,R_P_1,GPIO_PIN_SET);
			HAL_GPIO_WritePin(R_G_2,R_P_2,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_3,R_P_3,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_4,R_P_4,GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(R_G_1,R_P_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_2,R_P_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(R_G_3,R_P_3,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_4,R_P_4,GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(R_G_1,R_P_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_2,R_P_2,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_3,R_P_3,GPIO_PIN_SET);
			HAL_GPIO_WritePin(R_G_4,R_P_4,GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(R_G_1,R_P_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_2,R_P_2,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_3,R_P_3,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R_G_4,R_P_4,GPIO_PIN_SET);
			break;
	}
	
	
}

uint8_t Read_C(void)
{
	uint8_t C=0;

	if(HAL_GPIO_ReadPin(C_G_1,C_P_1)==GPIO_PIN_SET)
		C=1;
	else if(HAL_GPIO_ReadPin(C_G_2,C_P_2)==GPIO_PIN_SET)
		C=2;
	else if(HAL_GPIO_ReadPin(C_G_3,C_P_3)==GPIO_PIN_SET)
		C=3;
	else if(HAL_GPIO_ReadPin(C_G_4,C_P_4)==GPIO_PIN_SET)
		C=4;
	return C;
}

void keys_scan(void)
{
		HAL_GPIO_WritePin(R_G_1,R_P_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(R_G_2,R_P_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(R_G_3,R_P_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(R_G_4,R_P_4,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(C_G_1,C_P_1)+HAL_GPIO_ReadPin(C_G_2,C_P_2)+HAL_GPIO_ReadPin(C_G_3,C_P_3)+HAL_GPIO_ReadPin(C_G_4,C_P_4)!=0)
	{
		for(row=1;row<=4;row++)
		{
			Set_R(row);
			if(Read_C()!=0)
			{
				column=Read_C();
				break;
			}
		}
		key_chr=button[row][column];
	}
	else
	{
		key_chr='x';
	}
}
uint8_t  password_input(void)
{
		int i;

		OLED_ShowString(0,2,circle);
		oled_show_delete();
		oled_show_back();
		word[0]=word[1]=word[2]=word[3]=word[4]=word[5]=word[6]=word[7]=0x7f;
		for(i=0;i<8;i++)
		{
			int j=0;
			uint8_t keyed;
			while((key_chr<'0'||key_chr>'9')&&key_chr!='C'&&key_chr!='D'&&key_chr!='#')
			{
				j++;
				HAL_Delay(100);
				if(i>0&&j>10)
					OLED_ShowChar(i-1,2,'*');
				if(j>150)
				{
					OLED_Clean();
					return 0;
				}
			}
			keyed=key_chr;
			if(key_chr=='D')
			{
				if(i>0)
				{
					word[i-1]=0x7f;
					OLED_ShowChar(i-1,2,word[i-1]);
					i-=2;
				}
			}
			else if(key_chr=='C')
			{
				OLED_Clean();
				return 0;
			}
			else 
			{
				if(key_chr == '#')
					word[i]='F';
				else 
					word[i]=key_chr;
				if(i>0)
					OLED_ShowChar(i-1,2,'*');
				OLED_ShowChar(i,2,word[i]);
			}
			j=0;
			while(key_chr==keyed)
			{
				j++;
				HAL_Delay(20);
				if(j>50)
					break;
			}
		}
		return 1;
		
}
uint8_t password_compare(void)
{
	int i;
	for(i=0;i<8;i++)
		{
			if(word[i]!=password[i])
				break;
		}
		if(i==8)
		{
			oled_show_passward_correct();
			return 1;
		}
		else
		{
			oled_show_passward_mistake();
			HAL_Delay(500);
			return 0;
		}
	
}
void addprint(void)
{
							int 	i=0;
							while(HAL_GPIO_ReadPin(sign_gpio,sign_pin)==GPIO_PIN_RESET)
							{
								i++;
								oled_waiting_touch();
								oled_count_down('9'+1-i);
								HAL_Delay(1000);
								if(i>=10)
								{
									break;
								}
							}
							if(HAL_GPIO_ReadPin(sign_gpio,sign_pin)==GPIO_PIN_SET)
							{

								Send_CMD(&print_uart,CMD_ValidTempleteNum,0x00);
								HAL_Delay(500);
								if(reply_buf.pkg_size==0x0005&&reply_buf.cmd==0x00)
								{
									print_max_id=((uint16_t)reply_buf.parameter[0]<<8)+reply_buf.parameter[1];
									print_max_id++;	
									Send_CMD(&print_uart,CMD_Cancel,0x00);
									HAL_Delay(1000);
									if(reply_buf.pkg_size!=0x0003)
									{
										Send_CMD(&print_uart,CMD_Cancel,0x00);
										HAL_Delay(1000);
									}
									if(Send_CMD(&print_uart,CMD_AutoEnroll,print_max_id)!=HAL_OK)
									{
										Send_CMD(&print_uart,CMD_AutoEnroll,print_max_id);
									}
									HAL_Delay(500);
									i=0;
									while(reply_buf.cmd!=0x26&& reply_buf.cmd!=0x22 &&(reply_buf.pkg_size!=0x0005 ||reply_buf.parameter[0]!=0x06))
									{
										i++;
										HAL_Delay(100);
										if(reply_buf.parameter[0]==0x02)
										{
											oled_waiting_untouch();

										}
						//				else if(reply_buf.parameter[0]==0x01)
						//				{
						//					if(reply_buf.cmd==0x00)
						//					{
						//						HAL_Delay(100);
						//					}
						//					else if(reply_buf.cmd==0x01)
						//					{
						//						HAL_Delay(100);
						//					}
						//				}
										else if(reply_buf.parameter[0]==0x03)
										{
											oled_waiting_touch();

										}
										else if(reply_buf.parameter[0]==0x26||i>600)
										{
											oled_timeout();
											break; 
										}
									}
									HAL_Delay(300);
									if(reply_buf.parameter[0]==0x06&&reply_buf.parameter[1]==0xF2)
									{
										if(reply_buf.cmd==0x00)
										{
											oled_addprint_successfully();
											HAL_Delay(500);
											oled_standby_enter();
											HAL_Delay(500);
											power_off();
										}
										else if(reply_buf.cmd==0x01)
										{
											oled_addprint_failed();
											HAL_Delay(500);
											print_max_id--;
										}
										else if(reply_buf.cmd==0x1f)
										{
											oled_print_full();
											HAL_Delay(500);
											print_max_id--;
										}
									}
									Send_CMD(&print_uart,CMD_ValidTempleteNum,0x00);
									HAL_Delay(500);
									if(reply_buf.pkg_size==0x0005&&reply_buf.cmd==0x00)
										print_max_id=((uint16_t)reply_buf.parameter[0]<<8)+reply_buf.parameter[1];
								}
							}
							else
							{
								oled_standby_enter();
								HAL_Delay(500);
								power_off();
							}
}
void clearprint(void)
{
						OLED_ShowCHinese(40,3,26);
						OLED_ShowCHinese(56,3,29);
						OLED_ShowCHinese(72,3,67);
						Send_CMD(&print_uart,CMD_ClearALL,0);
						HAL_Delay(500);
						Send_CMD(&print_uart,CMD_ValidTempleteNum,0x00);
						HAL_Delay(500);
						if(reply_buf.pkg_size==0x0005&&reply_buf.cmd==0x00)
						{
							print_max_id=((uint16_t)reply_buf.parameter[0]<<8)+reply_buf.parameter[1];
							if(print_max_id==0)
							{
								oled_print_clear();
							}
							else
							{
								OLED_ShowCHinese(48,3,10);
								OLED_ShowCHinese(64,3,11);
							}
						}
						HAL_Delay(2000);
}
void inquireprint(void)
{
						Send_CMD(&print_uart,CMD_ValidTempleteNum,0x00);
						HAL_Delay(100);
						if(reply_buf.pkg_size==0x0005&&reply_buf.cmd==0x00)
						{
							print_max_id=((uint16_t)reply_buf.parameter[0]<<8)+reply_buf.parameter[1];
							OLED_ShowCHinese(8,2,5);
							OLED_ShowCHinese(24,2,33);
							OLED_ShowCHinese(40,2,66);
							OLED_ShowCHinese(56,2,42);
							OLED_ShowNum(9,1,print_max_id,3);
						}
						else
						{
							OLED_ShowCHinese(32,3,64);
							OLED_ShowCHinese(48,3,65);
							OLED_ShowCHinese(64,3,10);
							OLED_ShowCHinese(80,3,11);
						}
						HAL_Delay(2000);
}
uint8_t  resetpassword(void)
{
						oled_show_passwordinput();
						OLED_ShowChar(10,1,'1');
						if(password_input()!=1)
						{
							return 0;
						}
						int j;
						for( j=0;j<8;j++)
						{
							word2[j]=word[j];
						}
						oled_show_passwordinput();
						OLED_ShowChar(10,1,'2');
						if(password_input()!=1)
						{
							return 0;
						}
						for( j=0;j<8;j++)
						{
							if(word[j]!=word2[j])
								break;
						}
						if(j==8)
						{
							for(int i=0;i<8;i++)
							{
								Save[i+4]=password[i]=word[i];
							}
								uint8_t ID_[4];
								for(int i=0;i<4;i++)
								{
									if(password[i*2]=='F'&& password[i*2+1]=='F')
									{
										ID_[i]=0xFF;
									}
									else if(password[i*2]=='F')
										ID_[i]=0xF0+(password[i*2+1]-'0');
									else if(password[i*2+1]=='F')
										ID_[i]=(password[i*2]-'0')*16+0x0F;
									else
										ID_[i]=(password[i*2]-'0')*16+(password[i*2+1]-'0');
								}
							SET_ID(ID_[0],ID_[1],ID_[2],ID_[3]);
							HAL_Delay(400);
							reply_buf.pkg_size=0; 
							Send_CMD(&print_uart,CMD_ValidTempleteNum,0x00);
							uint8_t sfds=0;
							HAL_Delay(10);
							while(cmd_got==0)
							{		
								if(sfds>10)
									break;
								sfds++;
								HAL_Delay(100);
							}
							if(reply_buf.pkg_size==0x0005&&reply_buf.cmd==0x00)
							{
								WriteFlash(SN,Save,4);
								OLED_Clean();
								OLED_ShowCHinese(32,3,62);
								OLED_ShowCHinese(48,3,63);
								OLED_ShowCHinese(64,3,8);
								OLED_ShowCHinese(80,3,9);
							}
							else
							{
								OLED_Clean();
								OLED_ShowCHinese(32,3,10);
								OLED_ShowCHinese(48,3,11);
								OLED_ShowCHinese(64,3,59);
								OLED_ShowCHinese(80,3,60);
							}
							return 1;
						}
						else
						{
							OLED_Clean();
							OLED_ShowCHinese(32,3,45);
							OLED_ShowCHinese(48,3,46);
							OLED_ShowCHinese(64,3,59);
							OLED_ShowCHinese(80,3,60);
						}
						HAL_Delay(2000);
						return 0;
}
uint8_t angle_calibration(void)//放在while循环中
{
	int i;
	OLED_Clean();
	OLED_ShowChar(15,0,'A');
	OLED_ShowChar(15,2,'B');
	OLED_ShowChar(15,3,'C');
	OLED_ShowCHinese(88,6,55);
	OLED_ShowCHinese(104,6,56);
	OLED_ShowCHinese(48,0,17);
	OLED_ShowCHinese(64,0,20);
	OLED_ShowCHinese(48,4,18);
	OLED_ShowCHinese(64,4,20);
	i=0;
	HAL_Delay(1000);
	while(key_chr!='A' && key_chr!='B' && key_chr!='C')
	{
		i++;
		HAL_Delay(50);
		if(i>295)
		{
			return 0;
		}
	}
	if(key_chr=='A'||key_chr=='B')
	{
		uint16_t *compare;
		unlock();
		if(key_chr=='A')
		{
			Compare_expected = unlock_compare;
			compare=&unlock_compare;
		}
		else 
		{
			Compare_expected = lock_compare;
			compare=&lock_compare;
		}
		OLED_Clean();
		OLED_ShowChar(15,0,'A');
		OLED_ShowChar(15,2,'B');
		OLED_ShowChar(15,3,'C');
		OLED_ShowCHinese(88,6,57);
		OLED_ShowCHinese(104,6,58);
		OLED_ShowChar(7,0,'+');
		OLED_ShowChar(7,2,'-');
		OLED_ShowNum(5,1,Compare_expected,3);
		i=0;
		HAL_Delay(1000);
		while(key_chr!='C')
		{
			i++;
			HAL_Delay(20);
			if(key_chr=='A'&&Compare_expected<180)
			{
				i=0;
				Compare_expected++;
				OLED_ShowNum(5,1,Compare_expected,3);
				int t=0;
				while(key_chr=='A')
				{
					t++;
					HAL_Delay(10);
					if(t>70)
						break;
				}
				while(key_chr=='A')
				{
					HAL_Delay(100);
					if(Compare_expected<180)
					{
						Compare_expected++;
						OLED_ShowNum(5,1,Compare_expected,3);
					}
				}
			}
			else if(key_chr=='B'&&Compare_expected>36)
			{
				i=0;
				Compare_expected--;
				OLED_ShowNum(5,1,Compare_expected,3);
				int t=0;
				while(key_chr=='B')
				{
					t++;
					HAL_Delay(10);
					if(t>70)
						break;
				}
				while(key_chr=='B')
				{
					HAL_Delay(100);
					if(Compare_expected>36)
					{
						Compare_expected--;
						OLED_ShowNum(5,1,Compare_expected,3);
					}
				}
			}
			if(i>715)
			{
				lock();
				return 0;
			}
		}
		if(key_chr=='C')
		{
			if(*compare!=Compare_expected)
			{
				*compare=Compare_expected;
				Save[12]=lock_compare>>8;
				Save[13]=(uint8_t)lock_compare;
				Save[14]=unlock_compare>>8;
				Save[15]=(uint8_t)unlock_compare;
				WriteFlash(SN,Save,3);
			}
			lock();
			return 1;
		}
	}
	else if(key_chr=='C')
	{
		return 0;
	}
}



void keys_start(void)
{
	if(digital_enter)
	{		
		oled_show_passwordinput();
		if(password_input()==1 && password_compare()==1)
		{
			unlock();
			Save[16]=Save[17]=Save[18]=Save[19]=0x11;
			WriteFlash(SN,Save,4);
			oled_show_unlock();
			HAL_Delay(4000);
	  	lock();
			HAL_Delay(1000);
			oled_standby_enter();
			HAL_Delay(2000);
			power_off();
		}
		count_down=4;
		digital_enter=0;
	}
	if(menu_enter)
	{
			uint8_t choose=0,options[5]={0,1,2,3,4},*the_option;
				the_option =options ;
			oled_show_options(options - the_option);
			oled_show_choose(choose);
			oled_show_confirm();
			oled_show_back();
			int i=0;
			while(key_chr!='B'&&key_chr!='C')
			{
				i++;
				HAL_Delay(20);
				if(i>500)
					break;
				if(key_chr=='A')
				{
					i=0;
					if(choose>0)
						oled_show_choose(--choose);
					else if(the_option > options )
					{
						the_option--;
						oled_show_options(options - the_option);
						oled_show_choose(choose);
						oled_show_confirm();
						oled_show_back();
					}
					while(key_chr=='A')
					{
						HAL_Delay(10);
					}
				}
				else if(key_chr=='D')
				{
					i=0;
					if( choose<3 && (choose<sizeof(options)-1) )
						oled_show_choose(++choose);
					else if(the_option < options+sizeof(options)-4 )
					{
						the_option++;
						oled_show_options(options  - the_option);
						oled_show_choose(choose);
						oled_show_confirm();
						oled_show_back();
					}
					while(key_chr=='D')
					{
						HAL_Delay(10);
					}
				}
			}
			if(key_chr=='C')
			{
				OLED_Clean();
			}
			else if(key_chr=='B' &&( oled_show_passwordinput(),password_input()==1 )&& password_compare()==1)
			{
				OLED_Clean();
				switch(*(the_option+choose))
				{
					case 0:
					{
						addprint();
						break;
					}
					case 1:
					{
						inquireprint();
						break;
					}
					case 2:
					{
						clearprint();
						break;
					}
					case 3:
					{
						resetpassword();
						break;
					}
					case 4:
					{
						while(angle_calibration()==1)
						{
							HAL_Delay(10);
						}
						break;
					}
				}
				
				
			}
			OLED_Clean();
		count_down=4;
		menu_enter=0;
	}
	
}


