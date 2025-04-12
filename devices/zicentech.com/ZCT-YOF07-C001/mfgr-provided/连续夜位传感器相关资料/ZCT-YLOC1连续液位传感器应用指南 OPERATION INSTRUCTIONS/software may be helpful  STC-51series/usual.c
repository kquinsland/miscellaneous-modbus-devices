#include "usual.h"
#include "lib.h"
#include "ZS618_I2cSet.h"
//***************************************************************************  

//***************************************************************************  
void SetOrRead(unsigned char times,unsigned char keynum)
{
	unsigned int threshold;
	unsigned int StoreStd[4];	
	unsigned char i;
	if(((times==1)||(times==2))&&(keynum==1))
	{
		//设置标定值有两种方法，一种在线标定，一种烧录至618ROM中掉电不丢失，程序中两种方法都有，如果自己开发设置板程序，以在线标定为参考
		//如果用我们设置板标定，就不需要自己开发标定程序
		TreeTimesShine(times);
		if(ReadThresholdFrom618(times,&threshold)==1)//按键一次通过IIC获取芯片5脚的标定采样值，按键两次通过IIC获取芯片4脚的标定采样值
		{
			EEPROM_Read(StoreStd);
			EEPROM_Eraser();
			StoreStd[times-1]=threshold;
			EEPROM_Write(StoreStd);	//将标定采样值写入本设置板ROM中
			
			if(SetThresholdTo618(times,&threshold)==1)//在线设置标定值
			{
				AddToUartBuf(StoreStd[times-1]>>8);
				AddToUartBuf(StoreStd[times-1]);
				UartSendOver();
			}
			else
			{
				TreeTimesShine(times);
				return;
			}
		}
		else
		{
			TreeTimesShine(times);
			return;
		}
		//
		if(dealprg(times,threshold)!=1)//将标定值烧录至618ROM中，掉电不丢失，重新上电不需要再标定
		{
			if(dealprg(times,threshold)!=1)
			{
				TreeTimesShine(times);
				return;
			}
		}
	}
	else if((times==3)&&(keynum==1))
	{
		TreeTimesShine(times);
		if(ReadThresholdFrom618(1,&threshold)==1) //通过IIC获取芯片5脚的标定采样值
		{
			EEPROM_Read(StoreStd);
			EEPROM_Eraser();
			StoreStd[2]=threshold;
			EEPROM_Write(StoreStd);	//将标定采样值写入本设置板ROM中
			if(SetThresholdTo618(3,&threshold)==1)//在线设置标定值
			{
				AddToUartBuf(StoreStd[2]>>8);
				AddToUartBuf(StoreStd[2]);
				UartSendOver();
			}
			else
			{
				TreeTimesShine(times);
				return;
			}
		}
		else
		{
			TreeTimesShine(times);
			return;
		}

		if(ReadThresholdFrom618(2,&threshold)==1) //通过IIC获取芯片4脚的标定采样值
		{
			EEPROM_Read(StoreStd);
			EEPROM_Eraser();
			StoreStd[3]=threshold;
			EEPROM_Write(StoreStd);	//将标定采样值写入本设置板ROM中
			if(SetThresholdTo618(4,&threshold)==1)//在线设置标定值
			{
				AddToUartBuf(StoreStd[3]>>8);
				AddToUartBuf(StoreStd[3]);
				UartSendOver();
			}
			else
			{
				TreeTimesShine(times);
				return;
			}
		}
		else
		{
			TreeTimesShine(times);
			return;
		}
		if(dealprg2(3,StoreStd[2],StoreStd[3])!=1)//将标定值烧录至618ROM中，掉电不丢失，重新上电不需要再标定
		{
			if(dealprg2(3,StoreStd[2],StoreStd[3])!=1)
			{
				TreeTimesShine(times);
				return;
			}
		}

	}
		

		
	
	else if((times==1)&&(keynum==2))//重新上电之后的快速在线标定，将存储在rom中的标定值下发
	{
		TreeTimesShine(5);
		EEPROM_Read(StoreStd); 
		for(i=1;i<5;i++)
		{
			if(SetThresholdTo618(i,&StoreStd[i-1])==1)
			{
				AddToUartBuf(StoreStd[i-1]>>8);
				AddToUartBuf(StoreStd[i-1]);
				UartSendOver();
			}
			else
			{
				TreeTimesShine(5);
				return;
			}
		}
	}

	else if((times==4)&&(keynum==1)) //按键1的按键次数为4时进行0、1、2通道采样
	{
		TreeTimesShine(times);
		for(i=0;i<3;i++)
		{
			if(ReadThresholdFrom618(i,&threshold)==1)
			{
				AddToUartBuf(threshold>>8);
				AddToUartBuf(threshold);
				UartSendOver();
			}
			else
			{
				TreeTimesShine(times);
				return;
			}
		}

	}
	else
		return;
}
//*************************************************************************** 
