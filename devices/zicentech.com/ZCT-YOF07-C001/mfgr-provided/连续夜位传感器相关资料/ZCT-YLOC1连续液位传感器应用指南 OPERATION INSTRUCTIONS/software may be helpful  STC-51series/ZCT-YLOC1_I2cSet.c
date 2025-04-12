#include "ZS618_I2cSet.h"
#include "define.h"
#include "uart.h"

#define START_INDEX 0x0DA0
unsigned int val[4]={0x2000,0x4000,0x6000,0x8000};
unsigned char buffer[4];

//***************************************************************************  
void i2c_delay()//简单延时函数  
{ 
	char i;
	for(i=0;i<2;i++); }  
//***************************************************************************  
void i2c_start()  //开始信号 SCL在高电平期间，SDA一个下降沿则表示启动信号  
{     
    SDA=1; //释放SDA总线  
    i2c_delay();  
    SCL=1;  
    i2c_delay();  
    SDA=0;  
    i2c_delay();  
	SCL=0;
    i2c_delay(); 
}  
//***************************************************************************  
void i2c_stop()   //停止 SCL在高电平期间，SDA一个上升沿则表示停止信号  
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
bit i2c_checkack()  //应答 SCL在高电平期间，SDA被从设备拉为低电平表示应答  
{  
	bit	bit_temp;  
    SCL=1;  
    bit_temp=SDA;
	i2c_delay();  
	if(bit_temp)
		//无ACK回应
		return 0;
    SCL=0;  
	return 1;  
}  
//***************************************************************************  
void i2c_sendack()  //应答 SCL在高电平期间，SDA被从设备拉为低电平表示应答  
{  

	SDA=0;
	i2c_delay();
	SCL=1;
	i2c_delay();
	SCL=0;
	SDA=1;

}  
//***************************************************************************  
void i2c_sendnack()  //应答 SCL在高电平期间，SDA被从设备拉为低电平表示应答  
{  

	SDA=1;
	i2c_delay();
	SCL=1;
	i2c_delay();
	SCL=0;
	
}
//***************************************************************************  
void i2c_write_byte(unsigned char date) //写一个字节  
{  
    unsigned char i,temp;  
    temp=date;  
  
  
    for(i=0;i<8;i++)  
    {  
        temp=temp<<1;  
        SDA=CY;  
        i2c_delay();  
        SCL=1;//拉高SCL，此时SDA上的数据稳定  
        i2c_delay();  
		SCL=0;//拉低SCL，因为只有在时钟信号为低电平期间按数据线上的高低电平状态才允许变化；并在此时和上一个循环的scl=1一起形成一个上升沿  
    }  
    SDA=1;//释放SDA总线，接下来由从设备控制，比如从设备接收完数据后，在SCL为高时，拉低SDA作为应答信号  
    i2c_delay();  
}  
//***************************************************************************  
unsigned char i2c_read_byte()//读一个字节  
{  
    unsigned char i,k;  
 
    for(i=0;i<8;i++)  
    {
		i2c_delay();  
        SCL=1;//上升沿时，IIC设备将数据放在sda线上，并在高电平期间数据已经稳定，可以接收啦   
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
    i2c_write_byte((dev_addr<<1));//发送发送从设备地址 写操作 
    if(~i2c_checkack())
		{
		  return 0;
		}
	i2c_delay();  		 
    i2c_write_byte(reg_addr);//发送发送寄存器地址 写操作 
    if(~i2c_checkack())
		{
		return 0; 
		}
	i2c_delay();  
	for(i=0;i<len;i++)
	{
	    i2c_write_byte(src_buf[i]);//发送缓冲区数据 写操作 
	    i2c_checkack();//等待从设备的响应  
		i2c_delay();  				
	}
    i2c_stop();//停止 
	return 1;
}
//***************************************************************************  
bit	i2c_read_direct(unsigned dev_addr,unsigned char * dest_buf,unsigned char len)
{
	char i;
    i2c_start();//启动  
    i2c_write_byte((dev_addr<<1)+1);//发送发送从设备地址 读操作  
    if(~i2c_checkack())
		return 0;//等待从设备的响应  
	i2c_delay();
	dest_buf[0]=i2c_read_byte();//获取数据 
	for(i=1;i<len;i++)
	{
		i2c_sendack();
		i2c_delay();
		dest_buf[i]=i2c_read_byte();
	}
	i2c_sendnack();
	i2c_delay();
    i2c_stop();//停止  
	return 1;
}
//***************************************************************************  
/*
功能：33MHz频率下简单延时500us
输入：无
输出：无
*/
void Delay500us() 
{ 
	unsigned int i;
	for(i=0;i<800;i++);
	for(i=0;i<800;i++);
} 
//***************************************************************************  
/*
功能：读取出当前618的采样值
输入：chNum为需要读取采样值的通道号，
			sample为读取出采样值后需要存至的变量，
输出：1表示读取成功0表示读取失败
*/
bit ReadThresholdFrom618(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j,chnum;
	unsigned char CON_RESA2EEP=0xCA;
	bit flag=0;
	if(i2c_write(CON_ADDR, 0x01,&CON_RESA2EEP, 1)==0)//进行单片机与水位芯片握手操作
	{
		return 0;
	}
	for(i=0;i<20;i++)
	{
		if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//轮询水位芯片寄存器中的数据，判断单片机和水位芯片是否握手成功
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
		if(i2c_write(CON_ADDR, 0x01,&chnum, 1)==0)//握手成功后，发送水位芯片采样的通道编号加上0x30
			return 0;
		for(j=0;j<20;j++)
		{
			if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//轮询水位芯片寄存器，判断是否操作成功
				return 0;
			if(buffer[0]==chNum)//判断水位芯片操作成功后，将得到的采样值赋值给指针
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
功能：将当前需要设置的采样值发送至618中
输入：chnelnum为需要设置采样值的通道号，
			sample为设置至618的采样值变量，
输出：1表示设置成功0表示设置失败
*/
bit SetThresholdTo618(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j,chnum;
	unsigned char setsampl[2];
	unsigned char CON_SET618CR=0x8C;
	bit flag=0;
	if(i2c_write(CON_ADDR, 0x01,&CON_SET618CR, 1)==0)//进行单片机与水位芯片握手操作
		return 0;
	for(i=0;i<20;i++)
	{
		if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//轮询水位芯片寄存器中的数据，判断单片机和水位芯片是否握手成功
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
	if(flag==1)//握手成功后，发送需要设置的水位芯片参数和通道编号加上0x30
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
			if(i2c_read_direct(CON_ADDR,&buffer,0x03)==0)//轮询水位芯片寄存器，判断是否操作成功
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
	VPP_CTRL=CON_VPP_OFF;			//切断VPP电源
	VDD_CTRL=CON_VDD_OFF;		 	//切断VDD电源
	SDA = 0;
	SCL = 0;						//SDA和SCL断电
	Delay(5);
	VDD_CTRL=CON_VDD_ON;			//接通VDD电源
	Delay(1);
	SDA = 1;
	SCL = 1;						//SDA和SCL恢复
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
	//本函数读取内容，由buffer[1]buffer[0]返回
	if(addr!=(addr&0xfff))	//地址超范围
	{
		AddToUartBuf(0xe4);
		UartSendOver();	
		return 0;		
	}
	//送单字节读命令
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
	//读取内容	
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
	if(addr!=(addr&0xfff))	//地址超范围
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