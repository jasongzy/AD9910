 /*-----------------------------------------------
  名称：AD9854并口驱动
  编写：Liu
  日期：2014.3
  修改：无
  内容：
------------------------------------------------*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include "stm32f10x.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int						


#define AD9910_PWR 	PBout(5)	 
#define AD9910_SDIO PBout(6)    
#define DRHOLD 			PBout(7)   
#define DROVER 			PBout(8)   
#define UP_DAT 			PBout(9)    
#define PROFILE1 		PBout(12) 

#define MAS_REST 		PAout(2)  
#define SCLK 				PAout(3)     
#define DRCTL  			PAout(4)   
#define OSK 				PAout(5)       
#define PROFILE0 		PAout(6)  
#define PROFILE2 		PAout(7)  
#define CS  				PAout(8)      


void AD9110_IOInit(void);
void Init_ad9910(void);
void Freq_convert(ulong Freq);										//写频率

//void Write_Amplitude(bit Channel,unsigned int Amplitude);	//写幅度

void txd_8bit(uchar txdat);
void Txcfr(void);

void Write_Amplitude(uint Amp); //写幅度，输入范围：1-650 mV

//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：ns，范围：4*(1~65536)ns）
void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime);

void Square_wave(uint Sample_interval);//方波，采样时间间隔输入范围：4*(1~65536)ns

#endif


