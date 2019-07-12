/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6����
			��ʾ��12864
�ӿڣ������ӿ������key.h
ʱ�䣺2015/11/3
�汾��1.0
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
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

char str[30];	//��ʾ����
extern u8 _return;
int main(void)
{
//	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(72);	//��ʼ����ʱ����
	LED_Init();	//��ʼ��LED�ӿ�
	key_init();//������ʼ��
	initial_lcd();//Һ����ʼ��
	LCD_Clear();
	delay_ms(300);
	LCD_Refresh_Gram();
	
	Timerx_Init(99,71);
	
	//welcome_KW();
	//��־���������ɫ
	LCD_Clear();
	LCD_Show_CEStr(0,0,"5");//��ɫ orange
	//9910��ʼ��
	Init_ad9910();
	Amp_convert(300);
	Freq_convert(20000);

//	while(1)
//	{
//		KeyRead();//��ȡ����ֵ ���ڲ���ע���꣬���ùܣ� 
//		Set_PointFre(Keycode, 0);//��������
//		if(_return){_return=0;LCD_Refresh_Gram();}//������ʾ
//		KEY_EXIT();
//	}	
}

