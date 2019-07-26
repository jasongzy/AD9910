#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
//#include "lcd.h"
#include "key.h"
#include "AD9910.h"
//#include "task_manage.h"
//#include "timer.h"
#include "ascii_convert.h"

//char str[30]; //��ʾ����
//extern u8 _return;

extern uchar cfr2[4]; //cfr2������
extern uchar cfr1[4]; //cfr1������

int main(void)
{
	u16 i = 0;
	u16 j = 0;
	u8 key;
	u16 times = 0;
	u16 len;
	ulong ctrl[4];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����жϷ���
	//uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	uart_init(9600);
	delay_init(72); //��ʼ����ʱ����
	LED_Init();		//��ʼ��LED�ӿ�
	KEY_Init();		//������ʼ��
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
	
	cfr1[0] = 0x00;		 //RAM ʧ��
	cfr2[1] = 0x00;		 //DRG ʧ��
	Txcfr();			 //����cfrx������
	Amp_convert(200);	//д���ȣ����뷶Χ��1-650 mV
	Freq_convert(20000); //дƵ�ʣ����뷶Χ��1-400 000 000Hz
	
	printf("Initialization finished\r\n");

	while (1)
	{
		times++;
		if (times % 30 == 0)
			LED0 = !LED0; //��˸LED,��ʾϵͳ��������.
		delay_ms(10);

		if (USART_RX_STA & 0x8000)
		{
			LED1 = 0;
			len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
			if (USART_RX_BUF[0] == 0x41) //������������ʼ��"A",���ң�e.g. A20#B200#!��
			{
				cfr1[0] = 0x00; //RAM ʧ��
				cfr2[1] = 0x00; //DRG ʧ��
				Txcfr();		//����cfrx������
				//�����������
				for (i = 0; USART_RX_BUF[i] != 0x42 && i < len; i++);
				if (i == len) //δ�ҵ���2����ʼ��
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[0]=toint(USART_RX_BUF, i);
				ctrl[1]=toint(USART_RX_BUF + i, len - i);
				Freq_convert(1000*ctrl[0]);
				Amp_convert(ctrl[1]);
				printf("���ڿ��ƣ������źţ�%lu kHz / %lu mV��\r\n",ctrl[0],ctrl[1]);
			}
			else if (USART_RX_BUF[0] == 0x43) //������������ʼ��"C",������e.g. C20#!��
			{
				ctrl[0]=toint(USART_RX_BUF, len);
				Square_wave(1000*ctrl[0]);
				printf("���ڿ��ƣ������źţ�%lu kHz��\r\n",ctrl[0]);
			}
			else if (USART_RX_BUF[0] == 0x44) //������������ʼ��"D",��ݲ���e.g. D20#!��
			{
				ctrl[0]=toint(USART_RX_BUF, len);
				Sawtooth_wave(1000*ctrl[0]);
				printf("���ڿ��ƣ���ݲ��źţ�%lu kHz��\r\n",ctrl[0]);
			}
			else if (USART_RX_BUF[0] == 0x45) //������������ʼ��"E",ɨƵ��e.g. E100#F100000#G20#H120#!��
			{
				Amp_convert(200);
				//�����������
				for (i = 0; USART_RX_BUF[i] != 0x46 && i < len; i++);
				if (i == len) //δ�ҵ���2����ʼ��
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[0] = toint(USART_RX_BUF, i);
				for (j = i; USART_RX_BUF[j] != 0x47 && j < len; j++);
				if (j == len) //δ�ҵ���3����ʼ��
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[1] = toint(USART_RX_BUF + i, j - i);
				if (ctrl[0] >= ctrl[1]) //���޸�������
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				for (i = j; USART_RX_BUF[i] != 0x48 && i < len; i++);
				if (i == len) //δ�ҵ���4����ʼ��
				{
					printf("Error!\r\n");
					USART_RX_STA = 0;
					continue;
				}
				ctrl[2] = toint(USART_RX_BUF + j, i - j);
				ctrl[3] = toint(USART_RX_BUF + i, len - i);
				SweepFre(ctrl[0], ctrl[1], ctrl[2], ctrl[3] * 1000);
				printf("���ڿ��ƣ�ɨƵ�źţ�%lu-%lu Hz / %lu Hz / %lu ms��\r\n",ctrl[0],ctrl[1],ctrl[2],ctrl[3]);
			}
			else
			{
				printf("Error!\r\n");
			}
			USART_RX_STA = 0;
			delay_ms(20);
			LED1 = 1;
		}

		key = KEY_Scan(0); //�õ���ֵ
		if (key)
		{
			LED1 = 0;
			switch (key)
			{
			case WKUP_PRES:			 //��Ƶ����
				cfr1[0] = 0x00;		 //RAM ʧ��
				cfr2[1] = 0x00;		 //DRG ʧ��
				Txcfr();			 //����cfrx������
				Amp_convert(200);	//д���ȣ����뷶Χ��1-650 mV
				Freq_convert(20000); //дƵ�ʣ����뷶Χ��1-400 000 000Hz
				printf("�������ƣ������ź�\r\n");
				break;
			case KEY0_PRES: //RAM
				Sawtooth_wave(20000); //��ݲ�������ʱ�������뷶Χ��4*(1~65536)ns
				printf("�������ƣ���ݲ��ź�\r\n");
				break;
			case KEY2_PRES: //RAM
				Square_wave(20000); //����������ʱ�������뷶Χ��4*(1~65536)ns
				printf("�������ƣ������ź�\r\n");
				break;
			case KEY1_PRES:		  //ɨƵ
				Amp_convert(200); //д���ȣ����뷶Χ��1-650 mV
				//ɨƵ������Ƶ�ʣ�����Ƶ�ʣ�Ƶ�ʲ�������λ��Hz��������ʱ��������λ��us��
				SweepFre(100, 100000, 20, 120000); //����ʱ�䷶Χ��4*(1~65536)ns
				printf("�������ƣ�ɨƵ�ź�\r\n");
				break;
			}
			LED1 = 1;
		}
	}
}
