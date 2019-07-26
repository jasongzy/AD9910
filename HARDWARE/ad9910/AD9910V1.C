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

#include "stm32f4xx.h"
#include "AD9910.h"
#include "delay.h"
#include "sys.h"
/*-----------------------------------------------
  名称：AD9910串行驱动
  编写：Liu
  日期：2014.6
  修改：无
  内容：
------------------------------------------------*/
uchar cfr1[] = {0x00, 0x40, 0x00, 0x00};						  //cfr1控制字
uchar cfr2[] = {0x01, 0x00, 0x00, 0x00};						  //cfr2控制字
uchar cfr3[] = {0x05, 0x0F, 0x41, 0x32};						  //cfr3控制字  40M输入  25倍频  VC0=101   ICP=001;
uchar profile11[] = {0x3f, 0xff, 0x00, 0x00, 0x25, 0x09, 0x7b, 0x42}; //profile1控制字 0x25,0x09,0x7b,0x42
																	  //01振幅控制 23相位控制 4567频率调谐字

uchar ramprofile0[8] = {0x00};                                     //ramprofile0控制字
uchar drgparameter[20]={0x00};

//高14位幅度控制
const unsigned char ramdata_Square[4096] = {	
//方波
0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 
0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 
0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 
0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00,

0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,


};

//高14位幅度控制
const unsigned char ramdata_Sawtooth[4096] = {
//锯齿波
0x00,0x00,0x00,0x00, 0x03,0xfc,0x00,0x00, 0x07,0xf8,0x00,0x00, 0x0b,0xf4,0x00,0x00, 0x0f,0xf0,0x00,0x00, 0x13,0xec,0x00,0x00, 0x17,0xe8,0x00,0x00, 0x1b,0xe4,0x00,0x00, 
0x1f,0xe0,0x00,0x00, 0x23,0xdc,0x00,0x00, 0x27,0xd8,0x00,0x00, 0x2b,0xd4,0x00,0x00, 0x2f,0xd0,0x00,0x00, 0x33,0xcc,0x00,0x00, 0x37,0xc8,0x00,0x00, 0x3b,0xc4,0x00,0x00, 
0x3f,0xc0,0x00,0x00, 0x43,0xbc,0x00,0x00, 0x47,0xb8,0x00,0x00, 0x4b,0xb4,0x00,0x00, 0x4f,0xb0,0x00,0x00, 0x53,0xac,0x00,0x00, 0x57,0xa8,0x00,0x00, 0x5b,0xa4,0x00,0x00, 
0x5f,0xa0,0x00,0x00, 0x63,0x9c,0x00,0x00, 0x67,0x98,0x00,0x00, 0x6b,0x94,0x00,0x00, 0x6f,0x90,0x00,0x00, 0x73,0x8c,0x00,0x00, 0x77,0x88,0x00,0x00, 0x7b,0x84,0x00,0x00, 	

0x7f,0x80,0x00,0x00, 0x83,0x7c,0x00,0x00, 0x87,0x78,0x00,0x00, 0x8b,0x74,0x00,0x00, 0x8f,0x70,0x00,0x00, 0x93,0x6c,0x00,0x00, 0x97,0x68,0x00,0x00, 0x9b,0x64,0x00,0x00, 
0x9f,0x60,0x00,0x00, 0xa3,0x5c,0x00,0x00, 0xa7,0x58,0x00,0x00, 0xab,0x54,0x00,0x00, 0xaf,0x50,0x00,0x00, 0xb3,0x4c,0x00,0x00, 0xb7,0x48,0x00,0x00, 0xbb,0x44,0x00,0x00, 
0xbf,0x40,0x00,0x00, 0xc3,0x3c,0x00,0x00, 0xc7,0x38,0x00,0x00, 0xcb,0x34,0x00,0x00, 0xcf,0x30,0x00,0x00, 0xd3,0x2c,0x00,0x00, 0xd7,0x28,0x00,0x00, 0xdb,0x24,0x00,0x00, 
0xdf,0x20,0x00,0x00, 0xe3,0x1c,0x00,0x00, 0xe7,0x18,0x00,0x00, 0xeb,0x14,0x00,0x00, 0xef,0x10,0x00,0x00, 0xf3,0x0c,0x00,0x00, 0xf7,0x08,0x00,0x00, 0xfb,0x04,0x00,0x00,


};

void AD9110_IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE); //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All ^ (GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//=====================================================================

//======================发送8位数据程序===================================
void txd_8bit(uchar txdat)
{
	uchar i, sbt;
	sbt = 0x80;
	SCLK = 0;
	for (i = 0; i < 8; i++)
	{
		if ((txdat & sbt) == 0)
			AD9910_SDIO = 0;
		else
			AD9910_SDIO = 1;
		SCLK = 1;
		sbt = sbt >> 1;
		SCLK = 0;
	}
}

//======================ad9910初始化程序================================
void Init_ad9910(void)
{
	AD9110_IOInit(); //IO初始化
	AD9910_PWR = 0;  //软件拉低

	PROFILE2 = PROFILE1 = PROFILE0 = 0;
	DRCTL = 0;
	DRHOLD = 0;
	MAS_REST = 1;
	delay_ms(5);
	MAS_REST = 0;
}

//======================发送cfr控制字====================================
void Txcfr(void)
{
	uchar k, m;
	
	CS = 0;
	txd_8bit(0x00); //发送CFR1控制字地址
	for (m = 0; m < 4; m++)
		txd_8bit(cfr1[m]);
	CS = 1;
	for (k = 0; k < 10; k++)
		;

	CS = 0;
	txd_8bit(0x01); //发送CFR2控制字地址
	for (m = 0; m < 4; m++)
		txd_8bit(cfr2[m]);
	CS = 1;
	for (k = 0; k < 10; k++)
		;

	CS = 0;
	txd_8bit(0x02); //发送CFR3控制字地址
	for (m = 0; m < 4; m++)
		txd_8bit(cfr3[m]);
	CS = 1;
	for (k = 0; k < 10; k++)
		;

	UP_DAT = 1;
	for (k = 0; k < 10; k++)
		;
	UP_DAT = 0;
	delay_ms(1);
}
//=====================================================================

//======================ad9910发送profile0控制字程序===================================
void Txprodile0(void)
{
	uchar m;

	CS = 0;
	txd_8bit(0x0e); //发送profile0控制字地址
	for (m = 0; m < 8; m++)
		txd_8bit(profile11[m]);
	CS = 1;
	// for(k=0;k<10;k++);

	UP_DAT = 1;
	// for(k=0;k<10;k++);
	UP_DAT = 0;
	// Delay_ms(1);
}
//=====================================================================
//===================计算频偏字、频率字和发送程序======================
void Freq_convert(ulong Freq)
{
	ulong Temp;
	Temp = (ulong)Freq * 4.294967296; //将输入频率因子分为四个字节  4.294967296=(2^32)/1000000000
	profile11[7] = (uchar)Temp;
	profile11[6] = (uchar)(Temp >> 8);
	profile11[5] = (uchar)(Temp >> 16);
	profile11[4] = (uchar)(Temp >> 24);
	Txprodile0();
}

//======================计算正弦波幅值(Vpp)和发送程序==============================
void Amp_convert(uint Amp)
{
	ulong Temp;
	Temp = (ulong)Amp * 28.4829; //将输入幅度因子分为两个字节  25.20615385=(2^14)/650
	if (Temp > 0x3fff)
		Temp = 0x3fff;
	Temp &= 0x3fff;
	profile11[1] = (uchar)Temp;
	profile11[0] = (uchar)(Temp >> 8);
	Txprodile0();
}

//======================ad9910发送DRG参数程序==================================
void Txdrg(void)
{
	uchar m,k;

	CS=0;
	txd_8bit(0x0b);                                     //发送数字斜坡限制地址0x0b
	for (m=0;m<8;m++)
		txd_8bit(drgparameter[m]); 
	CS=1;
	for(k=0;k<10;k++);
	
	CS=0;
	txd_8bit(0x0c);                                     //发送数字斜坡步长地址0x0c
	for (m=8;m<16;m++)
		txd_8bit(drgparameter[m]); 
	CS=1;
	for(k=0;k<10;k++);
	
	CS=0;
	txd_8bit(0x0d);                                     //发送数字斜坡速率地址0x0d
	for (m=16;m<20;m++)
		txd_8bit(drgparameter[m]); 
	CS=1;
	for(k=0;k<10;k++);
	
	UP_DAT=1;
	for(k=0;k<10;k++);
	UP_DAT=0;
	delay_ms(1);
}         

//=====================扫频波参数设置和发送程序===================================
void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime)
{
	ulong Temp1, Temp2, ITemp3, DTemp3, ITemp4, DTemp4;
	Temp1 = (ulong)SweepMinFre*4.294967296;
	if(SweepMaxFre > 400000000)
		SweepMaxFre = 400000000;
	Temp2 = (ulong)SweepMaxFre*4.294967296;
	if(SweepStepFre > 400000000)
		SweepStepFre = 400000000;
	ITemp3 = (ulong)SweepStepFre*4.294967296;
	DTemp3 = ITemp3;
	ITemp4 = (ulong)SweepTime/4;                                    //1GHz/4, 单位：ns
	if(ITemp4 > 0xffff)
		ITemp4 = 0xffff;
	DTemp4 = ITemp4;
	
	//扫频上下限
	drgparameter[7]=(uchar)Temp1;
	drgparameter[6]=(uchar)(Temp1>>8);
	drgparameter[5]=(uchar)(Temp1>>16);
	drgparameter[4]=(uchar)(Temp1>>24);
	drgparameter[3]=(uchar)Temp2;
	drgparameter[2]=(uchar)(Temp2>>8);
	drgparameter[1]=(uchar)(Temp2>>16);
	drgparameter[0]=(uchar)(Temp2>>24);
	//频率步进（单位：Hz）
	drgparameter[15]=(uchar)ITemp3;
	drgparameter[14]=(uchar)(ITemp3>>8);
	drgparameter[13]=(uchar)(ITemp3>>16);
	drgparameter[12]=(uchar)(ITemp3>>24);
	drgparameter[11]=(uchar)DTemp3;
	drgparameter[10]=(uchar)(DTemp3>>8);
	drgparameter[9]=(uchar)(DTemp3>>16);
	drgparameter[8]=(uchar)(DTemp3>>24);
	//步进时间间隔（单位：us）
	drgparameter[19]=(uchar)ITemp4;
	drgparameter[18]=(uchar)(ITemp4>>8);
	drgparameter[17]=(uchar)DTemp4;
	drgparameter[16]=(uchar)(DTemp4>>8);
	//发送DRG参数
	Txdrg();
	cfr1[0] = 0x00; //RAM 失能
	cfr2[1] = 0x0e; //DRG 使能
	Txcfr(); //发送cfrx控制字
}

//==========================发送ramprofile0控制字=============================
void Txramprofile(void)
{
        uchar m,k;

        CS=0;
        txd_8bit(0x0e);                            //发送ramprofile0控制字地址
        for (m=0;m<8;m++)
                txd_8bit(ramprofile0[m]); 
        CS=1;
        for(k=0;k<10;k++);

        UP_DAT=1;
        for(k=0;k<10;k++);
        UP_DAT=0;
        delay_ms(1);
} 
	
//=======================发送方波、锯齿波波形程序=============================
void Txramdata(int wave_num)
{
        
	if(wave_num==1)
	{			uint m,k;
        CS=0;
        txd_8bit(0x16);                                    //发送ram控制字地址
        for (m=0; m<4096; m++)
						txd_8bit(ramdata_Square[m]); 
        CS=1;
        for(k=0;k<10;k++);
        UP_DAT=1;
        for(k=0;k<10;k++);
        UP_DAT=0;
        delay_ms(1);
	}
	else
	{
				uint m,k;
        CS=0;
        txd_8bit(0x16);                                   //发送ram控制字地址
        for(m=0; m<4096; m++)
						txd_8bit(ramdata_Sawtooth[m]); 
        CS=1;
        for(k=0;k<10;k++);
        UP_DAT=1;
        for(k=0;k<10;k++);
        UP_DAT=0;
        delay_ms(1);
	}
	
}   

//========================产生方波的程序======================================
void Square_wave(uint Sample_interval)
{
        ulong Temp;

	      Temp = ((1000000000/(unsigned long int)(Sample_interval)/64/4));        //1GHz/4, 采样间隔范围：4*(1~65536)ns
        if(Temp > 0xffff)
                Temp = 0xffff;
        ramprofile0[7] = 0x24;
        ramprofile0[6] = 0x00;
        ramprofile0[5] = 0x00;
        ramprofile0[4] = 0xc0;
        ramprofile0[3] = 0x0f;
        ramprofile0[2] = (uchar)Temp;
        ramprofile0[1] = (uchar)(Temp>>8);
        ramprofile0[0] = 0x00;
        Txramprofile();
				Txramdata(1); 
        cfr1[0] = 0xc0;                                    //RAM 使能，幅度控制
	      cfr2[1] = 0x00;                                              //DRG 失能
	      Txcfr();                                               //发送cfrx控制字				
}

//========================产生锯齿波的程序=====================================
void Sawtooth_wave(uint Sample_interval)
{
        ulong Temp;

	      Temp = ((1000000000/(unsigned long int)(Sample_interval)/64/4));         //1GHz/4, 采样间隔范围：4*(1~65536)ns
        if(Temp > 0xffff)
                Temp = 0xffff;
        ramprofile0[7] = 0x24;
        ramprofile0[6] = 0x00;
        ramprofile0[5] = 0x00;
        ramprofile0[4] = 0xc0;
        ramprofile0[3] = 0x0f;
        ramprofile0[2] = (uchar)Temp;
        ramprofile0[1] = (uchar)(Temp>>8);
        ramprofile0[0] = 0x00;
        Txramprofile();
				Txramdata(0); 
        cfr1[0] = 0xc0;                                    //RAM 使能，幅度控制
	      cfr2[1] = 0x00;                                              //DRG 失能
				Txcfr();                                               //发送cfrx控制字				
}

