#include "HX711.h"

#define GapValue 405

int32_t weight_mao = 0;

void Weight_Init(void)
{
	HAL_Delay(100);
	HAL_GPIO_WritePin(WEIGHT_CLK_GPIO_Port,WEIGHT_CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WEIGHT_DATA_GPIO_Port,WEIGHT_DATA_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

uint32_t HX711_Read(void)	//����128
{
	uint32_t count = 0;
	uint8_t i;
	HAL_GPIO_WritePin(WEIGHT_CLK_GPIO_Port,WEIGHT_CLK_Pin, GPIO_PIN_RESET);
	while (HAL_GPIO_ReadPin(WEIGHT_DATA_GPIO_Port,WEIGHT_DATA_Pin))
	{
		Delay_us(1);
	}
  	for(i=0;i<24;i++)
	{
  		HAL_GPIO_WritePin(WEIGHT_CLK_GPIO_Port,WEIGHT_CLK_Pin, GPIO_PIN_SET);
  		count=count<<1;
	  	if(HAL_GPIO_ReadPin(WEIGHT_DATA_GPIO_Port,WEIGHT_DATA_Pin))
	  	{
	  		count++;
	  	}
  		HAL_GPIO_WritePin(WEIGHT_CLK_GPIO_Port,WEIGHT_CLK_Pin, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(WEIGHT_CLK_GPIO_Port,WEIGHT_CLK_Pin, GPIO_PIN_SET);
    count=count^0x800000;
	HAL_GPIO_WritePin(WEIGHT_CLK_GPIO_Port,WEIGHT_CLK_Pin, GPIO_PIN_RESET);
	return count;
}

void Get_Mao()
{
	weight_mao = (int32_t)(HX711_Read()/GapValue);
}
int32_t Get_Weight(void)
{
	int32_t Weight_Shiwu = 0;
	if (HX711_Read() / GapValue - weight_mao>0)
	{
		Weight_Shiwu = (int32_t)(HX711_Read() / GapValue - weight_mao);
	} else
	{
		Weight_Shiwu = 0;
	}
	return Weight_Shiwu;
}
