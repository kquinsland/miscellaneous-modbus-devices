#include "uart.h"
#include "define.h"
#include "leddisplay.h"
#include "eeprom.h"
#include "key.h"
#include "ZS618_I2cSet.h"
#include "usual.h"

bit busy;												//�������ݷ�æ��־
bit	timer_flag=0;								//��ʱ�������־
bit led_flag=0;									//��ʱ�����8�α�־

unsigned char WaterCor;					//ˮλ��Ϣ
unsigned char Buffer1[4];				//�洢��ZS618��iic4���Ĵ����ж�ȡ������

unsigned int ShowTimer=0;						//����ܴ�ӡһ�ε�ʱ����
extern idata unsigned char Display[4];	//����ܴ�ӡ������
unsigned char KeyNum,Times;

void main()
{
	SystemInit();							//��Ƭ����ʼ��
	P1M0=0XFF;
	PEN=1;
	while(1)
	{
		if(timer_flag!=0 )			//��ʱ��0,ÿ0.7ms����һ��ˮλоƬiic�Ĵ�����ȡ����
		{
			timer_flag=0;
			i2c_read_direct(CON_ADDR,&Buffer1,0x04);		//���ϴ�д�Ĵ����ĵ�ַ��ʼ��,���ϴ�д�Ĵ���1����ζ�ȡ�ʹ�
																								//�Ĵ���1��ʼ��ȡ���Ĵ���3�����ϴ�д�Ĵ���0����ξʹӼĴ���
																								//0��ʼ��ȡ���Ĵ���3
			WaterCor=Buffer1[0];						//�Ĵ���0���ˮλ��Ϣ
		}
		
		if(led_flag!=0)			//ÿ8*0.7ms����һ�ΰ����ж�
		{
			led_flag=0;
			
			if(GetKeyAll(&Times,&KeyNum))	//�õ����°����ı�źͰ��µĴ���
			{
				if((Times==5)&&(KeyNum==2))
					PrintMCU();
				if((Times==6)&&(KeyNum==2))
					write(0x0da4,0x2aaa);
				SetOrRead(Times,KeyNum);		//���ݰ�����źʹ���������Ӧ�Ĳ���
				i2c_write(CON_ADDR, 0x00,&Times, 0);	//дһ�μĴ���0��������һ���Զ��ӼĴ���0��ʼ��
			}
			
			if(ShowTimer==100)	//����ܴ�ӡˮλ��Ϣ
			{
			  Display[0]=(WaterCor>>4)&0xf;
				Display[1]=WaterCor&0xf;
				Display[2]=0x10;
				Display[3]=0x10;
				ShowTimer=0;
				
				AddToUartBuf(WaterCor);
				UartSendOver();	
				
			}
			else
			{
				ShowTimer++;
			  DisplayAll();
			}
		}
	}
}

