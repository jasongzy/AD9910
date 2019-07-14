#include "sys.h"
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long int
//#include "math.h"

ulong toint(u8* a,u8 len)
{
	u8 i;
	ulong b = 0;
	for(i=1;a[i]!=0x23 && i<len-1;i++) //直到收到结束符#
	{
		if((a[i]>=48)&&(a[i]<=57))
		{
			b = b*10+(a[i]-48);
		}
		else 
		{
			break;
		}
	}
	return b;
}
