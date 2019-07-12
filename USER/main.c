/**********************************************************
                       康威电子
										 
功能：stm32f103rct6控制
			显示：12864
接口：按键接口请参照key.h
时间：2015/11/3
版本：1.0
作者：康威电子
其他：程序借鉴正点原子，添加自己的驱动，未经作者许可，不得用于其它任何用途

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "stm32_config.h"
#include "AD9910.h"

extern uchar cfr2[4]; //cfr2控制字
extern uchar cfr1[4]; //cfr1控制字

char str[30];	//显示缓存
extern u8 _return;
int main(void)
{
	u16 i=0;
	unsigned long int freq = 1000000;

	delay_init(72);	//初始化延时函数
	
	
	Init_ad9910();
	Freq_convert(30000000);
	Write_Amplitude(600); //写幅度，输入范围：1-650 mV
	

	while(1)
	{		
		Freq_convert(freq);
		freq = freq + 1000000;
		if(freq > 40000000)
		{
			freq = 1000000;
		}
		delay_ms(200);
	
	}	
}


/*

if(PAin(0) == 0)
		{
			delay_ms(5);
			cfr1[0] = 0x00; //RAM 失能
			cfr2[1] = 0x00; //DRG 失能
			Txcfr(); //发送cfrx控制字
			Write_Amplitude(500); //写幅度，输入范围：1-650 mV
			Freq_convert(100000); //写频率，输入范围：1-400 000 000Hz
		}
		if(PAin(8) == 0)
		{
			delay_ms(5);
			cfr1[0] = 0x00; //RAM 失能
			cfr2[1]=0x0e; //DRG 使能
			Txcfr(); //发送cfrx控制字
			Write_Amplitude(500); //写幅度，输入范围：1-650 mV
			//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：us）
			SweepFre(100, 100000, 10, 240000); //步进时间范围：4*(1~65536)ns
		}
		
		if(PDin(3) == 0)
		{
			delay_ms(5);
			Square_wave(200); //方波，采样时间间隔输入范围：4*(1~65536)ns
			cfr1[0] = 0xc0; //RAM 使能，幅度控制
			cfr2[1] = 0x00; //DRG 失能
			Txcfr(); //发送cfrx控制字
		}
		
*/

