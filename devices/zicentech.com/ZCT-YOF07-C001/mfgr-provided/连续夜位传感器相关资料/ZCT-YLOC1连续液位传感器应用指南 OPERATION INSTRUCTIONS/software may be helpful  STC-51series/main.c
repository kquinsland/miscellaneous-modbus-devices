#include "uart.h"
#include "define.h"
#include "leddisplay.h"
#include "eeprom.h"
#include "key.h"
#include "ZS618_I2cSet.h"
#include "usual.h"

bit busy;												//串口数据繁忙标志
bit	timer_flag=0;								//定时器溢出标志
bit led_flag=0;									//定时器溢出8次标志

unsigned char WaterCor;					//水位信息
unsigned char Buffer1[4];				//存储从ZS618中iic4个寄存器中读取的数据

unsigned int ShowTimer=0;						//数码管打印一次的时间间隔
extern idata unsigned char Display[4];	//数码管打印的数据
unsigned char KeyNum,Times;

void main()
{
	SystemInit();							//单片机初始化
	P1M0=0XFF;
	PEN=1;
	while(1)
	{
		if(timer_flag!=0 )			//定时器0,每0.7ms进行一次水位芯片iic寄存器读取操作
		{
			timer_flag=0;
			i2c_read_direct(CON_ADDR,&Buffer1,0x04);		//从上次写寄存器的地址开始读,如上次写寄存器1，这次读取就从
																								//寄存器1开始读取到寄存器3，如上次写寄存器0，这次就从寄存器
																								//0开始读取到寄存器3
			WaterCor=Buffer1[0];						//寄存器0存放水位信息
		}
		
		if(led_flag!=0)			//每8*0.7ms进行一次按键判断
		{
			led_flag=0;
			
			if(GetKeyAll(&Times,&KeyNum))	//得到按下按键的编号和按下的次数
			{
				if((Times==5)&&(KeyNum==2))
					PrintMCU();
				if((Times==6)&&(KeyNum==2))
					write(0x0da4,0x2aaa);
				SetOrRead(Times,KeyNum);		//根据按键编号和次数进行响应的操作
				i2c_write(CON_ADDR, 0x00,&Times, 0);	//写一次寄存器0，方便下一次自动从寄存器0开始读
			}
			
			if(ShowTimer==100)	//数码管打印水位信息
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

