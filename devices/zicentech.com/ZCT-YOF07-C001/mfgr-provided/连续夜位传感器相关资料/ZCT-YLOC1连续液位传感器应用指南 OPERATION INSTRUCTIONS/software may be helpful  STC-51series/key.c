#include "lib.h"
/*----------------------------
key �������
-----------------------------*/
#define	KEY1	P21
#define	KEY2	P20

bit GetKeyAll(unsigned char *times,unsigned char *keynum);


bit	key1_r=0,key1=0; 				//����start ����
bit	key2_r=0,key2=0; 				//����eot ����
bit	key3_r=0,key3=0;
//***************************************************************************  
void delay2ms()
{
	unsigned int i;
	for(i=0;i<5000;i++);
}
//***************************************************************************  
bit	getkey1()
{
	if(KEY1==0 && key1==0 && key1_r==1)
	{
		key1_r=key1;
		key1=KEY1;
		return 1;
	}
	key1_r=key1;
	key1=KEY1;
	return 0;
}
//***************************************************************************  
bit	getkey2()
{
	if(KEY2==0 && key2==0 && key2_r==1)
	{
		key2_r=key2;
		key2=KEY2;
		return 1;
	}
	key2_r=key2;
	key2=KEY2;
	return 0;
}
//***************************************************************************  
bit GetKeyAll(unsigned char *times,unsigned char *keynum)
{
	unsigned char Key1_Times,Key2_Times;
	unsigned int i;
	if(getkey1())  //�жϰ����Ƿ���
	{
		Key1_Times=1;
		for(i=0;i<500;i++)//��ѯ�ĵ������Ĵ���
		{
			if(getkey1())
			{
				i=0;
				Key1_Times++;
			}
			delay2ms();
		}
		*times=Key1_Times;//���������Ͱ�����Ÿ�ֵ��ָ�����
		*keynum=1;

		return 1;
	}
	if(getkey2())
	{
		Key2_Times=1;
		for(i=0;i<500;i++)
		{
			if(getkey2())
			{
				i=0;
				Key2_Times++;
			}
			delay2ms();
		}
		*times=Key2_Times;
		*keynum=2;

		return 1;
	}
	*times=0;
	*keynum=0;
	return 0;
}