////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// All rights reserved.
//
// Filename     ：Driver_UART.c
// Version      ：1.0
// Author       ：zhangjun
// Accomplished date ： 2016.7.1
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#ifndef LIB_DRIVER_UART
#define LIB_DRIVER_UART
#endif

#include <App_includes.h>
#include <Driver_includes.h>
#include "Driver_IIC.h"
#include "App_parameter.h"

//IIC初始化

#define NACK 0
#define ACK  1
#define CON_ADDR 0X40
#define START_INDEX 0x0DA0
unsigned int val[4]={0x0000,0x4000,0x6000,0x8000};
u8 buffer[4];

//extern CONFIG_PARAM ConfigPara;                          //配置参数

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    //PH4,5初始化设置
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    IIC_SDA=1;
    IIC_SCL=1;  

	
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	//delay_us(4);
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	//delay_us(4);
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号	
    delay_us(4);	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	  

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
     //   IIC_NAck();//发送nACK
  //  else
     //   IIC_Ack(); //发送ACK   
    return receive;
}
	/*u8 IIC_Read_Byte(unsigned char ack)
	{
		unsigned char i,receive=0;
		SDA_IN();//SDA设置为输入
		for(i=0;i<8;i++ )
		{
			IIC_SCL=1; 
			delay_us(2);
			receive<<=1;
			if(READ_SDA)receive++;	 
			IIC_SCL=0;
		}					 
		if (!ack)
			IIC_NAck();//发送nACK
		else
			IIC_Ack(); //发送ACK	 
		return receive;
	}*/







/*u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
  IIC_Send_Byte(ReadAddr<<1+1));   //发送器件地址0XA0,写数据 	   
 // IIC_Send_Byte(ReadAddr<<1);   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0X81);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}*/

unsigned char	AT24CXX_ReadOneByte(u16 dev_addr,unsigned char *dest_buf,u16 len)
{
	char i;
    IIC_Start();//启动  
    IIC_Send_Byte((dev_addr<<1)+1);//发送发送从设备地址 读操作  
   IIC_Wait_Ack();  
	delay_us(4);
	dest_buf[0]=IIC_Read_Byte(0);//获取数据 
	for(i=1;i<len;i++)
	{
		IIC_Ack();
		delay_us(4);
		dest_buf[i]=IIC_Read_Byte(0);
	}
	IIC_NAck();
	delay_us(4);
    IIC_Stop();//停止  
	return 1;
}


//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
//void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
//{
//	while(NumToRead)
//	{
//		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
//		NumToRead--;
//	}
//}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
/*void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(0X40+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_us(10);	 
}*/
u8	AT24CXX_WriteOneByte(u16 dev_addr, u16 reg_addr, unsigned char * src_buf, u8 len)
{
	char i;
	IIC_Start();
    IIC_Send_Byte((dev_addr<<1));//发送发送从设备地址 写操作 
    if(IIC_Wait_Ack()==1)
    	{
    	return 0; 
    	}
	delay_us(4);  		 
    IIC_Send_Byte(reg_addr);//发送发送寄存器地址 写操作 
    if(IIC_Wait_Ack()==1)
    	{
    	return 0; 
    	}
	delay_us(4);  
	for(i=0;i<len;i++)
	{
	    IIC_Send_Byte(src_buf[i]);//发送缓冲区数据 写操作 
	    IIC_Wait_Ack();//等待从设备的响应  
		delay_us(4);  				
	}
    IIC_Stop();//停止 
	return 1;
}

u8 ReadThresholdFrom601(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j=0,chnum;
	unsigned char buffer[4];
	unsigned char CON_RESA2EEP=0xCA;
	unsigned char buffer2[4];
	u8 flag=0;
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x01,&CON_RESA2EEP, 1)==0)//进行单片机与水位芯片握手操作
	{
		return 0;
	}
	for(i=0;i<20;i++)
	{
		if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//轮询水位芯片寄存器中的数据，判断单片机和水位芯片是否握手成功
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
		if(AT24CXX_WriteOneByte(CON_ADDR, 0x01,buffer2, 3)==0)//握手成功后，发送水位芯片采样的通道编号加上0x30
			return 0;
		for(j=0;j<200;j++)
		{
			if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//轮询水位芯片寄存器，判断是否操作成功
				return 0;
			if(buffer[0]==chNum)//判断水位芯片操作成功后，将得到的采样值赋值给指针
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
	ResetMCUPower();  //开启电源
	
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
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x01,&CON_SET601CR, 1)==0)//进行单片机与水位芯片握手操作
		return 0;
	for(i=0;i<20;i++)
	{
		if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//轮询水位芯片寄存器中的数据，判断单片机和水位芯片是否握手成功
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
	if(flag==1)//握手成功后，发送需要设置的水位芯片参数和通道编号加上0x30
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
			if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x03)==0)//轮询水位芯片寄存器，判断是否操作成功
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
	Digout(2,0);  //12-24V关
	Digout(0,0);  //3.3V关
	IIC_SDA=0;	  	  
	IIC_SCL=0;						//SDA和SCL断电
	delay_us(5);
	Digout(0,1);  //3.3V开
	delay_us(1);
	IIC_SDA=1;	  	  
	IIC_SCL=1;						//SDA和SCL恢复
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
	//本函数读取内容，由buffer[1]buffer[0]返回
	if(addr!=(addr&0xfff))	//地址超范围
	{
		return 0;		
	}
	//送单字节读命令
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
	if(addr!=(addr&0xfff))	//地址超范围
	{
		
		return 0;		
	}
	
	buffer[3] = 0xE0 | (*((char * )(&addr))&0x1f);
	buffer[2] = addr;
	buffer[1] = worddata;
	buffer[0] = *((char * )(&worddata));

		
	Digout(2,1);  //12-24V开
	if(AT24CXX_WriteOneByte(CON_ADDR, 0x00,buffer, 4)==0)
	{
		
		return 0;
	}		
	delay_us(20);
	if(AT24CXX_ReadOneByte(CON_ADDR,buffer,0x02)==0)
	{
		
		return 0;
	}
	Digout(2,0);  //12-24V关

	return 1;
}





void SetOrRead(unsigned char times,unsigned char keynum)
{
	unsigned int threshold;
	unsigned int StoreStd[4];	
	unsigned char i;
	if(((times==1)||(times==2))&&(keynum==1))
	{
		//设置标定值有两种方法，一种在线标定，一种烧录至601ROM中掉电不丢失，程序中两种方法都有，如果自己开发设置板程序，以在线标定为参考
		//如果用我们设置板标定，就不需要自己开发标定程序
		
		if(ReadThresholdFrom601(times,&threshold)==1)//按键一次通过IIC获取芯片5脚的标定采样值，按键两次通过IIC获取芯片4脚的标定采样值
		{
			
			StoreStd[times-1]=threshold;
			Digout(2,1);  //12-24V开
			
			if(SetThresholdTo601(times,&threshold)==1)//在线设置标定值
			{
				printf("低水位芯片4-5脚标定值:%d \r\n",threshold);//
				
				StoreStd_Set(times-1,threshold);
				Digout(2,0);  //12-24V关
			}
			
		}
		else
		{
			return;
		}
		/*if(dealprg(times,threshold)!=1)//将标定值烧录至618ROM中，掉电不丢失，重新上电不需要再标定
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
		
		if(ReadThresholdFrom601(1,&threshold)==1) //通过IIC获取芯片5脚的标定采样值
		{
			
			StoreStd[2]=threshold;
			//Digout(2,1);  //12-24V开
			if(SetThresholdTo601(3,&threshold)==1)//在线设置标定值
			{
				printf("芯片5脚标定值:%d \r\n",threshold);//
				StoreStd_Set(2,threshold);
				//Digout(2,0);  //12-24V关
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

		if(ReadThresholdFrom601(2,&threshold)==1) //通过IIC获取芯片4脚的标定采样值
		{
			
			StoreStd[3]=threshold;
			Digout(2,1);  //12-24V开
			if(SetThresholdTo601(4,&threshold)==1)//在线设置标定值
			{
				printf("芯片4脚标定值:%d \r\n",threshold);//
				StoreStd_Set(3,threshold);
				//Digout(2,0);  //12-24V关
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
		printf("在线下发标定值 \r\n");//
		//EEPROM_Read(StoreStd); 
		unsigned char i;
		for(i=1;i<5;i++)
		{
			if(SetThresholdTo601(i,&StoreStd[i-1])==1)
			{
				printf("在线下发标定值:%d，，%d \r\n",i,StoreStd[i-1]);//
			}
			else
			{
				printf("设置失败ERROR \r\n");//
				return;
			}
		}
	}




