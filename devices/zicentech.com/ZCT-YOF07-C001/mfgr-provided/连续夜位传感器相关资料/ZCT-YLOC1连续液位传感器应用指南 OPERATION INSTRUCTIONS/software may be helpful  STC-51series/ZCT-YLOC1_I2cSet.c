#include "ZS618_I2cSet.h"
#include "define.h"
#include "uart.h"

#define START_INDEX 0x0DA0
unsigned int val[4]={0x2000,0x4000,0x6000,0x8000};
unsigned char buffer[4];

//***************************************************************************  
void i2c_delay()//����ʱ����  
{ 
	char i;
	for(i=0;i<2;i++); }  
//***************************************************************************  
void i2c_start()  //��ʼ�ź� SCL�ڸߵ�ƽ�ڼ䣬SDAһ���½������ʾ�����ź�  
{     
    SDA=1; //�ͷ�SDA����  
    i2c_delay();  
    SCL=1;  
    i2c_delay();  
    SDA=0;  
    i2c_delay();  
	SCL=0;
    i2c_delay(); 
}  
//***************************************************************************  
void i2c_stop()   //ֹͣ SCL�ڸߵ�ƽ�ڼ䣬SDAһ�����������ʾֹͣ�ź�  
{  
    SCL=0;
    SDA=0;  
    i2c_delay();  
    SCL=1;  
    i2c_delay();  
    SDA=1;  
    i2c_delay();  
}
//***************************************************************************  
bit i2c_checkack()  //Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��  
{  
	bit	bit_temp;  
    SCL=1;  
    bit_temp=SDA;
	i2c_delay();  
	if(bit_temp)
		//��ACK��Ӧ
		return 0;
    SCL=0;  
	return 1;  
}  
//***************************************************************************  
void i2c_sendack()  //Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��  
{  

	SDA=0;
	i2c_delay();
	SCL=1;
	i2c_delay();
	SCL=0;
	SDA=1;

}  
//***************************************************************************  
void i2c_sendnack()  //Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��  
{  

	SDA=1;
	i2c_delay();
	SCL=1;
	i2c_delay();
	SCL=0;
	
}
//***************************************************************************  
void i2c_write_byte(unsigned char date) //дһ���ֽ�  
{  
    unsigned char i,temp;  
    temp=date;  
  
  
    for(i=0;i<8;i++)  
    {  
        temp=temp<<1;  
        SDA=CY;  
        i2c_delay();  
        SCL=1;//����SCL����ʱSDA�ϵ������ȶ�  
        i2c_delay();  
		SCL=0;//����SCL����Ϊֻ����ʱ���ź�Ϊ�͵�ƽ�ڼ䰴�������ϵĸߵ͵�ƽ״̬������仯�����ڴ�ʱ����һ��ѭ����scl=1һ���γ�һ��������  
    }  
    SDA=1;//�ͷ�SDA���ߣ��������ɴ��豸���ƣ�������豸���������ݺ���SCLΪ��ʱ������SDA��ΪӦ���ź�  
    i2c_delay();  
}  
//***************************************************************************  
unsigned char i2c_read_byte()//��һ���ֽ�  
{  
    unsigned char i,k;  
 
    for(i=0;i<8;i++)  
    {
		i2c_delay();  
        SCL=1;//������ʱ��IIC�豸�����ݷ���sda���ϣ����ڸߵ�ƽ�ڼ������Ѿ��ȶ������Խ�����   
		i2c_delay();			
        k=(k<<1)|SDA;  
		SCL=0;
    }  
    return k;  
}  
//***************************************************************************  
bit	i2c_write(unsigned dev_addr, unsigned reg_addr, unsigned char * src_buf, unsigned char len)
{
	char i;
	i2c_start();
    i2c_write_byte((dev_addr<<1));//���ͷ��ʹ��豸��ַ д���� 
    if(~i2c_checkack())
		{
		  return 0;
		}
	i2c_delay();  		 
    i2c_write_byte(reg_addr);//���ͷ��ͼĴ�����ַ д���� 
    if(~i2c_checkack())
		{
		return 0; 
		}
	i2c_delay();  
	for(i=0;i<len;i++)
	{
	    i2c_write_byte(src_buf[i]);//���ͻ��������� д���� 
	    i2c_checkack();//�ȴ����豸����Ӧ  
		i2c_delay();  				
	}
    i2c_stop();//ֹͣ 
	return 1;
}
//***************************************************************************  
bit	i2c_read_direct(unsigned dev_addr,unsigned char * dest_buf,unsigned char len)
{
	char i;
    i2c_start();//����  
    i2c_write_byte((dev_addr<<1)+1);//���ͷ��ʹ��豸��ַ ������  
    if(~i2c_checkack())
		return 0;//�ȴ����豸����Ӧ  
	i2c_delay();
	dest_buf[0]=i2c_read_byte();//��ȡ���� 
	for(i=1;i<len;i++)
	{
		i2c_sendack();
		i2c_delay();
		dest_buf[i]=i2c_read_byte();
	}
	i2c_sendnack();
	i2c_delay();
    i2c_stop();//ֹͣ  
	return 1;
}
//***************************************************************************  
/*
���ܣ�33MHzƵ���¼���ʱ500us
���룺��
�������
*/
void Delay500us() 
{ 
	unsigned int i;
	for(i=0;i<800;i++);
	for(i=0;i<800;i++);
} 
//***************************************************************************  
/*
���ܣ���ȡ����ǰ618�Ĳ���ֵ
���룺chNumΪ��Ҫ��ȡ����ֵ��ͨ���ţ�
			sampleΪ��ȡ������ֵ����Ҫ�����ı�����
�����1��ʾ��ȡ�ɹ�0��ʾ��ȡʧ��
*/
bit ReadThresholdFrom618(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j,chnum;
	unsigned char CON_RESA2EEP=0xCA;
	bit flag=0;
	if(i2c_write(CON_ADDR, 0x01,&CON_RESA2EEP, 1)==0)//���е�Ƭ����ˮλоƬ���ֲ���
	{
		return 0;
	}
	for(i=0;i<20;i++)
	{
		if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//��ѯˮλоƬ�Ĵ����е����ݣ��жϵ�Ƭ����ˮλоƬ�Ƿ����ֳɹ�
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
			Delay500us();
	}

	if(flag==1)
	{
		chnum=chNum+0x30;
		if(i2c_write(CON_ADDR, 0x01,&chnum, 1)==0)//���ֳɹ��󣬷���ˮλоƬ������ͨ����ż���0x30
			return 0;
		for(j=0;j<20;j++)
		{
			if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//��ѯˮλоƬ�Ĵ������ж��Ƿ�����ɹ�
				return 0;
			if(buffer[0]==chNum)//�ж�ˮλоƬ�����ɹ��󣬽��õ��Ĳ���ֵ��ֵ��ָ��
			{
				*sample=buffer[1]*256+buffer[2];
				break;
			}
			else if(j>=19)
			{
				return 0;
			}
			else
				Delay500us();
		}
		return 1;
	}
	else
		return 0;
}
//***************************************************************************  
/*
���ܣ�����ǰ��Ҫ���õĲ���ֵ������618��
���룺chnelnumΪ��Ҫ���ò���ֵ��ͨ���ţ�
			sampleΪ������618�Ĳ���ֵ������
�����1��ʾ���óɹ�0��ʾ����ʧ��
*/
bit SetThresholdTo618(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j,chnum;
	unsigned char setsampl[2];
	unsigned char CON_SET618CR=0x8C;
	bit flag=0;
	if(i2c_write(CON_ADDR, 0x01,&CON_SET618CR, 1)==0)//���е�Ƭ����ˮλоƬ���ֲ���
		return 0;
	for(i=0;i<20;i++)
	{
		if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//��ѯˮλоƬ�Ĵ����е����ݣ��жϵ�Ƭ����ˮλоƬ�Ƿ����ֳɹ�
			return 0;	
		if(buffer[0]==0xCD)
		{
			flag=1;
			break;
		}
		else if(j>=19)
			return 0;
		else
			Delay500us();
	}
	if(flag==1)//���ֳɹ��󣬷�����Ҫ���õ�ˮλоƬ������ͨ����ż���0x30
	{
		chnum=chNum+0x30;
		setsampl[0]=(*sample)/256;
		setsampl[1]=(*sample)%256;
		if(i2c_write(CON_ADDR, 0x02, setsampl, 2)==0)	
			return 0;	
		if(i2c_write(CON_ADDR, 0x01, &chnum, 1)==0)			
			return 0;	
		for(j=0;j<20;j++)
		{
			if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//��ѯˮλоƬ�Ĵ������ж��Ƿ�����ɹ�
				return 0;	
			if(buffer[0]==chNum)
				break;
			else if(j>=19)				
				return 0;	
			else
				Delay500us();
		}
		return 1;
	}
	else
		return 0;
}

void Delay(unsigned char n)
{
    unsigned int x;

    while (n--)
    {
        x = 0;
        while (++x);
    }
}

void ResetMCUPower()
{
	VPP_CTRL=CON_VPP_OFF;			//�ж�VPP��Դ
	VDD_CTRL=CON_VDD_OFF;		 	//�ж�VDD��Դ
	SDA = 0;
	SCL = 0;						//SDA��SCL�ϵ�
	Delay(5);
	VDD_CTRL=CON_VDD_ON;			//��ͨVDD��Դ
	Delay(1);
	SDA = 1;
	SCL = 1;						//SDA��SCL�ָ�
}
void StartPGM()
{
	buffer[0] = 0x58;
	buffer[1] = 0x58;
	if(~i2c_write(0x40, 0x00, &buffer, 2))
	{
		AddToUartBuf(0xe0);
		UartSendOver();	
	}
}
bit ReadSingleWordFromMCU(unsigned int addr)
{
	//��������ȡ���ݣ���buffer[1]buffer[0]����
	if(addr!=(addr&0xfff))	//��ַ����Χ
	{
		AddToUartBuf(0xe4);
		UartSendOver();	
		return 0;		
	}
	//�͵��ֽڶ�����
	buffer[3] = 0x60 | (*((char * )(&addr))&0x1f);
	buffer[2] = addr;
	buffer[1] = 0;
	buffer[0] = 0;	
	if(~i2c_write(0x40, 0x00, &buffer, 4))
	{
		AddToUartBuf(0xe4);
		UartSendOver();	
		return 0;
	}
	//��ȡ����	
//	if(~i2c_read_direct(0x40, &buffer ,2))
//	{
//		AddToUartBuf(0xe3);
//		UartSendOver();	
//		return 0;
//	}
//	AddToUartBuf(buffer[1]);
//	UartSendOver();	
//	AddToUartBuf(buffer[0]);
//	UartSendOver();	

	return	1;
		
}
void delay20us()
{
	unsigned char i;
	for(i=0;i<180;i++);
}
bit WriteSingleWordToMCU(unsigned int addr,unsigned int worddata)
{
	if(addr!=(addr&0xfff))	//��ַ����Χ
	{
		AddToUartBuf(0xe3);
		UartSendOver();	
		return 0;		
	}
	
	buffer[3] = 0xE0 | (*((char * )(&addr))&0x1f);
	buffer[2] = addr;
	buffer[1] = worddata;
	buffer[0] = *((char * )(&worddata));

		AddToUartBuf(buffer[3]);
		UartSendOver();	
		AddToUartBuf(buffer[2]);
		UartSendOver();	
		AddToUartBuf(buffer[0]);
		UartSendOver();	
		AddToUartBuf(buffer[1]);
		UartSendOver();	
	VPP_CTRL = CON_VPP_ON;
	if(~i2c_write(0x40, 0x00, &buffer, 4))
	{
		AddToUartBuf(0xe3);
		UartSendOver();	
		return 0;
	}		
	delay20us();
	if(~i2c_read_direct(0x40, &buffer ,2))
	{
		AddToUartBuf(0xe3);
		UartSendOver();	
		return 0;
	}
	VPP_CTRL = CON_VPP_OFF;
	return 1;
}

bit dealprg(unsigned char keytime,unsigned int onedata)
{
	unsigned int i;
	unsigned int oneword,oneaddr;
	ResetMCUPower();
	StartPGM();
	if(ReadSingleWordFromMCU(START_INDEX)!=1)
	{
		AddToUartBuf(0xe3);
		UartSendOver();	
		return 0;
	}
	for(i=0;i<1000;i++)
	{
		if(~i2c_read_direct(0x40, &buffer ,2))
		{
			AddToUartBuf(0xe3);
			UartSendOver();	
			return 0;
		}
		AddToUartBuf(buffer[0]);
		UartSendOver();	
		AddToUartBuf(buffer[1]);
		UartSendOver();	
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
bit PrintMCU()
{

	unsigned int i;
	ResetMCUPower();
	StartPGM();
	if(ReadSingleWordFromMCU(START_INDEX)!=1)
	{
		AddToUartBuf(0xe3);
		UartSendOver();	
		return 0;
	}
	for(i=0;i<1000;i++)
	{
		if(~i2c_read_direct(0x40, &buffer ,2))
		{
			AddToUartBuf(0xe3);
			UartSendOver();	
			return 0;
		}
		AddToUartBuf(buffer[0]);
		UartSendOver();	
		AddToUartBuf(buffer[1]);
		UartSendOver();	
	}
	ResetMCUPower();
	return 1;			
}
bit write(unsigned int addr1,unsigned int data1)
{
	ResetMCUPower();	
	StartPGM();
	if(WriteSingleWordToMCU(addr1,data1)!=1)
	{
		
		return 0;
	}
		
	return 1;
}