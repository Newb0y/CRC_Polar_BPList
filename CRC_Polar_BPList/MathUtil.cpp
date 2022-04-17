#include "MathUtil.h"

//�����Ƴ���
long int division_mod2(vector<int> dividend,long int divisor,int crc_x)
{
	long int reg=0x0;//����Ĵ���
	int k=dividend.size();
	for(int cur_bit=0;cur_bit<k;cur_bit++)
	{
		unsigned short temp=dividend[cur_bit];//ȡ��divisor[cur_bit]
		reg<<=1;
		reg|=temp;//��divisor[cur_bit]���ص��Ĵ�����
		if(((reg>>crc_x)&0x0001)==0x1)//�ж�reg�ĵ�crc_xλ�Ƿ�Ϊ1
			reg=reg^divisor;
	}
	return reg;
}