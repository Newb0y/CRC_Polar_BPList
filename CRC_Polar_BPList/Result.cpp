#include "Result.h"
#include "FileName.h"
#include<iostream>
#include<fstream>
#include <windows.h>

Result::Result(void)
{
}


Result::~Result(void)
{
}

void Result::init(double snr1)
{
	snr = snr1;
	simulationNum = 0;
	errorBitNum = 0;//错误比特数
	errorFrameNum = 0;//错误比特数
}

void Result:: addResultOnce(ResultOnce* resultOnce)
{
	errorBitNum += resultOnce->errorBitNum;
	errorFrameNum += resultOnce->errorFrameNum;
}

void Result::outputResultToConsole(int gap)
{
	int K = polar->K;
	double BER= (double)errorBitNum/(simulationNum * K);
	double BLER= (double)errorFrameNum/simulationNum;
	if (simulationNum % gap == 0)
	{
		cout<<"信噪比:"<<snr<<"    仿真次数:"<<simulationNum<<"    错误次数:"<<errorFrameNum<<"    BER_temp="<<BER<<"   BLER="<<BLER<<"\n";
		outputOtherResultToConsole();
	}
}

void Result::outputResultToFile()
{
	int K = polar->K;
	double BER= (double)errorBitNum/(simulationNum * K);
	double BLER= (double)errorFrameNum/simulationNum;
	ofstream oResult(RESULT_FILE,ios::app);
	oResult<<"信噪比:"<<snr<<"    仿真次数:"<<simulationNum<<"    错误次数:"<<errorFrameNum<<"    BER_temp="<<BER<<"   BLER="<<BLER<<"\n";
	oResult.close();
	ofstream oBER(BER_FILE,ios::app);
	oBER<<BER<<" ";
	oBER.close();
	ofstream oBLER(BLER_FILE,ios::app);
	oBLER<<BLER<<" ";
	oBLER.close();
	outputOtherResultToFile();
}

void Result::outputBaseInfo()
{
	outputToConsole();
	ofstream oResult(RESULT_FILE,ios::app);
	outputToFile(oResult);
	oResult.close();
	polar->outputBaseInfo();
	polar->outputDetailedInfo();
}

//输出到控制台
void Result::outputToConsole()
{
	SYSTEMTIME sys; 
    GetLocalTime( &sys ); 
	//输出仿真环境
	cout<<"*****************"<<sys.wYear<<"年"<<sys.wMonth<<"月"<<sys.wDay<<"日"<<sys.wHour<<"时"<<sys.wMinute<<"分"<<sys.wSecond+0.001*sys.wMilliseconds<<"秒";
	switch (sys.wDayOfWeek)
    {
		case 0:	cout<<"  星期日****************\n";	break;
		case 1:	cout<<"  星期一****************\n";	break;
		case 2:	cout<<"  星期二****************\n";	break;
		case 3:	cout<<"  星期三****************\n";	break;
		case 4:	cout<<"  星期四****************\n";	break;
		case 5:	cout<<"  星期五****************\n";	break;
		case 6:	cout<<"  星期六****************\n";	break;
    }
}
//输出文件中
void Result::outputToFile(ofstream& outfile)
{
	SYSTEMTIME sys; 
    GetLocalTime( &sys ); 

	//输出仿真环境
	outfile<<"\n*****************"<<sys.wYear<<"年"<<sys.wMonth<<"月"<<sys.wDay<<"日"<<sys.wHour<<"时"<<sys.wMinute<<"分"<<sys.wSecond+0.001*sys.wMilliseconds<<"秒";
	switch (sys.wDayOfWeek)
    {
		case 0:	outfile<<"  星期日****************\n";	break;
		case 1:	outfile<<"  星期一****************\n";	break;
		case 2:	outfile<<"  星期二****************\n";	break;
		case 3:	outfile<<"  星期三****************\n";	break;
		case 4:	outfile<<"  星期四****************\n";	break;
		case 5:	outfile<<"  星期五****************\n";	break;
		case 6:	outfile<<"  星期六****************\n";	break;
    }
}

