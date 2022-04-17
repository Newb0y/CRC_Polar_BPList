#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include"utility.h"
#include"random.h"
void BCDToBinary(unsigned long long bcd, int length, int vector[])
{
    int i, remain;
    unsigned long long qq;
	
    qq = bcd;
    for (i = 0; i < length; i++) {
        remain = qq % 2;
        qq = qq / 2;
        vector[length-i-1] = remain;
    }
}
//有用
void BCDToBinary1(int bcd, int length, int vector[])
{
    int i, remain;
    int qq;
	
    qq = bcd;
    for (i = 0; i < length; i++) {
        remain = qq % 2;
        qq = qq / 2;
        vector[length-i-1] = remain;
    }
}
int BinaryToBCD(int length, int vector[])
{
    int i, bcd = 0;
	
    for (i = 0; i < length; i++)
        if (vector[length-i-1] != 0)
            bcd += 1<<i;
		return bcd;
}

//有用
int reBinaryToBCD(int length, int vector[])//将vector中的二进制，翻转变为十进制
{
    int i, bcd = 0;
	
    for (i = 0; i < length; i++)
        if (vector[i] != 0)
            bcd += 1<<i;
		return bcd;
}

void dsBCDToBinary(double bcd,long int length,int vector[])
{
  int remain;
  for(int i=0;i<length;i++)
  {
	  //remain=(bcd*2)>=1?1:0;
	  if((bcd*2)>=1) 
	  {
		  remain=1;
		  bcd=sqrt(bcd*bcd*4)-1;
	  }
	  else 
	  {
		  remain=0;
		  bcd=sqrt(bcd*bcd*4);
	  }
      vector[i]=remain;
     
  }
}
double dsBinaryToBCD(int length, int vector[])
{
    double bcd=0.0;
	
    for (int i=0; i<length;i++)
        if (vector[length-i-1] != 0)
            bcd += 1<<i;
		return bcd/(1<<length);
}


double GaussRand(double sigma)
{ double x,y;
  x=RandD();
  if(x<TINY) x=TINY;
   y=2.0*PI*RandD();
   return(sigma*sqrt(-2*log(x))*cos(y));

}
/*void ComplexGauss(double sigma, dcomplex *v)
{
    double x, y, z;

    x = RandD();
    if (x < TINY) x = TINY;
    y = 2.0 * PI * RandD();
    z = sigma * sqrt(-2.0 * log(x));
    v->re = z * cos(y);
	v->im = z*sin(y);
}*/

//log2()近似计算
int log2(int x)
{
	int i=0;
	int temp=x;
	while(temp>=2)
	{
		temp=temp/2;
		i++;
	}

	return i;
}

double box_plus(double x,double y)
{
	double z, z1, zmax;
	if (x < y) {
		z1 = x;
		zmax = y;
	} else {
		z1 = y;
		zmax = x;
	}
	z = zmax + log(1 + exp(z1 - zmax));
	z = z - log(1. + exp(x + y));
	return -z;
}
void copyIntegerArray(int* a, int* b, int len)
{
	for(int i = 0; i < len; i ++)
	{
		b[i] = a[i];
	}
}
