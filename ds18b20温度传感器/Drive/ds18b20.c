
#include "ds18b20.h"

#include "cmsis_os2.h"

uint8_t DS18B20_Read_Byte(void)
{
        uint8_t i=0,dat=0;

        for (i=0;i<8;i++)
        {
        DQ_OUT();//设置为输入模式
        DQ(0); //拉低
        delay_us(2);
        DQ(1); //释放总线
        DQ_IN();//设置为输入模式
        delay_us(12);
        dat>>=1;
        if( DQ_GET() )
        {
            dat=dat|0x80;
        }
        delay_us(50);
     }
        return dat;
}
void DS18B20_Write_Byte(uint8_t dat)
{
        uint8_t i;
        DQ_OUT();//设置输出模式
        for (i=0;i<8;i++)
        {
                if ( (dat&0x01) ) //写1
                {
                        DQ(0);
                        delay_us(2);
                        DQ(1);
                        delay_us(60);
                }
                else //写0
                {
                        DQ(0);//拉低60us
                        delay_us(60);
                        DQ(1);//释放总线
                        delay_us(2);
                }
                dat=dat>>1;//传输下一位
        }
}
uint8_t DS18B20_Check(void)
{
        uint8_t timeout=0;
        //复位DS18B20
        DQ_OUT(); //设置为输出模式
        DQ(1);
        delay_us(15);
        DQ(0); //拉低DQ
        delay_us(740); //拉低750us
        DQ(1); //拉高DQ
        delay_us(20);  //15us
    //设置为输入模式
        DQ_IN();
    //等待拉低，拉低说明有应答
        while ( DQ_GET() &&timeout<240)
        {
                timeout++;//超时判断
                delay_us(1);
        }
        //设备未应答
        if(timeout>=240)
                return 1;
        timeout=0;
        //等待18B20释放总线
        while ( !DQ_GET() &&timeout<240)
        {
                timeout++;//超时判断
                delay_us(1);
        };
    //释放总线失败
        if(timeout>=240)
                return 1;
        return 0;
}
void DS18B20_Start(void)
{
        DS18B20_Check();                //查询是否有设备应答
        DS18B20_Write_Byte(0xcc);   //对总线上所有设备进行寻址
        DS18B20_Write_Byte(0x44);   //启动温度转换
}
float DS18B20_GetTemperture(void)
{
        uint16_t temp;
        uint8_t dataL=0,dataH=0;
        float value;
        DS18B20_Start();
        osDelay(750);
        DS18B20_Check();
        DS18B20_Write_Byte(0xcc);//对总线上所有设备进行寻址
        DS18B20_Write_Byte(0xbe);// 读取数据命令
        dataL=DS18B20_Read_Byte(); //LSB
        dataH=DS18B20_Read_Byte(); //MSB
        temp=(dataH<<8)+dataL;//整合数据
        if(dataH&0X80)//高位为1，说明是负温度
        {
                temp=(~temp)+1;
                value=temp*(-0.0625);
        }
        else
        {
                value=temp*0.0625;
        }
        return value;
}