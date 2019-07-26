/*-----------------------------------------------
  名称：AD9854并口驱动
  编写：Liu
  日期：2014.3
  修改：无
  内容：
------------------------------------------------*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include "stm32f4xx.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long int

//新增IO
//#define SDO PAout(4)
//#define PDCLK PCout(6)//PDCLK
//#define PLL_LOCK PCout(7)//悬空
//#define SYNC_CLK PCout(9)//悬空
//#define TXENABLE PCout(11)//TXEN
//#define RSO PCout(12)//RAM-SWP-OVR
#define AD9910_PWR PCout(13) //0
//原始IO

#define AD9910_SDIO PAout(5)
#define DRHOLD PCout(1)
#define DROVER PCout(2)
#define UP_DAT PCout(3)
#define PROFILE1 PCout(10)
#define MAS_REST PAout(6) //
#define SCLK PAout(2)     //
#define DRCTL PAout(4)    //
#define OSK PCout(8)
#define PROFILE0 PCout(4)
#define PROFILE2 PCout(5)
#define CS PBout(10) //

//void Init_AD9910(void);  //INIT 9910
//void AD9910WriteByte (unsigned char commond, unsigned char *databyte,unsigned char lengh);
void AD9110_IOInit(void);
void Init_ad9910(void);
void Freq_convert(ulong Freq);            //写频率
void Amp_convert(unsigned int Amplitude); //写幅度
void Txcfr(void);
void Txramprofile(void);
void Txramdata(int wave_num);
void Square_wave(uint Sample_interval);                                                   //方波;
void Sawtooth_wave(uint Sample_interval);                                                 //三角波
void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime); //扫频

#endif
