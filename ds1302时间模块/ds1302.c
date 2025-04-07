
#include "ds1302.h"

#include "delay.h"

struct TIMEData TimeData;
struct TIMERAM TimeRAM;

uint8_t read_time[7];

void ds1032_DATAOUT_init(void) // ����˫�� I/O Ϊ���̬
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   = GPIO_PIN_14;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
}

void ds1032_DATAINPUT_init(void) // ����˫�� I/O Ϊ����̬
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   = GPIO_PIN_14;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void ds1302_write_onebyte(uint8_t data)//��DS1302����һ�ֽ�����
{
	uint8_t count = 0;
	SCLK_L;
	ds1032_DATAOUT_init();
	for(count=0;count<8;count++)
	{
		SCLK_L;
		if (data & 0x01)
		{
			DATA_H;
		}
		else
		{
			DATA_L;
		} //��׼���������ٷ���
		SCLK_H; //����ʱ���ߣ���������
		data >>= 1;
	}
}
 
void ds1302_wirte_rig(uint8_t address,uint8_t data)//��ָ���Ĵ�����ַ��������
{
	uint8_t temp1=address;
	uint8_t temp2=data;
	CE_L;
	SCLK_L;
	delay_us(1);
	CE_H;
	delay_us(3);
	ds1302_write_onebyte(temp1);
	ds1302_write_onebyte(temp2);
	CE_L;
	SCLK_L;
	delay_us(3);
}
 
uint8_t ds1302_read_rig(uint8_t address)//��ָ����ַ��ȡһ�ֽ�����
{
	uint8_t temp3=address;
	uint8_t count=0;
	uint8_t return_data=0x00;
	CE_L;
	SCLK_L;
	delay_us(3);
	CE_H;
	delay_us(3);
	ds1302_write_onebyte(temp3);
	ds1032_DATAINPUT_init();//����I/O��Ϊ����
	delay_us(3);
	for(count=0;count<8;count++)
	{
		delay_us(3);//ʹ��ƽ����һ��ʱ��
		return_data>>=1;
		SCLK_H;
		delay_us(5);//ʹ�ߵ�ƽ����һ��ʱ��
		SCLK_L;
		delay_us(30);//��ʱ14us����ȥ��ȡ��ѹ������׼ȷ
		if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14))
		{
			return_data = return_data | 0x80;
		}
	}
	delay_us(2);
	CE_L;
	DATA_L;
	return return_data;
}

void ds1032_read_time(void)
{
	read_time[0]=ds1302_read_rig(0x81);//����
	read_time[1]=ds1302_read_rig(0x83);//����
	read_time[2]=ds1302_read_rig(0x85);//��ʱ
	read_time[3]=ds1302_read_rig(0x87);//����
	read_time[4]=ds1302_read_rig(0x89);//����
	read_time[5]=ds1302_read_rig(0x8B);//������
	read_time[6]=ds1302_read_rig(0x8D);//����
}
 
void ds1032_read_realTime(void)
{
	ds1032_read_time();  //BCD��ת��Ϊ10����
	TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
	TimeData.minute=((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
	TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
	TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
	TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
	TimeData.week=read_time[5];
	TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;	
}
void ds1302_write_realTime(struct TIMEData time_data)
{
	ds1302_wirte_rig(0x8e,0x00);//�ر�д����
	ds1302_wirte_rig(0x82,time_data.minute);
	ds1302_wirte_rig(0x84,time_data.hour);
	ds1302_wirte_rig(0x86,time_data.day);
	ds1302_wirte_rig(0x88,time_data.month);
	ds1302_wirte_rig(0x8a,time_data.week);
	ds1302_wirte_rig(0x8c,time_data.year);
	ds1302_wirte_rig(0x8e,0x80);//�ر�д����
}

// void ds1032_init(void)
// {
// 	struct TIMEData time_data;
// 	time_data.hour = 0x22;
// 	time_data.minute = 0x59;
// 	time_data.second = 0x0;
// 	time_data.month = 0x04;
// 	time_data.day = 0x07;
// 	time_data.week = 0x01;
// 	time_data.year = 0x2025;
// 	ds1302_write_realTime(time_data);
// }