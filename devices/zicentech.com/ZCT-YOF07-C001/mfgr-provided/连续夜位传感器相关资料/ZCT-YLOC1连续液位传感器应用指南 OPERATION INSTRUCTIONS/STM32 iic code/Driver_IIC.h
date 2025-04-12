#ifndef _IIC_H
#define _IIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}	//PB7����ģʽ
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;} 	//PB7���ģʽ

//IO����
#define IIC_SCL   PBout(6) //SCL
#define IIC_SDA   PBout(7) //SDA
#define READ_SDA  PBin(7)  //����SDA

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 

u8	AT24CXX_ReadOneByte(u16 dev_addr,u8 * dest_buf,u16 len);
//u8 AT24CXX_ReadOneByte(u16 ReadAddr);	
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);	
u8 ReadThresholdFrom601(unsigned char chNum,unsigned int *sample);
u8 SetThresholdTo601(unsigned char chNum,unsigned int *sample);
void SetOrRead(unsigned char times,unsigned char keynum);

u8 write(unsigned int addr1,unsigned int data1);
u8 dealprg(unsigned char keytime,unsigned int onedata);
void ResetMCUPower();
void StartPGM();
u8 ReadSingleWordFromMCU(unsigned int addr);
u8 WriteSingleWordToMCU(unsigned int addr,unsigned int worddata);

//u8	AT24CXX_WriteOneByte(u16 dev_addr, u16 reg_addr, u8 * src_buf, u8 len)





extern u8	AT24CXX_ReadOneByte(u16 dev_addr,u8 * dest_buf,u16 len);
extern void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);	
extern u8 ReadThresholdFrom601(unsigned char chNum,unsigned int *sample);
extern u8 SetThresholdTo601(unsigned char chNum,unsigned int *sample);

extern void SetOrRead(unsigned char times,unsigned char keynum);



#endif
