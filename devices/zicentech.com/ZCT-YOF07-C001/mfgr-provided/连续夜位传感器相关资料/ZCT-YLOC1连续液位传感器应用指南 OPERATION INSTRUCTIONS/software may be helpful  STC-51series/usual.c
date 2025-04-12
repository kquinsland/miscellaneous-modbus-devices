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
		//���ñ궨ֵ�����ַ�����һ�����߱궨��һ����¼��618ROM�е��粻��ʧ�����������ַ������У�����Լ��������ð���������߱궨Ϊ�ο�
		//������������ð�궨���Ͳ���Ҫ�Լ������궨����
		TreeTimesShine(times);
		if(ReadThresholdFrom618(times,&threshold)==1)//����һ��ͨ��IIC��ȡоƬ5�ŵı궨����ֵ����������ͨ��IIC��ȡоƬ4�ŵı궨����ֵ
		{
			EEPROM_Read(StoreStd);
			EEPROM_Eraser();
			StoreStd[times-1]=threshold;
			EEPROM_Write(StoreStd);	//���궨����ֵд�뱾���ð�ROM��
			
			if(SetThresholdTo618(times,&threshold)==1)//�������ñ궨ֵ
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
		if(dealprg(times,threshold)!=1)//���궨ֵ��¼��618ROM�У����粻��ʧ�������ϵ粻��Ҫ�ٱ궨
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
		if(ReadThresholdFrom618(1,&threshold)==1) //ͨ��IIC��ȡоƬ5�ŵı궨����ֵ
		{
			EEPROM_Read(StoreStd);
			EEPROM_Eraser();
			StoreStd[2]=threshold;
			EEPROM_Write(StoreStd);	//���궨����ֵд�뱾���ð�ROM��
			if(SetThresholdTo618(3,&threshold)==1)//�������ñ궨ֵ
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

		if(ReadThresholdFrom618(2,&threshold)==1) //ͨ��IIC��ȡоƬ4�ŵı궨����ֵ
		{
			EEPROM_Read(StoreStd);
			EEPROM_Eraser();
			StoreStd[3]=threshold;
			EEPROM_Write(StoreStd);	//���궨����ֵд�뱾���ð�ROM��
			if(SetThresholdTo618(4,&threshold)==1)//�������ñ궨ֵ
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
		if(dealprg2(3,StoreStd[2],StoreStd[3])!=1)//���궨ֵ��¼��618ROM�У����粻��ʧ�������ϵ粻��Ҫ�ٱ궨
		{
			if(dealprg2(3,StoreStd[2],StoreStd[3])!=1)
			{
				TreeTimesShine(times);
				return;
			}
		}

	}
		

		
	
	else if((times==1)&&(keynum==2))//�����ϵ�֮��Ŀ������߱궨�����洢��rom�еı궨ֵ�·�
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

	else if((times==4)&&(keynum==1)) //����1�İ�������Ϊ4ʱ����0��1��2ͨ������
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
