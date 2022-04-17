#include "MathUtil.h"

//二进制除法
long int division_mod2(vector<int> dividend,long int divisor,int crc_x)
{
	long int reg=0x0;//定义寄存器
	int k=dividend.size();
	for(int cur_bit=0;cur_bit<k;cur_bit++)
	{
		unsigned short temp=dividend[cur_bit];//取出divisor[cur_bit]
		reg<<=1;
		reg|=temp;//将divisor[cur_bit]加载到寄存器中
		if(((reg>>crc_x)&0x0001)==0x1)//判断reg的第crc_x位是否为1
			reg=reg^divisor;
	}
	return reg;
}