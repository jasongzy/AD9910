/**********************************************************
                       康威电子
										 
功能：stm32f103rct6控制
			显示：12864
接口：按键接口请参照key.h
时间：2015/11/3
版本：1.0
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "AD9910.h"
#include "task_manage.h"
#include "timer.h"

char str[30];	//显示缓存
extern u8 _return;
int main(void)
{
//	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	LED_Init();	//初始化LED接口
	key_init();//按键初始化
	initial_lcd();//液晶初始化
	LCD_Clear();
	delay_ms(300);
	LCD_Refresh_Gram();
	
	Timerx_Init(99,71);
	
	//welcome_KW();
	//标志排针插线颜色
	LCD_Clear();
	LCD_Show_CEStr(0,0,"5");//橙色 orange
	//9910初始化
	Init_ad9910();
	Amp_convert(300);
	Freq_convert(20000);

//	while(1)
//	{
//		KeyRead();//读取按键值 （内部备注不详，不用管） 
//		Set_PointFre(Keycode, 0);//按键处理
//		if(_return){_return=0;LCD_Refresh_Gram();}//更新显示
//		KEY_EXIT();
//	}	
}

