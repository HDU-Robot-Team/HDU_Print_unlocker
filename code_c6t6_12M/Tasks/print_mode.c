#include "print_mode.h"

uint8_t buf_rec[MAX_LEN];
volatile struct print_flag reply_buf;
uint8_t cmd_got=0;
uint16_t print_max_id=0x0000;
uint16_t print_id=0x0000;
uint16_t sum_AutoEnroll=0x003F;
uint16_t count_down;
uint32_t electric_quantity[10];
uint8_t  Compare_expected,Compare_now=40;
//uint8_t Print_ID_[4]; //指纹模块芯片地址
uint8_t Print_ID_[4] = {0xff,0xff,0xff,0xff}; //默认指纹模块芯片地址
//添加指纹指令
uint8_t CMD_AutoEnroll[17]={0XEF,0X01,0xFF,0xFF,0xFF,0xFF, 0X01, 0X00,0X08, 0X31, 0X00,0X00, 0X05, 0X00,0X00, /*SUM*/0X00,0X3F};
//识别指纹指令
uint8_t CMD_AutoIdentify[17]={0XEF,0X01,0xFF,0xFF,0xFF,0xFF, 0X01, 0X00,0X08, 0X32,0X03,0Xff,0Xff, 0X00,0X00, /*SUM*/0X02,0X3C};
//清除所有指纹指令
uint8_t CMD_ClearALL[17]={ 0xEF,0x01,0xFF,0xFF,0xFF,0xFF, 0X01, 0X00,0x03, 0x0D,0x00,0x11,0X00,0X00, 0X00,0X00,0x00};
//取消进程指令
uint8_t CMD_Cancel[17]={0XEF,0X01,0xFF,0xFF,0xFF,0xFF,  0X01, 0X00,0X03,0x30,0x00,0x34,0X00, 0X00,0X00,0x00,0x00};
//读取指纹数指令
uint8_t CMD_ValidTempleteNum[17]={0XEF,0X01,0xFF,0xFF,0xFF,0xFF, 0X01, 0X00,0X03,0x1D,0x00,0x21,0X00, 0X00,0X00,0x00,0x00};
//设置芯片地址指令
uint8_t CMD_SetChipAddr[17]={0XEF,0X01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x15,/*芯片*/0x00,0x00,0x00,0x00/*地址*/,0x00,0x1D,0x00};
void print_fun()
{
		int i;

			i=0; 
			key_chr='x';
			while(HAL_GPIO_ReadPin(sign_gpio,sign_pin)==GPIO_PIN_RESET )
			{
				i++;
				oled_waiting_touch();
				if(Unlocked==1)//*上锁
					oled_show_digital();
				oled_show_menu();
				HAL_Delay(100);
				if(key_chr!='x' && key_chr!='*' && key_chr!='#' && key_chr!='A' && key_chr!='B' && key_chr!='C' && key_chr!='D')
				{
					digital_enter=1;
					break;
				}
				else if(key_chr=='#')
				{
					menu_enter=1;
					break;
				}
				else if(key_chr=='*')
				{
					Save[16]=Save[17]=Save[18]=Save[19]=0x00;
					WriteFlash(SN,Save,4);
					oled_show_lock();
					HAL_Delay(1000);
					oled_standby_enter();
					HAL_Delay(2000);
					power_off();		
				}
				if(i>=10)
				{
					oled_count_down('0'+count_down);
					break;
				}
			}

			if(HAL_GPIO_ReadPin(sign_gpio,sign_pin)==GPIO_PIN_SET && menu_enter==0 && digital_enter==0)
			{
				oled_count_break();
				if(Unlocked==1)
				{
					unlock();
					oled_show_unlock();
					HAL_Delay(4500);
					lock();
					HAL_Delay(1000);
					oled_standby_enter();
					HAL_Delay(3000);
					power_off();		
				}
				else
				{
						oled_identifying();
						reply_buf.parameter[0]=0x00;
						HAL_Delay(10);

						if(Send_CMD(&print_uart,CMD_AutoIdentify,0x00)!=HAL_OK)
						{		
							Send_CMD(&print_uart,CMD_AutoIdentify,0x00);
						}
						i=0;
						
						while(cmd_got==0)
						{
							i++;
							HAL_Delay(10);
							if(i>200)
								break;
						}
						i=0;
						while(reply_buf.parameter[0]!=0x05)
						{
							i++;
							HAL_Delay(10);
							if(i>200)
								break;
						}
						if(i<=200 && reply_buf.cmd==0x00)
						{
							unlock();
							Save[16]=Save[17]=Save[18]=Save[19]=0x11;
							WriteFlash(SN,Save,4);
							oled_identify_right();
							HAL_Delay(400);
							oled_show_unlock();
							HAL_Delay(4000);
							lock();
							HAL_Delay(1000);
							oled_standby_enter();
							HAL_Delay(2000);
							power_off();
						}
						else if(i<=200)
						{
							OLED_Clean(); 
							if(reply_buf.cmd==0x09)//没有指纹
							{
								oled_identify_fault();
							}
							else if(reply_buf.cmd==0x17)//擦干净指纹识别区再次尝试
							{
								oled_print_dirty();
							}
							else if(reply_buf.cmd==0x24)//指纹库为空
							{
								oled_print_empty();
							}
							else if(reply_buf.cmd==0x26)//超时
							{
								oled_timeout();
							}
							HAL_Delay(500);
						}
					}
			}
}


HAL_StatusTypeDef Send_CMD(UART_HandleTypeDef* huart, uint8_t* CMD,uint16_t print_id)
{
	cmd_got=0;
	if(CMD==CMD_AutoEnroll)
	{
		CMD[11]=print_id;
		CMD[10]=print_id >> 8;
		CMD[15]=(sum_AutoEnroll+print_id)>>8;
		CMD[16]=(sum_AutoEnroll+print_id);
	}
	CMD[2]=Print_ID_[0];
	CMD[3]=Print_ID_[1];
	CMD[4]=Print_ID_[2];
	CMD[5]=Print_ID_[3];
	if(CMD==CMD_SetChipAddr)
	{
		Print_ID_[0]=CMD_SetChipAddr[10];
		Print_ID_[1]=CMD_SetChipAddr[11];
		Print_ID_[2]=CMD_SetChipAddr[12];
		Print_ID_[3]=CMD_SetChipAddr[13];
	}
	return HAL_UART_Transmit_DMA(huart,CMD,17);

}

void power_on()
{
	HAL_GPIO_WritePin(power_gpio,power_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(print_gpio,print_pin,GPIO_PIN_SET);
	
}	
void power_off()
{
	
	HAL_GPIO_WritePin(print_gpio,print_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(power_gpio,power_pin,GPIO_PIN_RESET);
}
void SET_ID(uint16_t id_1,uint16_t id_2,uint16_t id_3,uint16_t id_4)
{
	uint16_t SUM;
	CMD_SetChipAddr[10]=id_1;
	CMD_SetChipAddr[11]=id_2;
	CMD_SetChipAddr[12]=id_3;
	CMD_SetChipAddr[13]=id_4;
	SUM=((uint16_t)CMD_SetChipAddr[14]<<8)+CMD_SetChipAddr[15];
	SUM+=id_1+id_2+id_3+id_4;
	CMD_SetChipAddr[14]=SUM>>8;
	CMD_SetChipAddr[15]=SUM;
	Send_CMD(&print_uart,CMD_SetChipAddr,0);
}

void buf_treat()
{
	reply_buf.head = ((uint16_t)buf_rec[0]<<8)+buf_rec[1];
	reply_buf.address=((uint32_t)buf_rec[2]<<24)+((uint32_t)buf_rec[3]<<16)+((uint32_t)buf_rec[4]<<8)+buf_rec[5];
	reply_buf.pkg_id= buf_rec[6];
	reply_buf.pkg_size=((uint16_t)buf_rec[7]<<8)+buf_rec[8];
	reply_buf.cmd = buf_rec[9];
	reply_buf.parameter[0]=buf_rec[10];
	if(reply_buf.pkg_size==0x0005)
	{
		reply_buf.parameter[1]=buf_rec[11];
		reply_buf.sum=((uint16_t)buf_rec[12]<<8)+buf_rec[13];
	}
	else if(reply_buf.pkg_size==0x0008)
	{
		reply_buf.parameter[1]=0x00;
		reply_buf.print_id=((uint16_t)buf_rec[11]<<8)+buf_rec[12];
		reply_buf.score=((uint16_t)buf_rec[13]<<8)+buf_rec[14];
		reply_buf.sum=((uint16_t)buf_rec[15]<<8)+buf_rec[16];
	}
	else if(reply_buf.pkg_size==0x0003||reply_buf.pkg_size==0x0007)
	{
		reply_buf.parameter[0]=reply_buf.parameter[1]=0x00;
		reply_buf.sum=((uint16_t)buf_rec[10]<<8)+buf_rec[11];
	}
	cmd_got=1;
}



uint16_t dma_current_data_counter(DMA_Channel_TypeDef *dma_stream)
{
  /* Return the number of remaining data units for DMAy Streamx */
  return ((uint16_t)(dma_stream->CNDTR));
}


void uart_receive_handler(UART_HandleTypeDef *huart)
{
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
			__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
	{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
	/* handle received data in idle interrupt */
		if (huart == &print_uart)
		{
			/* clear DMA transfer complete flag */
			__HAL_DMA_DISABLE(huart->hdmarx);
			uint32_t temp;
			temp=MAX_LEN - huart->hdmarx->Instance->CNDTR;
			
			if (temp == 12 || temp == 14 || temp == 17)
			{
				if(buf_rec[0]==0xEF&&buf_rec[1]==0x01&&buf_rec[2]==Print_ID_[0]&&buf_rec[3]==Print_ID_[1]&&buf_rec[4]==Print_ID_[2]&&buf_rec[5]==Print_ID_[3])
					buf_treat();
				memset(buf_rec,0,sizeof(buf_rec));
			}
			
			__HAL_DMA_SET_COUNTER(huart->hdmarx, MAX_LEN);
			__HAL_DMA_ENABLE(huart->hdmarx);

		}
	}
}
void print_mode_init(void)
{
//	uart_receive_dma_no_it(&print_uart, buf_rec, MAX_LEN);

	__HAL_UART_CLEAR_IDLEFLAG(&print_uart);
	__HAL_UART_ENABLE_IT(&print_uart, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&print_uart, buf_rec,MAX_LEN);
}

