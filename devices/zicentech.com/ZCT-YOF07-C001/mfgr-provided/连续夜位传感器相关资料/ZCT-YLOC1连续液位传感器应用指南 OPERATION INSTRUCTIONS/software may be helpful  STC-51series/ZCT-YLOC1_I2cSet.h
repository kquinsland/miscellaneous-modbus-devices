#ifndef __ZS618_I2cSet_H_
#define __ZS618_I2cSet_H_

#include "STC15W408S.h"

#define SDA			P34
#define SCL			P33
#define CON_ADDR      0x40

bit	i2c_read_direct(unsigned dev_addr,unsigned char * dest_buf,unsigned char len);
bit	i2c_write(unsigned dev_addr, unsigned reg_addr, unsigned char * src_buf, unsigned char len);

bit ReadThresholdFrom618(unsigned char chnelnum,unsigned int *sample);
bit SetThresholdTo618(unsigned char chnelNum,unsigned int *sample);
bit PrintMCU();
bit dealprg(unsigned char keytime,unsigned int onedata);
bit write(unsigned int addr1,unsigned int data1);
#endif