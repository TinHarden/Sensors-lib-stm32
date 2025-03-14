#include <string.h>
#include "main.h"
#include "AS608.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "projdefs.h"
#include "usart.h"

uint32_t AS608Addr = 0XFFFFFFFF; //Ĭ��
SysPara AS608Para;//ָ��ģ��AS608����
uint16_t ValidN;//ģ������Чģ�����
uint8_t aRxBuffer[RXBUFFERSIZE];//���ջ���
uint8_t RX_len;//�����ֽڼ���

//���ڷ���һ���ֽ�
static void Com_SendData(uint8_t data)
{
	HAL_UART_Transmit(&huart1,&data, 1,50);
}
//���Ͱ�ͷ
static void SendHead(void)
{
	Com_SendData(0xEF);
	Com_SendData(0x01);
}
//���͵�ַ
static void SendAddr(void)
{
	Com_SendData(AS608Addr>>24);
	Com_SendData(AS608Addr>>16);
	Com_SendData(AS608Addr>>8);
	Com_SendData(AS608Addr);
}
//���Ͱ���ʶ,
static void SendFlag(uint8_t flag)
{
	Com_SendData(flag);
}
//���Ͱ�����
static void SendLength(int length)
{
	Com_SendData(length>>8);
	Com_SendData(length);
}
//����ָ����
static void Sendcmd(uint8_t cmd)
{
	Com_SendData(cmd);
}
//����У���
static void SendCheck(uint16_t check)
{
	Com_SendData(check>>8);
	Com_SendData(check);
}
//�ж��жϽ��յ�������û��Ӧ���
//waittimeΪ�ȴ��жϽ������ݵ�ʱ�䣨��λ1ms��
//����ֵ�����ݰ��׵�ַ
extern uint8_t RX_len;//�����ֽڼ���
static uint8_t *JudgeStr(uint16_t waittime)
{
	char *data;
	uint8_t str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';
	
	while(--waittime)
	{
		osDelay(pdMS_TO_TICKS(1));
		if(RX_len)//���յ�һ������
		{
			RX_len=0;
			data=strstr((const char*)&aRxBuffer,(const char*)str);
			if(data)
				return (uint8_t*)data;	
		}
	}
	return 0;
}
//¼��ͼ�� GZ_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer�� 
//ģ�鷵��ȷ����
uint8_t GZ_GetImage(void)
{
	uint16_t temp;
	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x01);
	temp =  0x01+0x03+0x01;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//�������� GZ_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2			 
//����:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//ģ�鷵��ȷ����
uint8_t GZ_GenChar(uint8_t BufferID)
{
	uint16_t temp;
	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(0x02);
	Com_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//��ȷ�ȶ���öָ������ GZ_Match
//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ� 
//ģ�鷵��ȷ����
uint8_t GZ_Match(void)
{
	uint16_t temp;
	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//����ָ�� GZ_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣			
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
uint8_t GZ_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x04);
	Com_SendData(BufferID);
	Com_SendData(StartPage>>8);
	Com_SendData(StartPage);
	Com_SendData(PageNum>>8);
	Com_SendData(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(uint8_t)StartPage
	+(PageNum>>8)+(uint8_t)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//�ϲ�����������ģ�壩GZ_RegModel
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2	
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_RegModel(void)
{
	uint16_t temp;
	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}
//����ģ�� GZ_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�			
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_StoreChar(uint8_t BufferID,uint16_t PageID)
{
	uint16_t temp;
	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(0x06);
	Com_SendData(BufferID);
	Com_SendData(PageID>>8);
	Com_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(uint8_t)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}
//ɾ��ģ�� GZ_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_DeletChar(uint16_t PageID,uint16_t N)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x0C);
	Com_SendData(PageID>>8);
	Com_SendData(PageID);
	Com_SendData(N>>8);
	Com_SendData(N);
	temp = 0x01+0x07+0x0C+(PageID>>8)+(uint8_t)PageID+(N>>8)+(uint8_t)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//���ָ�ƿ� GZ_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_Empty(void)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//дϵͳ�Ĵ��� GZ_WriteReg
//����:  дģ��Ĵ���
//����:  �Ĵ������RegNum:4\5\6
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_WriteReg(uint8_t RegNum,uint8_t DATA)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x05);
	Sendcmd(0x0E);
	Com_SendData(RegNum);
	Com_SendData(DATA);
	temp = RegNum+DATA+0x01+0x05+0x0E;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//��ϵͳ�������� GZ_ReadSysPara
//����:  ��ȡģ��Ļ��������������ʣ�����С��)
//����:  ��
//˵��:  ģ�鷵��ȷ���� + ����������16bytes��
uint8_t GZ_ReadSysPara(SysPara *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01+0x03+0x0F;
	SendCheck(temp);
	data=JudgeStr(1000);
	if(data)
	{
		ensure = data[9];
		p->GZ_max = (data[14]<<8)+data[15];
		p->GZ_level = data[17];
		p->GZ_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		p->GZ_size = data[23];
		p->GZ_N = data[25];
	}		
	else
		ensure=0xff;
	return ensure;
}
//����ģ���ַ GZ_SetAddr
//����:  ����ģ���ַ
//����:  GZ_addr
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_SetAddr(uint32_t GZ_addr)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x15);
	Com_SendData(GZ_addr>>24);
	Com_SendData(GZ_addr>>16);
	Com_SendData(GZ_addr>>8);
	Com_SendData(GZ_addr);
	temp = 0x01+0x07+0x15
	+(uint8_t)(GZ_addr>>24)+(uint8_t)(GZ_addr>>16)
	+(uint8_t)(GZ_addr>>8) +(uint8_t)GZ_addr;				
	SendCheck(temp);
	AS608Addr=GZ_addr;//������ָ�������ַ
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = GZ_addr;
	if(ensure==0x00)//���õ�ַ�ɹ�
	{
		
	}
	return ensure;
}
//���ܣ� ģ���ڲ�Ϊ�û�������256bytes��FLASH�ռ����ڴ��û����±�,
//	�ü��±��߼��ϱ��ֳ� 16 ��ҳ��
//����:  NotePageNum(0~15),Byte32(Ҫд�����ݣ�32���ֽ�)
//˵��:  ģ�鷵��ȷ����
uint8_t GZ_WriteNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
	uint16_t temp;
  uint8_t  ensure,i;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(36);
	Sendcmd(0x18);
	Com_SendData(NotePageNum);
	for(i=0;i<32;i++)
	 {
		 Com_SendData(Byte32[i]);
		 temp += Byte32[i];
	 }
  temp =0x01+36+0x18+NotePageNum+temp;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//������GZ_ReadNotepad
//���ܣ�  ��ȡFLASH�û�����128bytes����
//����:  NotePageNum(0~15)
//˵��:  ģ�鷵��ȷ����+�û���Ϣ
uint8_t GZ_ReadNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
	uint16_t temp;
  uint8_t  ensure,i;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(0x19);
	Com_SendData(NotePageNum);
	temp = 0x01+0x04+0x19+NotePageNum;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		for(i=0;i<32;i++)
		{
			Byte32[i]=data[10+i];
		}
	}
	else
		ensure=0xff;
	return ensure;
}
//��������GZ_HighSpeedSearch
//���ܣ��� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
//		  �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ����
//		  �ܺõ�ָ�ƣ���ܿ�������������
//����:  BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
//˵��:  ģ�鷵��ȷ����+ҳ�루����ָ��ģ�壩
uint8_t GZ_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x1b);
	Com_SendData(BufferID);
	Com_SendData(StartPage>>8);
	Com_SendData(StartPage);
	Com_SendData(PageNum>>8);
	Com_SendData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	+(StartPage>>8)+(uint8_t)StartPage
	+(PageNum>>8)+(uint8_t)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
 	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[10]<<8) +data[11];
		p->mathscore=(data[12]<<8) +data[13];
	}
	else
		ensure=0xff;
	return ensure;
}
//����Чģ����� GZ_ValidTempleteNum
//���ܣ�����Чģ�����
//����: ��
//˵��: ģ�鷵��ȷ����+��Чģ�����ValidN
uint8_t GZ_ValidTempleteNum(uint16_t *ValidN)
{
	uint16_t temp;
  uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01+0x03+0x1d;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
	return ensure;
}
//��AS608���� GZ_HandShake
//����: GZ_Addr��ַָ��
//˵��: ģ�鷵�µ�ַ����ȷ��ַ��	
uint8_t GZ_HandShake(uint32_t *GZ_Addr)
{
	SendHead();
	SendAddr();
	Com_SendData(0X01);
	Com_SendData(0X00);
	Com_SendData(0X00);	
	osDelay(pdMS_TO_TICKS(200));
	if(RX_len)
	{
		RX_len=0;
		if(//�ж��ǲ���ģ�鷵�ص�Ӧ���				
					aRxBuffer[0]==0XEF
				&&aRxBuffer[1]==0X01
				&&aRxBuffer[6]==0X07
			)
			{
				*GZ_Addr=(aRxBuffer[2]<<24) + (aRxBuffer[3]<<16)
								+(aRxBuffer[4]<<8) + (aRxBuffer[5]);
				return 0;
			}

	}
	return 1;		
}
//ģ��Ӧ���ȷ������Ϣ����
//���ܣ�����ȷ���������Ϣ������Ϣ
//����: ensure
const char *EnsureMessage(uint8_t ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="���ݰ����մ���";break;
		case  0x02:
			p="��������û����ָ";break;
		case  0x03:
			p="¼��ָ��ͼ��ʧ��";break;
		case  0x04:
			p="ָ��ͼ��̫�ɡ�̫��������������";break;
		case  0x05:
			p="ָ��ͼ��̫ʪ��̫��������������";break;
		case  0x06:
			p="ָ��ͼ��̫�Ҷ�����������";break;
		case  0x07:
			p="ָ��ͼ����������������̫�٣������̫С��������������";break;
		case  0x08:
			p="ָ�Ʋ�ƥ��";break;
		case  0x09:
			p="û������ָ��";break;
		case  0x0a:
			p="�����ϲ�ʧ��";break;
		case  0x0b:
			p="����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ";
		case  0x10:
			p="ɾ��ģ��ʧ��";break;
		case  0x11:
			p="���ָ�ƿ�ʧ��";break;	
		case  0x15:
			p="��������û����Чԭʼͼ��������ͼ��";break;
		case  0x18:
			p="��д FLASH ����";break;
		case  0x19:
			p="δ�������";break;
		case  0x1a:
			p="��Ч�Ĵ�����";break;
		case  0x1b:
			p="�Ĵ����趨���ݴ���";break;
		case  0x1c:
			p="���±�ҳ��ָ������";break;
		case  0x1f:
			p="ָ�ƿ���";break;
		case  0x20:
			p="��ַ����";break;
		default :
			p="ģ�鷵��ȷ��������";break;
	}
 return p;	
}

void ZhiWenInit()
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) aRxBuffer, RXBUFFERSIZE);
}
//¼ָ��
void Add_FR(uint16_t ID)
{
	uint8_t i=0,ensure ,processnum=0;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				LCD_PrintString(3, 0, "�밴ָ��");
				ensure=GZ_GetImage();
				if(ensure==0x00)
				{
					ensure=GZ_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//�����ڶ���
					}
				}
				break;
			case 1:
				i++;
				ensure=GZ_GetImage();
				if(ensure==0x00)
				{
					ensure=GZ_GenChar(CharBuffer2);//��������
					if(ensure==0x00)
					{
						i=0;
						processnum=2;//����������
					}
				}
				break;
			case 2:
				ensure=GZ_Match();
				if(ensure==0x00)
				{
					processnum=3;//�������Ĳ�
				}
				else
				{
					i=0;
					processnum=0;//���ص�һ��
				}
				osDelay(pdMS_TO_TICKS(1000));
				break;
			case 3:
				ensure=GZ_RegModel();
				if(ensure==0x00)
				{
					processnum=4;//�������岽
				}else {processnum=0;}
				osDelay(pdMS_TO_TICKS(1000));
				break;
			case 4:
				ensure=GZ_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00)
				{
					LCD_Clear();
					LCD_PrintString(3,0,"success");
					GZ_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					osDelay(pdMS_TO_TICKS(1500));//��ʱ�������ʾ
					return ;
				}else
				{
					processnum = 0;
				}
				break;
		}
		osDelay(pdMS_TO_TICKS(800));
		if(i==10)//����10��û�а���ָ���˳�
		{
			LCD_PrintString(3,0,"timeout");
			break;
		}
	}
}

//ˢָ��
uint8_t press_FR(void)
{
	SearchResult seach;
	uint8_t ensure;
	ensure = GZ_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ�
	{
		ensure=GZ_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{
			ensure = GZ_HighSpeedSearch(CharBuffer1, 0, 300, &seach);
			if(ensure==0x00)//�����ɹ�
			{
				if (seach.mathscore > 100)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

// //ɾ��ָ��
// void Del_FR(void)
// {
// 	// GZ_Empty();//���ָ�ƿ�
// 	GZ_DeletChar(0,1);//ɾ������ָ��
// }



