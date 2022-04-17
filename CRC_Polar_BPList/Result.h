/**
@Date 2020/12/29
@Author �����
@Description ���ڷ�װһ��SNR�·���Ľ��������ж����������������̳и�����дoutputOtherResultToConsole();outputOtherResultToFile();
*/
#pragma once
#include "ResultOnce.h"
#include "PolarCoder.h"
class Result
{
	public:
		double snr;
		int simulationNum;
		int errorBitNum;//���������
		int errorFrameNum;
		PolarCoder* polar;
		Result();
		~Result(void);
		virtual void init(double snr1);
		virtual void addResultOnce(ResultOnce* resultOnce);
		void outputResultToConsole(int gap);
		void outputResultToFile();
		void outputBaseInfo();
	protected:
		virtual void outputOtherResultToConsole(){return;}
		virtual void outputOtherResultToFile(){return;}
	private:
		void outputToConsole();
		void outputToFile(ofstream& outfile);
};

