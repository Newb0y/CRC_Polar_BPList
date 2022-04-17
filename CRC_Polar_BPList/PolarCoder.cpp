
#include "PolarCoder.h"
#include "FileName.h"
#include "utility.h"
#include "MemoryManagementUtil.h"
#include<iostream>
#include<fstream>
#include <windows.h>
using namespace std;
PolarCoder::PolarCoder()
{
	ifstream iPolar(POLAR_FILE);
	iPolar>>K;
	iPolar>>N;
	iPolar.close();

	ifstream iCRC(CRC_length);
	iCRC >> CRCLength;
	iCRC.close();
	KwithCRC = K + CRCLength;

	n = log(N)/log(2);
	M = n + 1;
	R = (double)K/(double)N;
	Iw_N = new IW[N];
	mark_frozenPosition = new int[N];
	readConsturction();
	IW_NToMark_FrozenPosition();
	F = createIntegerArray(N, N);
	Generate_F();
	sourceWithCRC = createIntegerArray(KwithCRC);
}

PolarCoder::~PolarCoder(void)
{
	delete[] Iw_N;
	delete[] mark_frozenPosition;
	deleteIntegerArray(F, N);
}

void PolarCoder::constructor(Communication* comm)
{
	int* source = comm->source;
	int* constructionSequence = comm->constructionSequence;
	CRC_encoder(K, source, sourceWithCRC);
	int j = 0;
	/*for (int i=N-1;i>=0;i--)
	{
		if (!mark_frozenPosition[i])
			constructionSequence[i] = 0;
		else if (mark_frozenPosition[i])
		{
			constructionSequence[i] = sourceWithCRC[j];
			j++;
		}
	}*/
	for (int i = 0; i < KwithCRC; i++)
	{
		constructionSequence[Iw_N[N - i - 1].index] = sourceWithCRC[i];
	}
	for (int i = 0; i < (N - KwithCRC); i++)
	{
		constructionSequence[Iw_N[i].index] = 0;
	}
}


void PolarCoder::encoder(Communication* comm)
{
	int* constructionSequence = comm->constructionSequence;
	int* codeword = comm->codeword;
	
	for(int j=0;j<N;j++)
	{
		codeword[j]=0;
		for(int l=0;l<N;l++)
		{
			codeword[j] ^= (constructionSequence[l]*F[l][j]);
		}
	}
}

void PolarCoder::readConsturction()
{
	ifstream iSequence(SEQUENCE_FILE);
	ifstream iMetric(METRIC_FILE);
	for (int i = 0; i < N; i++)
	{
		iSequence>>Iw_N[i].index;
		iMetric>>Iw_N[i].metric;
	}
	iSequence.close();
	iMetric.close();
}


//有用
void PolarCoder::Generate_F()
{
	int M = n + 1;
	int *data1=new int[M-1];
	int *data2=new int[M-1];
	int *data3=new int[M-1];
	int f;
	for(int i=0;i<N;i++)
	{
	  BCDToBinary1(i,M-1,data1);
	   for(int j=0;j<N;j++)
	   {  f=1;
	      BCDToBinary1(j,M-1,data2);
		  for(int k=0;k<M-1;k++)
		  {
		    data3[k]=(1+data2[k]+data1[k]*data2[k])%2;
			if(data3[k]==0){f=0;break;}
			else{
				f=f*data3[k];
		  } 
		  }
	     F[i][j]=f;
	   }		
	}

	delete[] data1;
	delete[] data2;
	delete[] data3;
}

int PolarCoder::CounteErrors(int *estimates, int *rawdata, int length)
{
	int k, e = 0;

	for (k = 0; k < length; k++)
	{
		if (rawdata[Iw_N[N - KwithCRC + k].index] != estimates[Iw_N[N - KwithCRC + k].index])
		{
			e++;
		}
	}

	return e;
}

ResultOnce PolarCoder::resultAnalysis(Communication* comm)
{
	ResultOnce resultOnce;
	int*  constructionSequence = comm->constructionSequence;
	int* Estimate = comm->Estimate;
	int errorBitNumTemp = CounteErrors(Estimate, constructionSequence, K);
	resultOnce.errorBitNum = errorBitNumTemp;
	resultOnce.errorFrameNum = errorBitNumTemp > 0 ? 1 : 0;

	return resultOnce;
}

//信息序列转冻结位
void PolarCoder::IW_NToMark_FrozenPosition()
{
	for(int i = 0; i < N - KwithCRC; i ++)
	{
		mark_frozenPosition[Iw_N[i].index] = 0;
	}
	for(int i = N - KwithCRC; i < N; i ++)
	{
		mark_frozenPosition[Iw_N[i].index] = 1;//信息位
	}
}


void PolarCoder::outputBaseInfo()
{
	cout<<"构造器类型："<<SEQUENCE_FILE<<"\n";
	cout<<"信源长度："<<K<<"   码长："<<N<<"\n";
	ofstream oResult(RESULT_FILE,ios::app);
	oResult<<"构造器类型："<<SEQUENCE_FILE<<"\n";
	oResult<<"信源长度："<<K<<"   码长："<<N<<"\n";
	oResult << "CRCLength: " << CRCLength << endl;
	oResult.close();
}

#include "MathUtil.h"
void PolarCoder::CRC_encoder(int K, int* input, int* output)
{
	long int  POLY;
	int N = K + CRCLength;
	if (CRCLength == 0)
	{
		for (int i = 0; i < K; i++)	output[i] = input[i];
		return;
	}
	else if (CRCLength == 3)		POLY = 0x9;
	else if (CRCLength == 4)		POLY = 0x13;
	else if (CRCLength == 5)		POLY = 0x25;
	else if (CRCLength == 6)		POLY = 0x67;
	else if (CRCLength == 7)		POLY = 0xCF;
	else if (CRCLength == 8)		POLY = 0x131;
	else if (CRCLength == 9)		POLY = 0x233;
	else if (CRCLength == 10)	POLY = 0x64F;
	else if (CRCLength == 11)	POLY = 0xBB7;
	else if (CRCLength == 12)	POLY = 0x180F;
	else if (CRCLength == 13)	POLY = 0x357F;
	else if (CRCLength == 14)	POLY = 0x6E57;
	else if (CRCLength == 15)	POLY = 0x9E47;
	else if (CRCLength == 16)	POLY = 0x18005;
	else if (CRCLength == 17)	POLY = 0x2df4f;
	else if (CRCLength == 18)	POLY = 0x472f3;
	else if (CRCLength == 19)	POLY = 0xdf6af;
	else if (CRCLength == 20)	POLY = 0x189b0f;
	else if (CRCLength == 21)	POLY = 0x2e0fd5;
	else if (CRCLength == 24)	POLY = 0x1864CFB;
	else	printf("\n Wrong crc_x at CRC.\n");

	vector<int> temp_input(N);
	for (int i = 0; i < K; i++)
	{
		output[i] = input[i];
		temp_input[i] = input[i];
	}
	for (int i = K; i < N; i++)	temp_input[i] = 0;

	long int reg = division_mod2(temp_input, POLY, CRCLength);

	int* temp_CRC = createIntegerArray(CRCLength);
	BCDToBinary(reg, CRCLength, temp_CRC);

	for (int i = K; i < N; i++)	output[i] = temp_CRC[i - K];

	deleteIntegerArray(temp_CRC);
	vector<int>().swap(temp_input);
}

int PolarCoder::CRC_check(int* u, int KwithCRC, int crcLength)
{
	vector<int> input(KwithCRC,0);

	for (int i = 0; i < KwithCRC; i++)
	{
		input[i] = u[Iw_N[N - i - 1].index];
	}

	/*int j = 0;
	for (int i=N-1;i>=0;i--)
	{
		if (mark_frozenPosition[i])
		{
			input[j++] = u[i];
		}
	}*/

	long int  POLY;
	if (CRCLength == 0)
	{
		return 0;
	}
	else if (CRCLength == 3)
	{
		POLY = 0x9;
	}
	else if (CRCLength == 4)
	{
		POLY = 0x13;
	}
	else if (CRCLength == 5)
	{
		POLY = 0x25;
	}
	else if (CRCLength == 6)
	{
		POLY = 0x67;
	}
	else if (CRCLength == 7)
	{
		POLY = 0xCF;
	}
	else if (CRCLength == 8)
	{
		POLY = 0x131;
	}
	else if (CRCLength == 9)
	{
		POLY = 0x233;
	}
	else if (CRCLength == 10)
	{
		POLY = 0x64F;
	}
	else if (CRCLength == 11)
	{
		POLY = 0xBB7;
	}
	else if (CRCLength == 12)
	{
		POLY = 0x180F;
	}
	else if (CRCLength == 13)
	{
		POLY = 0x357F;
	}
	else if (CRCLength == 14)
	{
		POLY = 0x6E57;
	}
	else if (CRCLength == 15)
	{
		POLY = 0x9E47;
	}
	else if (CRCLength == 16)
	{
		POLY = 0x18005;
	}
	else if (CRCLength == 17)
	{
		POLY = 0x2df4f;
	}
	else if (CRCLength == 18)
	{
		POLY = 0x472f3;
	}
	else if (CRCLength == 19)
	{
		POLY = 0xdf6af;
	}
	else if (CRCLength == 20)
	{
		POLY = 0x189b0f;
	}
	else if (CRCLength == 21)
	{
		POLY = 0x2e0fd5;
	}
	else if (CRCLength == 24)
	{
		POLY = 0x1864CFB;
	}
	else
	{
		printf("\n Wrong CRCLength at CRC.\n");
	}
	long int reg = division_mod2(input, POLY, CRCLength);
	return reg;
}