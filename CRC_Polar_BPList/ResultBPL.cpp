#include "ResultBPL.h"
#include "FileName.h"
#include<fstream>
#include<iostream>

ResultBPL::ResultBPL(int N1)
{
	N = N1;
	errorBitDistribution = new int[N];
}


ResultBPL::~ResultBPL(void)
{
	delete[] errorBitDistribution;
}

void ResultBPL::init(double snr1)
{
	Result::init(snr1);
	for(int i = 0 ; i < N;i++)
	{
		errorBitDistribution[i] = 0;
	}
}

void ResultBPL::addResultOnce(ResultOnce* resultOnce)
{
	Result::addResultOnce(resultOnce);
	for(int i = 0 ; i < N;i++)
	{
		errorBitDistribution[i] += resultOnce->errorBitDistribution[i];
	}

	delete[] resultOnce->errorBitDistribution;
}

void ResultBPL::outputOtherResultToConsole()
{

}
void ResultBPL::outputOtherResultToFile()
{
	ofstream oErrorBitDistribution(BED_FILE);
	for(int i = 0; i < N; i++)
	{
		oErrorBitDistribution<<errorBitDistribution[i]<<" ";
		//oErrorBitDistribution<<" "<<errorBitDistribution[i];
	}
	oErrorBitDistribution.close();
}
