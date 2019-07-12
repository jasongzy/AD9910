/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6����
			��ʾ��12864
�ӿڣ������ӿ������key.h
ʱ�䣺2015/11/3
�汾��1.0
���ߣ���������
����������������ԭ�ӣ�����Լ���������δ��������ɣ��������������κ���;

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "stm32_config.h"
#include "AD9910.h"

extern uchar cfr2[4]; //cfr2������
extern uchar cfr1[4]; //cfr1������

char str[30];	//��ʾ����
extern u8 _return;
int main(void)
{
	u16 i=0;
	unsigned long int freq = 1000000;

	delay_init(72);	//��ʼ����ʱ����
	
	
	Init_ad9910();
	Freq_convert(30000000);
	Write_Amplitude(600); //д���ȣ����뷶Χ��1-650 mV
	

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
			cfr1[0] = 0x00; //RAM ʧ��
			cfr2[1] = 0x00; //DRG ʧ��
			Txcfr(); //����cfrx������
			Write_Amplitude(500); //д���ȣ����뷶Χ��1-650 mV
			Freq_convert(100000); //дƵ�ʣ����뷶Χ��1-400 000 000Hz
		}
		if(PAin(8) == 0)
		{
			delay_ms(5);
			cfr1[0] = 0x00; //RAM ʧ��
			cfr2[1]=0x0e; //DRG ʹ��
			Txcfr(); //����cfrx������
			Write_Amplitude(500); //д���ȣ����뷶Χ��1-650 mV
			//ɨƵ������Ƶ�ʣ�����Ƶ�ʣ�Ƶ�ʲ�������λ��Hz��������ʱ��������λ��us��
			SweepFre(100, 100000, 10, 240000); //����ʱ�䷶Χ��4*(1~65536)ns
		}
		
		if(PDin(3) == 0)
		{
			delay_ms(5);
			Square_wave(200); //����������ʱ�������뷶Χ��4*(1~65536)ns
			cfr1[0] = 0xc0; //RAM ʹ�ܣ����ȿ���
			cfr2[1] = 0x00; //DRG ʧ��
			Txcfr(); //����cfrx������
		}
		
*/

