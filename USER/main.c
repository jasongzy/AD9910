#include "stm32_config.h"
#include "stdio.h"
//#include "led.h"
//#include "lcd.h"
#include "key.h"
#include "AD9910.h"
//#include "task_manage.h"
//#include "timer.h"

char str[30]; //��ʾ����
extern u8 _return;

extern uchar cfr2[4]; //cfr2������
extern uchar cfr1[4]; //cfr1������

int main(void)
{
	//	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2); //�����жϷ���
	delay_init(72);										//��ʼ����ʱ����
	//LED_Init();											//��ʼ��LED�ӿ�
	key_init(); //������ʼ��
	//initial_lcd();										//Һ����ʼ��
	//LCD_Clear();
	delay_ms(300);
	//LCD_Refresh_Gram();

	//Timerx_Init(99, 71);

	//welcome_KW();
	//��־���������ɫ
	//LCD_Clear();
	//LCD_Show_CEStr(0, 0, "5"); //��ɫ orange
	//9910��ʼ��
	Init_ad9910();

	while (1)
	{
		KeyRead(); //��ȡ����ֵ ���ڲ���ע���꣬���ùܣ�
				   //		Set_PointFre(Keycode, 0);//��������
				   //		if(_return){_return=0;LCD_Refresh_Gram();}//������ʾ

		if (PBin(11) == 0) //��Ƶ����
		{
			cfr1[0] = 0x00;		 //RAM ʧ��
			cfr2[1] = 0x00;		 //DRG ʧ��
			Txcfr();			 //����cfrx������
			Amp_convert(500);	//д���ȣ����뷶Χ��1-650 mV
			Freq_convert(20000); //дƵ�ʣ����뷶Χ��1-400 000 000Hz
		}

		if (PBin(15) == 0) //ɨƵ
		{
			Amp_convert(200);				   //д���ȣ����뷶Χ��1-650 mV
											   //ɨƵ������Ƶ�ʣ�����Ƶ�ʣ�Ƶ�ʲ�������λ��Hz��������ʱ��������λ��us��
			SweepFre(100, 100000, 20, 120000); //����ʱ�䷶Χ��4*(1~65536)ns
		}

		if (PBin(14) == 0) //RAM����
		{
			Square_wave(20000); //����������ʱ�������뷶Χ��4*(1~65536)ns
		}

		if (PBin(13) == 0) //RAM��ݲ�
		{
			Sawtooth_wave(20000); //��ݲ�������ʱ�������뷶Χ��4*(1~65536)ns
		}

		KEY_EXIT();
	}
}
