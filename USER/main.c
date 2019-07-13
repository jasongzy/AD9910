#include "stm32_config.h"
#include "stdio.h"
//#include "led.h"
//#include "lcd.h"
#include "key.h"
#include "AD9910.h"
//#include "task_manage.h"
//#include "timer.h"

char str[30]; //显示缓存
extern u8 _return;

extern uchar cfr2[4]; //cfr2控制字
extern uchar cfr1[4]; //cfr1控制字

int main(void)
{
	//	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2); //设置中断分组
	delay_init(72);										//初始化延时函数
	//LED_Init();											//初始化LED接口
	key_init(); //按键初始化
	//initial_lcd();										//液晶初始化
	//LCD_Clear();
	delay_ms(300);
	//LCD_Refresh_Gram();

	//Timerx_Init(99, 71);

	//welcome_KW();
	//标志排针插线颜色
	//LCD_Clear();
	//LCD_Show_CEStr(0, 0, "5"); //橙色 orange
	//9910初始化
	Init_ad9910();

	while (1)
	{
		KeyRead(); //读取按键值 （内部备注不详，不用管）
				   //		Set_PointFre(Keycode, 0);//按键处理
				   //		if(_return){_return=0;LCD_Refresh_Gram();}//更新显示

		if (PBin(11) == 0) //单频正弦
		{
			cfr1[0] = 0x00;		 //RAM 失能
			cfr2[1] = 0x00;		 //DRG 失能
			Txcfr();			 //发送cfrx控制字
			Amp_convert(500);	//写幅度，输入范围：1-650 mV
			Freq_convert(20000); //写频率，输入范围：1-400 000 000Hz
		}

		if (PBin(15) == 0) //扫频
		{
			Amp_convert(200);				   //写幅度，输入范围：1-650 mV
											   //扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：us）
			SweepFre(100, 100000, 20, 120000); //步进时间范围：4*(1~65536)ns
		}

		if (PBin(14) == 0) //RAM方波
		{
			Square_wave(20000); //方波，采样时间间隔输入范围：4*(1~65536)ns
		}

		if (PBin(13) == 0) //RAM锯齿波
		{
			Sawtooth_wave(20000); //锯齿波，采样时间间隔输入范围：4*(1~65536)ns
		}

		KEY_EXIT();
	}
}
