#include "dht11.h"

uint8_t Time = 0;

void Delay_ms(uint16_t ms) {
    osDelay(ms);
}

void Delay_us(uint16_t us) {
    // 假设定时器的计数器频率是 1MHz (1 tick = 1us)
    uint32_t start_time = TIM4->CNT;  // 获取当前定时器计数值
    uint32_t delay_ticks = us;

    // 等待直到定时器计数器达到延时时间
    while ((TIM4->CNT - start_time) < delay_ticks) {
        // 可选：可以在此加入 taskYIELD() 来给任务调度让出 CPU
        taskYIELD();
    }
}


void DHT11_Mode(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(mode)
	{
		GPIO_InitStructure.Pin = DHT11;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}
	else
	{
        GPIO_InitStructure.Pin = DHT11;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
	}
	HAL_GPIO_Init(DHT11_PROT, &GPIO_InitStructure);
}

void DHT11_Sends_Start(void)
{
	DHT11_Mode(OUT);
	
	DHT11_Low;
	Delay_ms(18);	//18ms
	DHT11_High;
	Delay_us(40);	//40us
}

void DHT11_Sends_Response(void)
{
	DHT11_Mode(INT);
	
	while(HAL_GPIO_ReadPin(DHT11_PROT,DHT11) == 0 && (Time < 100))	//Low 80us
	{
		Time++;
		Delay_us(1);	//100us DHT11=0 80us
	}
	Time = 0;
	
	while(HAL_GPIO_ReadPin(DHT11_PROT,DHT11) == 1 && (Time < 100))	//High 80us
	{
		Time++;
		Delay_us(1);	//100us DHT11=0 80us
	}	
	Time = 0;
}

uint8_t DHT11_Read_Byte(void)
{
    uint8_t data = 0;
	
	for(uint8_t i = 0; i < 8; i++)
	{
		DHT11_Mode(INT);
		
		while(HAL_GPIO_ReadPin(DHT11_PROT,DHT11) == 0 && (Time < 100))
		{
			Time++;
			Delay_us(1);	//100us DHT11=0 80us
		}	
		Time = 0;
		
		data <<= 1;
	
		Delay_us(40);	//get hight 70us
		
		if(HAL_GPIO_ReadPin(DHT11_PROT, DHT11) == 1)
		{
			data |= 0x01;
		
		}
		
		while(HAL_GPIO_ReadPin(DHT11_PROT,DHT11) == 1 && (Time < 100))
		{
			Time++;
			Delay_us(1);	//100us DHT11=0 80us
		}	
		Time = 0;
	}
	return data;
}

void DHT11_Read_Data(uint8_t *humi, uint8_t *temp)
{
    uint8_t DATA[5] = {0,0,0,0,0,};
	
	DHT11_Sends_Start();
	DHT11_Sends_Response();
	
	for(uint8_t i = 0; i < 5; i++)
	{
		DATA[i] = DHT11_Read_Byte();
	}
	Delay_ms(1);	//1ms 50us
	
	if(DATA[0] + DATA[1] + DATA[2] + DATA[3] == DATA[4])
	{
		*humi = DATA[0];
		*temp = DATA[2];
	}
	else
	{
		for(uint8_t i = 0; i < 5; i++)
		{
			DATA[i] = 0;
		}
	
	}
}
