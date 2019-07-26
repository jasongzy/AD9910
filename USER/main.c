#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
//#include "lcd.h"
#include "key.h"
#include "AD9910.h"
//#include "task_manage.h"
//#include "timer.h"
#include "ascii_convert.h"

//char str[30]; //显示缓存
//extern u8 _return;

extern uchar cfr2[4]; //cfr2控制字
extern uchar cfr1[4]; //cfr1控制字

int main(void)
{
	u16 i = 0;
	u16 j = 0;
	u8 key;
	u16 times = 0;
	u16 len;
	ulong ctrl[4];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组
	//uart_init(115200);	 //串口初始化为115200
	uart_init(9600);
	delay_init(72); //初始化延时函数
	LED_Init();		//初始化LED接口
	KEY_Init();		//按键初始化
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
	
	cfr1[0] = 0x00;		 //RAM 失能
	cfr2[1] = 0x00;		 //DRG 失能
	Txcfr();			 //发送cfrx控制字
	Amp_convert(200);	//写幅度，输入范围：1-650 mV
	Freq_convert(20000); //写频率，输入范围：1-400 000 000Hz
	
	printf("Initialization finished\r\n");

	while (1)
	{
		times++;
		if (times % 30 == 0)
			LED0 = !LED0; //闪烁LED,提示系统正在运行.
		delay_ms(10);

		if (USART_RX_STA & 0x8000)
		{
			LED1 = 0;
			len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
			if (USART_RX_BUF[0] == 0x41) //串口输入了起始符"A",正弦（e.g. A20#B200#!）
			{
				cfr1[0] = 0x00; //RAM 失能
				cfr2[1] = 0x00; //DRG 失能
				Txcfr();		//发送cfrx控制字
				//拆分两组数据
				for (i = 0; USART_RX_BUF[i] != 0x42 && i < len; i++);
				if (i == len) //未找到第2个起始符
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[0]=toint(USART_RX_BUF, i);
				ctrl[1]=toint(USART_RX_BUF + i, len - i);
				Freq_convert(1000*ctrl[0]);
				Amp_convert(ctrl[1]);
				printf("串口控制：正弦信号（%lu kHz / %lu mV）\r\n",ctrl[0],ctrl[1]);
			}
			else if (USART_RX_BUF[0] == 0x43) //串口输入了起始符"C",方波（e.g. C20#!）
			{
				ctrl[0]=toint(USART_RX_BUF, len);
				Square_wave(1000*ctrl[0]);
				printf("串口控制：方波信号（%lu kHz）\r\n",ctrl[0]);
			}
			else if (USART_RX_BUF[0] == 0x44) //串口输入了起始符"D",锯齿波（e.g. D20#!）
			{
				ctrl[0]=toint(USART_RX_BUF, len);
				Sawtooth_wave(1000*ctrl[0]);
				printf("串口控制：锯齿波信号（%lu kHz）\r\n",ctrl[0]);
			}
			else if (USART_RX_BUF[0] == 0x45) //串口输入了起始符"E",扫频（e.g. E100#F100000#G20#H120#!）
			{
				Amp_convert(200);
				//拆分四组数据
				for (i = 0; USART_RX_BUF[i] != 0x46 && i < len; i++);
				if (i == len) //未找到第2个起始符
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[0] = toint(USART_RX_BUF, i);
				for (j = i; USART_RX_BUF[j] != 0x47 && j < len; j++);
				if (j == len) //未找到第3个起始符
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[1] = toint(USART_RX_BUF + i, j - i);
				if (ctrl[0] >= ctrl[1]) //下限高于上限
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				for (i = j; USART_RX_BUF[i] != 0x48 && i < len; i++);
				if (i == len) //未找到第4个起始符
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[2] = toint(USART_RX_BUF + j, i - j);
				ctrl[3] = toint(USART_RX_BUF + i, len - i);
				SweepFre(ctrl[0], ctrl[1], ctrl[2], ctrl[3] * 1000);
				printf("串口控制：扫频信号（%lu-%lu Hz / %lu Hz / %lu ms）\r\n",ctrl[0],ctrl[1],ctrl[2],ctrl[3]);
			}
			else
			{
				printf("Error!\r\n");
			}
			USART_RX_STA = 0;
			delay_ms(20);
			LED1 = 1;
		}

		key = KEY_Scan(0); //得到键值
		if (key)
		{
			LED1 = 0;
			switch (key)
			{
			case WKUP_PRES:			 //单频正弦
				cfr1[0] = 0x00;		 //RAM 失能
				cfr2[1] = 0x00;		 //DRG 失能
				Txcfr();			 //发送cfrx控制字
				Amp_convert(200);	//写幅度，输入范围：1-650 mV
				Freq_convert(20000); //写频率，输入范围：1-400 000 000Hz
				printf("按键控制：正弦信号\r\n");
				break;
			case KEY0_PRES: //RAM
				Sawtooth_wave(20000); //锯齿波，采样时间间隔输入范围：4*(1~65536)ns
				printf("按键控制：锯齿波信号\r\n");
				break;
			case KEY2_PRES: //RAM
				Square_wave(20000); //方波，采样时间间隔输入范围：4*(1~65536)ns
				printf("按键控制：方波信号\r\n");
				break;
			case KEY1_PRES:		  //扫频
				Amp_convert(200); //写幅度，输入范围：1-650 mV
				//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：us）
				SweepFre(100, 100000, 20, 120000); //步进时间范围：4*(1~65536)ns
				printf("按键控制：扫频信号\r\n");
				break;
			}
			LED1 = 1;
		}
	}
}
