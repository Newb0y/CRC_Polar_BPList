#ifndef UTILITY_H
#define UTILITY_H

const double  PI=3.1415926536;
const double  TINY=1.e-100;
void BCDToBinary(unsigned long long bcd, int length, int vector[]);
int BinaryToBCD(int length, int vector[]);
void dsBCDToBinary(double bcd,long int length,int vector[]);
double dsBinaryToBCD(int length, int vector[]);
double GaussRand(double sigma);
void BCDToBinary1(int bcd, int length, int vector[]);
int reBinaryToBCD(int length, int vector[]);
//void ComplexGauss(double sigma, dcomplex *v);

int log2(int x);//log2()½üËÆ¼ÆËã
void copyIntegerArray(int* a, int* b, int len);
double box_plus(double x,double y);
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define Inf 14

#endif