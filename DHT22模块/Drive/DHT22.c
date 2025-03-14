#include "dht22.h"

const uint32_t MAX_TIMER = 20000;

void Delay_ms(uint16_t ms) {
    osDelay(ms);
}
void DHT11_Mode(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(mode)
	{
		GPIO_InitStructure.Pin = DHT22_Pin;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}
	else
	{
		GPIO_InitStructure.Pin = DHT22_Pin;
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	}
	HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStructure);
}
//DHT22 起始信号
void DHT22_Rst(void)	   
{
	DHT11_Mode(1); //输出模式
	DHT22_OUT_Low; 	//拉低
	Delay_ms(10);
	DHT22_OUT_High; 	//拉高
	Delay_us(30);
}
//等待DHT22响应
//1:失败  0:成功
uint8_t DHT22_Check(void) 	   
{   
	int8_t y=100;
	DHT11_Mode(0);// 切换到输入模式
	while ((!DHT22_IN) && y)//拉低过程
	{
		y--;
		Delay_us(1);
	};	 
	if(y<1) return 1;
	while (DHT22_IN && y)//拉高过程
	{
		y--;
		Delay_us(1);
	};
	if(y<1) return 1;	    
	return 0;
}
//读取1位    1/0
uint8_t DHT22_Read_Bit(void) 			 
{
	uint8_t bit;
	uint8_t y=100;
	DHT11_Mode(0);// 切换到输入模式
	while((!DHT22_IN) && y)//等待高
	{
		y--;
		Delay_us(1);
	}
	y=100;
	bit=0;
	Delay_us(30);
	if(DHT22_IN)bit=1;
	while(DHT22_IN && y)//等待低
	{
		y--;
		Delay_us(1);
	}
	return bit;
}
//一个字节
uint8_t DHT22_Read_Byte(void)    
{        
	uint8_t i,byt;
	byt=0;
	for (i=0;i<8;i++) 
	{
		byt<<=1; 
		byt|=DHT22_Read_Bit();
	}
	return byt;
}
//一次数据
//temp:温度
//hum:湿度
//1成功   0失败
uint8_t DHT22_Read_Data(uint16_t* hum, uint16_t* temp)
{        
	uint8_t buf[5];
	uint8_t i;
	
	DHT22_Rst();
	if(DHT22_Check()==0)
	{
		for(i=0;i<5;i++)//全部读取  40个
		{
			buf[i]=DHT22_Read_Byte();
		}	
		//数据校核
		if((unsigned char)(buf[0]+buf[1]+buf[2]+buf[3])==buf[4]) //防止超出范围的强制转换
		{
			*hum=buf[0] << 8 | buf[1];
			*temp=buf[2] << 8 | buf[3];
			return 1;
		}
	}
	return 0;   
}