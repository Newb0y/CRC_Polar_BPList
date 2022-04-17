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
	errorBitNum = 0;//���������
	errorFrameNum = 0;//���������
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
		cout<<"�����:"<<snr<<"    �������:"<<simulationNum<<"    �������:"<<errorFrameNum<<"    BER_temp="<<BER<<"   BLER="<<BLER<<"\n";
		outputOtherResultToConsole();
	}
}

void Result::outputResultToFile()
{
	int K = polar->K;
	double BER= (double)errorBitNum/(simulationNum * K);
	double BLER= (double)errorFrameNum/simulationNum;
	ofstream oResult(RESULT_FILE,ios::app);
	oResult<<"�����:"<<snr<<"    �������:"<<simulationNum<<"    �������:"<<errorFrameNum<<"    BER_temp="<<BER<<"   BLER="<<BLER<<"\n";
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

//���������̨
void Result::outputToConsole()
{
	SYSTEMTIME sys; 
    GetLocalTime( &sys ); 
	//������滷��
	cout<<"*****************"<<sys.wYear<<"��"<<sys.wMonth<<"��"<<sys.wDay<<"��"<<sys.wHour<<"ʱ"<<sys.wMinute<<"��"<<sys.wSecond+0.001*sys.wMilliseconds<<"��";
	switch (sys.wDayOfWeek)
    {
		case 0:	cout<<"  ������****************\n";	break;
		case 1:	cout<<"  ����һ****************\n";	break;
		case 2:	cout<<"  ���ڶ�****************\n";	break;
		case 3:	cout<<"  ������****************\n";	break;
		case 4:	cout<<"  ������****************\n";	break;
		case 5:	cout<<"  ������****************\n";	break;
		case 6:	cout<<"  ������****************\n";	break;
    }
}
//����ļ���
void Result::outputToFile(ofstream& outfile)
{
	SYSTEMTIME sys; 
    GetLocalTime( &sys ); 

	//������滷��
	outfile<<"\n*****************"<<sys.wYear<<"��"<<sys.wMonth<<"��"<<sys.wDay<<"��"<<sys.wHour<<"ʱ"<<sys.wMinute<<"��"<<sys.wSecond+0.001*sys.wMilliseconds<<"��";
	switch (sys.wDayOfWeek)
    {
		case 0:	outfile<<"  ������****************\n";	break;
		case 1:	outfile<<"  ����һ****************\n";	break;
		case 2:	outfile<<"  ���ڶ�****************\n";	break;
		case 3:	outfile<<"  ������****************\n";	break;
		case 4:	outfile<<"  ������****************\n";	break;
		case 5:	outfile<<"  ������****************\n";	break;
		case 6:	outfile<<"  ������****************\n";	break;
    }
}

