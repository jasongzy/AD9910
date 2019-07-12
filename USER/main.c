#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "AD9910.h"
#include "task_manage.h"
#include "timer.h"
#include "bsp_key.h"

char str[30];	                                             
extern u8 _return;

int main(void)
{
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	     
	delay_init(72);	                                         
	delay_ms(300);
	Timerx_Init(99,71);
	Init_ad9910();
	Txcfr();
	Write_Amplitude(20);
	Freq_convert(20000);
//	
//	delay_ms(5);
//	
	while(1)
	{
		
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) ==KEY_ON )
			 Square_wave(200000);                                    

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) ==KEY_ON )
			 Sawtooth_wave(100);
		
	}
	
//			Write_Amplitude(200); //写幅度，输入范围：1-650 mV
//		//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：us）
//		SweepFre(100, 100000, 20, 120000); //步进时间范围：4*(1~65536)ns
}

