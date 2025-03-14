#ifndef __AS608_H
#define __AS608_H
#include "main.h"

#define RXBUFFERSIZE 500
#define CharBuffer1 0x01
#define CharBuffer2 0x02

typedef struct  
{
	uint16_t pageID;//ָ��ID
	uint16_t mathscore;//ƥ��÷�
}SearchResult;

typedef struct
{
	uint16_t GZ_max;//ָ���������
	uint8_t  GZ_level;//��ȫ�ȼ�
	uint32_t GZ_addr;
	uint8_t  GZ_size;//ͨѶ���ݰ���С
	uint8_t  GZ_N;//�����ʻ���N
}SysPara;

extern uint32_t AS608Addr; //Ĭ��
extern SysPara AS608Para;//ָ��ģ��AS608����
extern uint16_t ValidN;//ģ������Чģ�����
extern uint8_t aRxBuffer[RXBUFFERSIZE];//���ջ���
extern uint8_t RX_len;//�����ֽڼ���

uint8_t GZ_GetImage(void); //¼��ͼ�� 
 
uint8_t GZ_GenChar(uint8_t BufferID);//�������� 

uint8_t GZ_Match(void);//��ȷ�ȶ���öָ������ 

uint8_t GZ_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);//����ָ�� 
 
uint8_t GZ_RegModel(void);//�ϲ�����������ģ�壩 
 
uint8_t GZ_StoreChar(uint8_t BufferID,uint16_t PageID);//����ģ��

uint8_t GZ_WriteReg(uint8_t RegNum,uint8_t DATA);//дϵͳ�Ĵ��� 
 
uint8_t GZ_ReadSysPara(SysPara *p); //��ϵͳ�������� 

uint8_t GZ_SetAddr(uint32_t addr);  //����ģ���ַ 

uint8_t GZ_WriteNotepad(uint8_t NotePageNum,uint8_t *content);//д���±� 

uint8_t GZ_ReadNotepad(uint8_t NotePageNum,uint8_t *note);//������ 

uint8_t GZ_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);//�������� 

uint8_t GZ_HandShake(uint32_t *GZ_Addr); //��AS608ģ������

const char *EnsureMessage(uint8_t ensure);//ȷ���������Ϣ����

uint8_t GZ_Empty(void);//���ָ�ƿ�
uint8_t GZ_DeletChar(uint16_t PageID,uint16_t N);//ɾ��ģ��
uint8_t GZ_ValidTempleteNum(uint16_t *ValidN);//����Чģ�����
void ZhiWenInit();
void Add_FR(uint16_t ID);
uint8_t press_FR(void);

#endif

