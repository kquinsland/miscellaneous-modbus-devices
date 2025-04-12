////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// All rights reserved.
//
// Filename     ��Driver_UART.c
// Version      ��1.0
// Author       ��zhangjun
// Accomplished date �� 2016.7.1
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#ifndef LIB_DRIVER_UART
#define LIB_DRIVER_UART
#endif

#include <App_includes.h>
#include <Driver_includes.h>
#include "Driver_IIC.h"
#include "App_parameter.h"

//IIC��ʼ��

#define NACK 0
#define ACK  1
#define CON_ADDR 0X40
#define START_INDEX 0x0DA0
unsigned int val[4]={0x0000,0x4000,0x6000,0x8000};
u8 buffer[4];

//extern CONFIG_PARAM ConfigPara;                          //���ò���

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PH4,5��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    IIC_SDA=1;
    IIC_SCL=1;  

	
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	//delay_us(4);
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	//delay_us(4);
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�	
    delay_us(4);	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	  

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=1; 
        delay_us(2);
		IIC_SCL=0;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
   // if (!ack)
     //   IIC_NAck();//����nACK
  //  else
     //   IIC_Ack(); //����ACK   
    return receive;
}
	/*u8 IIC_Read_Byte(unsigned char ack)
	{
		unsigned char i,receive=0;
		SDA_IN();//SDA����Ϊ����
		for(i=0;i<8;i++ )
		{
			IIC_SCL=1; 
			delay_us(2);
			receive<<=1;
			if(READ_SDA)receive++;	 
			IIC_SCL=0;
		}					 
		if (!ack)
			IIC_NAck();//����nACK
		else
			IIC_Ack(); //����ACK	 
		return receive;
	}*/







/*u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
  IIC_Send_Byte(ReadAddr<<1+1));   //����������ַ0XA0,д���� 	   
 // IIC_Send_Byte(ReadAddr<<1);   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0X81);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}*/

unsigned char	AT24CXX_ReadOneByte(u16 dev_addr,unsigned char *dest_buf,u16 len)
{
	char i;
    IIC_Start();//����  
    IIC_Send_Byte((dev_addr<<1)+1);//���ͷ��ʹ��豸��ַ ������  
   IIC_Wait_Ack();  
	delay_us(4);
	dest_buf[0]=IIC_Read_Byte(0);//��ȡ���� 
	for(i=1;i<len;i++)
	{
		IIC_Ack();
		delay_us(4);
		dest_buf[i]=IIC_Read_Byte(0);
	}
	IIC_NAck();
	delay_us(4);
    IIC_Stop();//ֹͣ  
	return 1;
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
//void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
//{
//	while(NumToRead)
//	{
//		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
//		NumToRead--;
//	}
//}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
/*void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(0X40+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_us(10);	 
}*/
u8	AT24CXX_WriteOneByte(u16 dev_addr, u16 reg_addr, unsigned char * src_buf, u8 len)
{
	char i;
	IIC_Start();
    IIC_Send_Byte((dev_addr<<1));//���ͷ��ʹ��豸��ַ д���� 
    if(IIC_Wait_Ack()==1)
    	{
    	return 0; 
    	}
	delay_us(4);  		 
    IIC_Send_Byte(reg_addr);//���ͷ��ͼĴ�����ַ д���� 
    if(IIC_Wait_Ack()==1)
    	{
    	return 0; 
    	}
	delay_us(4);  
	for(i=0;i<len;i++)
	{
	    IIC_Send_Byte(src_buf[i]);//���ͻ��������� д���� 
	    IIC_Wait_Ack();//�ȴ����豸����Ӧ  
		delay_us(4);  				
	}
    IIC_Stop();//ֹͣ 
	return 1;
}

u8 ReadThresholdFrom601(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j=0,chnum;
	unsigned char buffer[4];
	unsigned char CON_RESA2EEP=0xCA;
	unsigned char buffer2[4];
	u8 flag=0;
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x01,&CON_RESA2EEP, 1)==0)//���е�Ƭ����ˮλоƬ���ֲ���
	{
		return 0;
	}
	for(i=0;i<20;i++)
	{
		if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ����е����ݣ��жϵ�Ƭ����ˮλоƬ�Ƿ����ֳɹ�
		{	
			return 0;
		}
		if(buffer[0]==0xCD)
		{
			flag=1;
			break;
		}
		else if(j>=19)
		{
			return 0;
		}
		else
			delay_us(500);
		j++;
	}
//delay_ms(5000);
	if(flag==1)
	{
		chnum=chNum+0x30;
		buffer2[0]=chnum;
		buffer2[1]=0x02;
		buffer2[2]=0x03;
		buffer2[3]=0x04;
		if(AT24CXX_WriteOneByte(CON_ADDR, 0x01,buffer2, 3)==0)//���ֳɹ��󣬷���ˮλоƬ������ͨ����ż���0x30
			return 0;
		for(j=0;j<200;j++)
		{
			if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ������ж��Ƿ�����ɹ�
				return 0;
			if(buffer[0]==chNum)//�ж�ˮλоƬ�����ɹ��󣬽��õ��Ĳ���ֵ��ֵ��ָ��
			{
				*sample=buffer[1]*256+buffer[2];
				break;
			}
			else if(j>=199)
			{
				return 0;
			}
			else
				delay_us(500);
		}
		AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03);
		return 1;
	}
	else
		return 0;
}

u8 write(unsigned int addr1,unsigned int data1)
{
	ResetMCUPower();	
	StartPGM();
	if(WriteSingleWordToMCU(addr1,data1)!=1)
	{
		
		return 0;
	}
		
	return 1;
}


u8 dealprg(unsigned char keytime,unsigned int onedata)
{
	unsigned int i;
	unsigned int oneword,oneaddr;
	ResetMCUPower();  //������Դ
	
	StartPGM();
	if(ReadSingleWordFromMCU(START_INDEX)!=1)
	{
		
		return 0;
	}
	for(i=0;i<1000;i++)
	{
		if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x02)==0)
		{
			
			return 0;
		}
		
		//UartSendOver();	
		//UartSendOver();	
		oneword=((buffer[0]*256+buffer[1])&0xe000);
		if((oneword==val[0])||(oneword==val[1])||(oneword==val[2])||(oneword==val[3]))
			continue;
		else if(oneword==0xe000)
		{
			oneaddr=START_INDEX+i*2;
			break;
		}
		else
			return 0;
	}
	onedata=(onedata&0x1fff)|val[keytime-1];

	if(write(oneaddr,onedata)!=1)
	{
		ResetMCUPower();	
		return 0;
	}
	ResetMCUPower();	
	return 1;
}

u8 SetThresholdTo601(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,chnum;
	unsigned char setsampl[2];
	unsigned char CON_SET601CR=0x8C;
	u8 flag=0;
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x01,&CON_SET601CR, 1)==0)//���е�Ƭ����ˮλоƬ���ֲ���
		return 0;
	for(i=0;i<20;i++)
	{
		if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ����е����ݣ��жϵ�Ƭ����ˮλоƬ�Ƿ����ֳɹ�
			return 0;	
		if(buffer[0]==0xCD)
		{
			flag=1;
			break;
		}
		else if(i>=19)
			return 0;
		else
			delay_us(500);
	}
	if(flag==1)//���ֳɹ��󣬷�����Ҫ���õ�ˮλоƬ������ͨ����ż���0x30
	{
		chnum=chNum+0x30;
		setsampl[0]=(*sample)/256;
		setsampl[1]=(*sample)%256;
		if(AT24CXX_WriteOneByte(CON_ADDR, 0x02, setsampl, 2)==0)	
			return 0;	
		if(AT24CXX_WriteOneByte(CON_ADDR, 0x01, &chnum, 1)==0)			
			return 0;	
		for(i=0;i<20;i++)
		{
			if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ������ж��Ƿ�����ɹ�
				return 0;	
			if(buffer[0]==chNum)
				break;
			else if(i>=19)				
				return 0;	
			else
				delay_us(500);
		}
		return 1;
	}
	else
		return 0;
}

void ResetMCUPower()
{
	Digout(2,0);  //12-24V��
	Digout(0,0);  //3.3V��
	IIC_SDA=0;	  	  
	IIC_SCL=0;						//SDA��SCL�ϵ�
	delay_us(5);
	Digout(0,1);  //3.3V��
	delay_us(1);
	IIC_SDA=1;	  	  
	IIC_SCL=1;						//SDA��SCL�ָ�
}

void StartPGM()
{
	buffer[0] = 0x58;
	buffer[1] = 0x58;
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x00,buffer, 2)==1)
	{
		
	}
}

u8 ReadSingleWordFromMCU(unsigned int addr)
{
	//��������ȡ���ݣ���buffer[1]buffer[0]����
	if(addr!=(addr&0xfff))	//��ַ����Χ
	{
		return 0;		
	}
	//�͵��ֽڶ�����
	buffer[3] = 0x60 | (*((char * )(&addr))&0x1f);
	buffer[2] = addr;
	buffer[1] = 0;
	buffer[0] = 0;	
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x00,buffer, 4)==0)
	{
		return 0;
	}

	return	1;
		
}

u8 WriteSingleWordToMCU(unsigned int addr,unsigned int worddata)
{
	if(addr!=(addr&0xfff))	//��ַ����Χ
	{
		
		return 0;		
	}
	
	buffer[3] = 0xE0 | (*((char * )(&addr))&0x1f);
	buffer[2] = addr;
	buffer[1] = worddata;
	buffer[0] = *((char * )(&worddata));

		
	Digout(2,1);  //12-24V��
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x00,buffer, 4)==0)
	{
		
		return 0;
	}		
	delay_us(20);
	if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x02)==0)
	{
		
		return 0;
	}
	Digout(2,0);  //12-24V��

	return 1;
}





void SetOrRead(unsigned char times,unsigned char keynum)
{
	unsigned int threshold;
	unsigned int StoreStd[4];	
	unsigned char i;
	if(((times==1)||(times==2))&&(keynum==1))
	{
		//���ñ궨ֵ�����ַ�����һ�����߱궨��һ����¼��601ROM�е��粻��ʧ�����������ַ������У�����Լ��������ð���������߱궨Ϊ�ο�
		//������������ð�궨���Ͳ���Ҫ�Լ������궨����
		
		if(ReadThresholdFrom601(times,&threshold)==1)//����һ��ͨ��IIC��ȡоƬ5�ŵı궨����ֵ����������ͨ��IIC��ȡоƬ4�ŵı궨����ֵ
		{
			
			StoreStd[times-1]=threshold;
			Digout(2,1);  //12-24V��
			
			if(SetThresholdTo601(times,&threshold)==1)//�������ñ궨ֵ
			{
				printf("��ˮλоƬ4-5�ű궨ֵ:%d \r\n",threshold);//
				
				StoreStd_Set(times-1,threshold);
				Digout(2,0);  //12-24V��
			}
			
		}
		else
		{
			return;
		}
		/*if(dealprg(times,threshold)!=1)//���궨ֵ��¼��618ROM�У����粻��ʧ�������ϵ粻��Ҫ�ٱ궨
		{
			if(dealprg(times,threshold)!=1)
			{
				//TreeTimesShine(times);
				return;
			}
		}*/
		
	}
	else if((times==3)&&(keynum==1))
	{
		
		if(ReadThresholdFrom601(1,&threshold)==1) //ͨ��IIC��ȡоƬ5�ŵı궨����ֵ
		{
			
			StoreStd[2]=threshold;
			//Digout(2,1);  //12-24V��
			if(SetThresholdTo601(3,&threshold)==1)//�������ñ궨ֵ
			{
				printf("оƬ5�ű궨ֵ:%d \r\n",threshold);//
				StoreStd_Set(2,threshold);
				//Digout(2,0);  //12-24V��
			}
			else
			{
				return;
			}
		}
		else
		{
			
			return;
		}

		if(ReadThresholdFrom601(2,&threshold)==1) //ͨ��IIC��ȡоƬ4�ŵı궨����ֵ
		{
			
			StoreStd[3]=threshold;
			Digout(2,1);  //12-24V��
			if(SetThresholdTo601(4,&threshold)==1)//�������ñ궨ֵ
			{
				printf("оƬ4�ű궨ֵ:%d \r\n",threshold);//
				StoreStd_Set(3,threshold);
				//Digout(2,0);  //12-24V��
			}
			else
			{
				
				return;
			}
		}
		else
		{
			
			return;
		}


	}
		

		

}


void on_line_calibration(unsigned char times,unsigned int *StoreStd)
	{
		printf("�����·��궨ֵ \r\n");//
		//EEPROM_Read(StoreStd); 
		unsigned char i;
		for(i=1;i<5;i++)
		{
			if(SetThresholdTo601(i,&StoreStd[i-1])==1)
			{
				printf("�����·��궨ֵ:%d����%d \r\n",i,StoreStd[i-1]);//
			}
			else
			{
				printf("����ʧ��ERROR \r\n");//
				return;
			}
		}
	}




