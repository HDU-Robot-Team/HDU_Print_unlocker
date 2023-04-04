#include "flash.h"
 
uint8_t Save[20]={0x66,0x66,0x99,0x99,  0x46,0x46,0x46,0x46,  0x46,0x46,0x46,0x46, 0,0,0,0, 0,0,0,0};

//FLASH写入数据
void WriteFlash(uint32_t Address,uint8_t *data,uint8_t pages)//4个字节一页
{
    uint16_t i = 0;
    uint32_t Data;
		uint32_t startAddress;
		uint32_t endAddress;
    startAddress = SN;
    endAddress = SN+(pages+1)*4;
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Address;
    f.NbPages = 1;
    uint32_t PageError = 0;
    HAL_FLASHEx_Erase(&f, &PageError);
	while(startAddress <= endAddress)
	{
		Data=((uint32_t)*data<<24)+((uint32_t)*(data+1)<<16)+((uint32_t)*(data+2)<<8)+(uint32_t)*(data+3);
		data+=4;
		if(HAL_FLASH_Program(TYPEPROGRAM_WORD, startAddress, Data) == HAL_OK)
		{
			startAddress +=4;
		}
	}
    //4、锁住FLASH
	HAL_FLASH_Lock();
}
 
//FLASH读取数据
uint32_t ReadFlash(uint32_t Address)
{
	uint32_t temp = *(__IO uint32_t*)(Address);
	return temp;
}






